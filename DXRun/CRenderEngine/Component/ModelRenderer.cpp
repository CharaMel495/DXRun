// ModelRenderer.cpp
#include "pch.h"
#include "ModelRenderer.h"
#include "RenderQueue.h"
#include "Utility/CModel.h"
#include "GraphicsDevice.h"
#include "CDeviceContext.h"
#include "IShader.h"
#include "Definition.h"

using namespace CaramelEngine;

ModelRenderer::ModelRenderer(std::weak_ptr<Actor> owner, CString name) noexcept
    : IRenderer(owner, name)
{
}

void ModelRenderer::initialize() noexcept
{

}

void ModelRenderer::render(RenderQueue& queue) noexcept
{
    if (!_model || !_shader)
        return;

    // 深度ソート用（Z）
    const auto& transform =
        getOwner().lock()->getTransform();

    RenderCommand cmd
    { 
        this, 
        _shader, 
        static_cast<uint32_t>(RenderStage::World), 
        transform->getWorldPosition().getZ() 
    };
    
    queue.push(cmd);
}

void ModelRenderer::draw(GraphicsDevice& gfx) const noexcept
{
    if (!_model)
        return;

    auto& context = gfx.context();

    // トポロジは Renderer の責務
    context.setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    const auto& meshes = _model->getMeshes();

    for (const auto& mesh : meshes)
    {
        // --- VertexBuffer ---
        UINT stride = sizeof(Vertex);
        UINT offset = 0;

        context.setVertexBuffer(
            0,
            1,
            mesh.vertexBuffer.GetAddressOf(),
            &stride,
            &offset
        );

        // --- IndexBuffer ---
        context.setIndexBuffer(
            mesh.indexBuffer.Get(),
            DXGI_FORMAT_R32_UINT,
            0
        );

        // --- Draw ---
        context.drawIndexed(
            mesh.indexCount,
            0,
            0
        );
    }
}
