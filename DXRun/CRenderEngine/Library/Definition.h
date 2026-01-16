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
}