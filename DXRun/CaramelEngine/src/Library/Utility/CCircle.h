#pragma once
#include "ICollider.h"

namespace CaramelEngine
{
    struct CircleHitJudgeData
    {
        std::shared_ptr<Transform> Transform;
        float Radius;
    };

    /// <summary>
    /// SIMD最適化された当たり判定矩形クラス (コンポーネント継承版)
    /// </summary>
    class CCircle : public Component, public ICollider
    {
    public:

        inline CCircle() noexcept = delete;

        // コンストラクタ
        CCircle(std::weak_ptr<Actor> owner) noexcept :
            Component(owner, "CRectangle"),
            _transform(),
            _radius(0.0f)
        {
        }

        // コピーコンストラクタ
        CCircle(const CCircle& other) noexcept :
            Component(other._owner, other._name),
            _transform(other._transform),
            _radius(other._radius)

        {
        }

        ~CCircle() {}

        //初期化メソッド (Componentからの継承)
        void initialize() noexcept override
        {

        }

        void setTransform(std::shared_ptr<Transform> transform)
        {
            _transform = transform;
            auto scale = _transform->getScale();
            _radius = scale.getX() * 0.5f;
        }

        void accept(IColliderVisitor& visitor) override 
        {
            visitor.visit(*this);
        }

        CircleHitJudgeData getHitJudgeData() const noexcept
        { 
            return
            {
                _transform,
                _radius
            };
        }

        eColliderType getColliderType() override { return eColliderType::Circle; };

    private:

        std::shared_ptr<Transform> _transform;
        float _radius;
    };
}