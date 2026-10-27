#pragma once
#include "ICollider.h"
#include "IColliderVisitor.h"

namespace CaramelEngine
{
    struct RayParameter
    {
        // ここにフィナンに判定に必要な情報を聞いて、その情報だけを持つ中身を書く
        CVector3 Origin;
        CVector3 Direction;
        float Length;
    };

    class CRay : public ICollider
    {
    public:

        inline CRay() noexcept = delete;

        // コンストラクタ
        CRay(std::weak_ptr<Actor> owner) noexcept :
            _origin(),
            _direction(),
            _length(),
            _cache()
        {
        }

        //初期化メソッド (Componentからの継承)
        void initialize(RayParameter rayparameter) noexcept
        {
			_origin = rayparameter.Origin;
			_direction = rayparameter.Direction;
			_length = rayparameter.Length;
        }

        void accept(IColliderVisitor& visitor) override
        {
            visitor.visit(*this); // 自分を訪問させる
        }

        const RayParameter& getHitJudgeData() const noexcept
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
        mutable RayParameter _cache;
    };
}