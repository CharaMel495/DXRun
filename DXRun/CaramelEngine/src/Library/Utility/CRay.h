#pragma once
#include "ICollider.h"
#include "IColliderVisitor.h"

namespace CaramelEngine
{
    struct RayHitJudgeData
    {
        // ここにフィナンに判定に必要な情報を聞いて、その情報だけを持つ中身を書く
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

        void accept(IColliderVisitor& visitor) override
        {
            //visitor.visit(*this); // 自分を訪問させる
        }

    private:

        CVector3 _origin;
        CVector3 _direction;
        CVector3 _length;
    };
}