#include "pch.h"
#include "SpriteRenderer.h"
#include "RenderQueue.h"

namespace CaramelEngine
{
    SpriteRenderer::SpriteRenderer(std::weak_ptr<Actor> owner) noexcept
        : Component(owner, "SpriteRenderer")
    {
    }

    void SpriteRenderer::initialize() noexcept
    {
        if (auto owner = getOwner().lock())
            _transform = owner->getTransform();
    }

    void SpriteRenderer::update() noexcept
    {
        if (_blinkFrame > 0)
        {
            _blinkFrame--;
            _visible = ((_blinkFrame / _blinkInterval) % 2) != 0;
        }
    }

    void SpriteRenderer::fixedUpdate() noexcept
    {
       
    }

    void SpriteRenderer::draw() noexcept
    {
        if (!_visible || !_sprite || !_transform)
            return;

        RenderCommand cmd{};
        cmd.pass = RenderPassType::Sprite;
        cmd.srv = _sprite->getSRV();

        // Transformのワールド行列から各値を抽出
        DirectX::XMMATRIX world = _transform->getWorldMatrix();
        DirectX::XMFLOAT4X4 m;
        DirectX::XMStoreFloat4x4(&m, world);

        cmd.position = { m._41, m._42, m._43 };
        cmd.scale = _transform->getScale().toXMVECTOR();
        cmd.rotation = _transform->getRotation().toEulerXYZ().getZ(); // Z軸回転を想定

        cmd.color = _color;

        // UVスケール＆オフセット
        const auto uv = _sprite->getUVRect();
        cmd.uvOffset = { uv.u, uv.v };
        cmd.uvScale = { uv.width, uv.height };

        cmd.depth = 0.0f;
        cmd.sortKey = 0;

        RenderQueue::getInstance().add(cmd);
    }
}
