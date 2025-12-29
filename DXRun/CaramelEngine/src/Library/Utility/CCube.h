#pragma once
#include "ICollider.h"
#include "IColliderVisitor.h"

namespace CaramelEngine
{
    struct CubeHitJudgeData
    {
        // ここにフィナンに判定に必要な情報を聞いて、その情報だけを持つ中身を書く
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

        void setTransform(std::shared_ptr<Transform> transform)
        {
            _transform = transform;
        }

        void accept(IColliderVisitor& visitor) override
        {
            //visitor.visit(*this); // 自分を訪問させる
        }

    private:

        std::shared_ptr<Transform> _transform;
    };
}