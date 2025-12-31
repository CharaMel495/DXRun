#pragma once
#include <DirectXMath.h>
#include <string>
#include <format>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "CVector3.h"
#include "CQuaternion.h"

namespace CaramelEngine
{
    class alignas(16) Transform
    {
    private:
        CVector3 _position;
        CQuaternion _rotation;
        CVector3 _scale;

        Transform* _parent = nullptr;
        std::vector<Transform*> _children;

        mutable DirectX::XMMATRIX _localMatrix{};
        mutable DirectX::XMMATRIX _worldMatrix{};
        mutable bool _dirty = true;

    private:
        void markDirty() const
        {
            if (_dirty) return;
            _dirty = true;
            for (auto* child : _children)
            {
                if (child)
                    child->markDirty();
            }
        }

        void updateMatrix() const
        {
            if (!_dirty) return;

            using namespace DirectX;

            _localMatrix =
                XMMatrixScalingFromVector(_scale.toXMVECTOR()) *
                XMMatrixRotationQuaternion(_rotation.toXMVECTOR()) *
                XMMatrixTranslationFromVector(_position.toXMVECTOR());

            _worldMatrix = _parent
                ? _localMatrix * _parent->getWorldMatrix()
                : _localMatrix;

            _dirty = false;
        }

        DirectX::XMMATRIX getRotationMatrix() const
        {
            return DirectX::XMMatrixRotationQuaternion(_rotation.toXMVECTOR());
        }

    public:
        Transform(
            const CVector3& position = {},
            const CQuaternion& rotation = CQuaternion::identity(),
            const CVector3& scale = { 1,1,1 },
            Transform* parent = nullptr)
            : _position(position)
            , _rotation(rotation)
            , _scale(scale)
        {
            if (parent)
                setParent(parent);
        }

        // --- setters ---
        void setPosition(const CVector3& pos) { _position = pos; markDirty(); }
        void setRotation(const CQuaternion& rot) { _rotation = rot; markDirty(); }
        void setScale(const CVector3& scl) { _scale = scl; markDirty(); }

        void setParent(Transform* parent)
        {
            if (_parent == parent)
                return;

            // Œ»Ý‚Ìe‚©‚çŠO‚·
            if (_parent)
            {
                auto& siblings = _parent->_children;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
            }

            _parent = parent;

            if (_parent)
                _parent->_children.push_back(this);

            markDirty();
        }

        // --- getters ---
        CVector3 getPosition() const { return _position; }
        CQuaternion getRotation() const { return _rotation; }
        CVector3 getScale() const { return _scale; }

        CVector3 getForward() const
        {
            using namespace DirectX;
            return CVector3(XMVector3TransformNormal({ 0,0,1,0 }, getRotationMatrix()));
        }

        CVector3 getRight() const
        {
            using namespace DirectX;
            return CVector3(XMVector3TransformNormal({ 1,0,0,0 }, getRotationMatrix()));
        }

        CVector3 getUp() const
        {
            using namespace DirectX;
            return CVector3(XMVector3TransformNormal({ 0,1,0,0 }, getRotationMatrix()));
        }

        void lookAt(const CVector3& target, const CVector3& up = { 0,1,0 })
        {
            using namespace DirectX;
            auto view = XMMatrixLookAtRH(
                _position.toXMVECTOR(),
                target.toXMVECTOR(),
                up.toXMVECTOR());

            _rotation = CQuaternion(
                XMQuaternionRotationMatrix(XMMatrixTranspose(view)));

            markDirty();
        }

        void rotateAround(const CVector3& point, const CVector3& axis, float angleRadians)
        {
            using namespace DirectX;
            XMVECTOR pt = point.toXMVECTOR();
            XMVECTOR offset = _position.toXMVECTOR() - pt;
            XMMATRIX rot = XMMatrixRotationAxis(axis.toXMVECTOR(), angleRadians);

            offset = XMVector3TransformNormal(offset, rot);
            _position = CVector3(pt + offset);
            _rotation = _rotation * CQuaternion(XMQuaternionRotationMatrix(rot));

            markDirty();
        }

        void interpolateTo(const Transform& target, float t)
        {
            _position = CVector3::lerp(_position, target._position, t);
            _rotation = CQuaternion::slerp(_rotation, target._rotation, t);
            _scale = CVector3::lerp(_scale, target._scale, t);
            markDirty();
        }

        DirectX::XMMATRIX getLocalMatrix() const
        {
            updateMatrix();
            return _localMatrix;
        }

        DirectX::XMMATRIX getWorldMatrix() const
        {
            updateMatrix();
            return _worldMatrix;
        }

        DirectX::XMMATRIX getInverseMatrix() const
        {
            return DirectX::XMMatrixInverse(nullptr, getWorldMatrix());
        }

        std::string toJSON() const
        {
            auto p = _position; auto s = _scale; auto r = _rotation;
            return std::format(
                "{{\"pos\":[{:.3f},{:.3f},{:.3f}],"
                "\"rot\":[{:.3f},{:.3f},{:.3f},{:.3f}],"
                "\"scale\":[{:.3f},{:.3f},{:.3f}]}}",
                p.getX(), p.getY(), p.getZ(),
                r.getX(), r.getY(), r.getZ(), r.getW(),
                s.getX(), s.getY(), s.getZ());
        }

        void debugPrint() const
        {
            printf("Transform\n");
            printf(" Pos: (%.2f, %.2f, %.2f)\n", _position.getX(), _position.getY(), _position.getZ());
            printf(" Rot: (%.2f, %.2f, %.2f, %.2f)\n",
                _rotation.getX(), _rotation.getY(), _rotation.getZ(), _rotation.getW());
            printf(" Scl: (%.2f, %.2f, %.2f)\n", _scale.getX(), _scale.getY(), _scale.getZ());
        }
    };
}
