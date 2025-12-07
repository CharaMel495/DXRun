#pragma once
#include <memory>
#include <string>
#include "Component.h"
#include "CVector2.h"
#include "Color.h"
#include "Sprite.h"
#include "SpriteManager.h"

namespace CaramelEngine
{
    class SpriteRenderer : public Component
    {
    public:
        explicit SpriteRenderer(std::weak_ptr<Actor> owner) noexcept;

        void initialize() noexcept override;
        void update() noexcept;
        void fixedUpdate() noexcept;
        void draw() noexcept;

        const std::shared_ptr<Sprite> getSprite() const noexcept { return _sprite; }
        void SetSprite(std::shared_ptr<Sprite> sprite) noexcept { _sprite = sprite; }

        void SetTransform(std::shared_ptr<Transform> tr) { _transform = tr; }

    private:
        std::shared_ptr<Sprite> _sprite = nullptr;
        CVector2 _offset{ 0.f, 0.f };
        Color _color = Color::White();
        bool _visible = true;
        int _blinkFrame = 0;
        int _blinkInterval = 5;

        std::shared_ptr<Transform> _transform = nullptr;
    };

}
