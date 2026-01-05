#pragma once
#include "ICollider.h"
#include "IColliderVisitor.h"

namespace CaramelEngine
{
    struct SphereHitJudgeData
    {
        // ここにフィナンに判定に必要な情報を聞いて、その情報だけを持つ中身を書く
        CVector3 Center;
        float Radius;
    };

    class CSphere : public Component, public ICollider
    {
    public:

        inline CSphere() noexcept = delete;

        // コンストラクタ
        CSphere(std::weak_ptr<Actor> owner) noexcept :
            Component(owner, "CSphere"),
            _transform(),
            _cache()
        {
        }

        //初期化メソッド (Componentからの継承)
        void initialize() noexcept override
        {

        }

        void setTransform(std::shared_ptr<Transform> transform)
        {
            _transform = transform;
        }

        void accept(IColliderVisitor& visitor) override
        {
            visitor.visit(*this); // 自分を訪問させる
        }

        SphereHitJudgeData& getHitJudgeData() const
        {
            _cache.Center = _transform->getPosition();
            _cache.Radius = _transform->getScale().getX() / 2; // Sphereはxyz全て等しい前提
            return _cache;
        }

        eColliderType getColliderType() override { return eColliderType::Sphere; };

    private:

        std::shared_ptr<Transform> _transform;
        mutable SphereHitJudgeData _cache;
    };
}