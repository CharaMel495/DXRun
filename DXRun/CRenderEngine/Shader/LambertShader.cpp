#include "pch.h"
#include "LambertShader.h"
#include "GraphicsDevice.h"

bool LambertShader::initialize(GraphicsDevice& gfx)
{
    // --- Shader bytecode（事前コンパイル想定） ---
    extern const unsigned char g_lambertVS[];
    extern size_t g_lambertVSSize;
    extern const unsigned char g_lambertPS[];
    extern size_t g_lambertPSSize;

    if (FAILED(gfx.device().createVertexShader(
        g_lambertVS,
        g_lambertVSSize,
        _vertexShader.ReleaseAndGetAddressOf())))
    {
        return false;
    }

    if (FAILED(gfx.device().createPixelShader(
        g_lambertPS,
        g_lambertPSSize,
        _pixelShader.ReleaseAndGetAddressOf())))
    {
        return false;
    }

    // --- InputLayout ---
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    if (FAILED(gfx.device().createInputLayout(
        layout,
        _countof(layout),
        g_lambertVS,
        g_lambertVSSize,
        _inputLayout.ReleaseAndGetAddressOf())))
    {
        return false;
    }

    // --- Constant Buffer ---
    D3D11_BUFFER_DESC cbDesc{};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(LambertConstants);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(gfx.device().createBuffer(
        &cbDesc,
        nullptr,
        _constantBuffer.ReleaseAndGetAddressOf())))
    {
        return false;
    }

    return true;
}

void LambertShader::bind(CDeviceContext& context) const noexcept
{
    context.setInputLayout(_inputLayout.Get());
    context.setVertexShader(_vertexShader.Get());
    context.setPixelShader(_pixelShader.Get());
}

void LambertShader::applyCBuffer(
    CDeviceContext& context,
    const RenderCommand& command) const noexcept
{
    // Renderer -> Transform -> World 行列取得
    const auto& transform =
        command.renderer->getOwner().lock()->getTransform();

    DirectX::XMMATRIX world = transform->getWorldMatrix();

    DirectX::XMStoreFloat4x4(
        &_constants->world,
        DirectX::XMMatrixTranspose(world)
    );

    // viewProj は Shader 側で保持
    // light 情報も Shader 側

    context.UpdateConstantBuffer(
        _constantBuffer.Get(),
        _constants.get(),
        sizeof(LambertConstants)
    );

    // VS / PS 両方にバインド
    context.setVSConstantBuffer(0, _constantBuffer.Get());
    context.setPSConstantBuffer(0, _constantBuffer.Get());
}
