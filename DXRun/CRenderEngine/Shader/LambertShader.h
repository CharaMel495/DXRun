// LambertShader.h
#pragma once
#include "IShader.h"
#include "Definition.h"
#include <wrl.h>

namespace CaramelEngine
{
    class GraphicsDevice;

    class LambertShader final : public IShader
    {
    public:
        LambertShader() = default;
        ~LambertShader() override = default;

        bool initialize(GraphicsDevice& gfx);

        void bind(CDeviceContext& context) const noexcept override;
        void applyCBuffer(
            CDeviceContext& context,
            const RenderCommand& command) const noexcept override;

        void setViewProj(const DirectX::XMFLOAT4X4& vp) noexcept { _constants->viewProj = vp; };
        void setLightDirection(const DirectX::XMFLOAT3& dir) noexcept { _constants->lightDir = dir; };
        void setLightColor(const DirectX::XMFLOAT3& color) noexcept { _constants->lightColor = color; };

    private:
        Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer;

        std::shared_ptr<LambertConstants> _constants{};
    };
}
