#pragma once
#include "ICollider.h"
#include "IColliderVisitor.h"

namespace CaramelEngine
{
    struct CubeHitJudgeData
    {
        // ここにフィナンに判定に必要な情報を聞いて、その情報だけを持つ中身を書く
        std::shared_ptr<Transform> Transform;
        Cvector<CVector3> Verts;
        CVector3 Center;
        CVector3 Axis[3];    // 正規化済み
        CVector3 HalfExtent;
    };

    class CCube : public Component, public ICollider
    {
    public:

        inline CCube() noexcept = delete;

        // コンストラクタ
        CCube(std::weak_ptr<Actor> owner) noexcept :
            Component(owner, "CCube"),
            _transform()
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

        CubeHitJudgeData& getHitJudgeData() const
        {
            // Transform から取得
            const CVector3 pos = _transform->getPosition();
            const CQuaternion rot = _transform->getRotation();
            const CVector3 scale = _transform->getScale();

            // Center
            _cache.Center = pos;

            // HalfExtent（ローカルサイズ * スケール）
            _cache.HalfExtent = scale * 0.5f;

            // 回転からワールド軸を作る
            // ローカル基底 (1,0,0),(0,1,0),(0,0,1) を回す
            _cache.Axis[0] = _transform->getRight();   // X軸
            _cache.Axis[1] = _transform->getUp();      // Y軸
            _cache.Axis[2] = _transform->getForward(); // Z軸

            // 念のため正規化
            for (int i = 0; i < 3; ++i)
                _cache.Axis[i] = _cache.Axis[i].normalized();

            // 頂点（8点）を生成（必要な場合のみ）
            _cache.Verts.clear();
            _cache.Verts.reserve(8);

            for (int x = -1; x <= 1; x += 2)
            {
                for (int y = -1; y <= 1; y += 2)
                {
                    for (int z = -1; z <= 1; z += 2)
                    {
                        CVector3 local =
                            _cache.Axis[0] * (_cache.HalfExtent.getX() * x) +
                            _cache.Axis[1] * (_cache.HalfExtent.getY() * y) +
                            _cache.Axis[2] * (_cache.HalfExtent.getZ() * z);

                        _cache.Verts.push_back(_cache.Center + local);
                    }
                }
            }

            return _cache;
        }

        eColliderType getColliderType() override { return eColliderType::Cube; };

    private:

        std::shared_ptr<Transform> _transform;
        // キャッシュ（毎フレーム更新想定）
        mutable CubeHitJudgeData _cache;
    };
}