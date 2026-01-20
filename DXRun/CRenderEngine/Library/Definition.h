#pragma once
#include "CaramelEngine.h"
#include "IRenderer.h"
#include "IShader.h"

namespace CaramelEngine
{
    struct RenderCommand
    {
        const IRenderer* const renderer;
        const IShader* const shader;
        uint32_t sortKey;
        float depth;
    };

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 uv;
    };

    struct LambertConstants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4X4 viewProj;

        DirectX::XMFLOAT3 lightDir;
        float padding0;

        DirectX::XMFLOAT3 lightColor;
        float padding1;
    };
}