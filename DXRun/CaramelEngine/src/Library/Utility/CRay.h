#pragma once
#include "ICollider.h"
#include "IColliderVisitor.h"

namespace CaramelEngine
{
    struct RayHitJudgeData
    {
        // ここにフィナンに判定に必要な情報を聞いて、その情報だけを持つ中身を書く
        CVector3 Origin;
        CVector3 Direction;
        float Length;
    };

    class CRay : public Component, public ICollider
    {
    public:

        inline CRay() noexcept = delete;

        // コンストラクタ
        CRay(std::weak_ptr<Actor> owner) noexcept :
            Component(owner, "CRay"),
            _origin(),
            _direction(),
            _length()
        {
        }

        //初期化メソッド (Componentからの継承)
        void initialize() noexcept override
        {

        }

        void accept(IColliderVisitor& visitor) override
        {
            visitor.visit(*this); // 自分を訪問させる
        }

        const RayHitJudgeData& getHitJudgeData() const noexcept
        {
            _cache.Origin = _origin;
            _cache.Direction = _direction.normalized(); // 念のため
            _cache.Length = _length;
            return _cache;
        }

        eColliderType getColliderType() override { return eColliderType::Ray; };

    private:

        CVector3 _origin;
        CVector3 _direction;
        float _length;
        mutable RayHitJudgeData _cache;
    };
}