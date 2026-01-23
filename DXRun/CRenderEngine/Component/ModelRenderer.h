// ModelRenderer.h
#pragma once
#include "IRenderer.h"
#include "Definition.h"

namespace CaramelEngine
{
    class CModel;
    class IShader;

    class ModelRenderer final : public IRenderer
    {
    public:
        ModelRenderer(
            std::weak_ptr<Actor> owner,
            CString name) noexcept;

        ~ModelRenderer() override = default;

        void initialize() noexcept override;

        void setModel(CModel* model) noexcept { _model = model; }
        void setShader(IShader* shader) noexcept { _shader = shader; }

        void setCastShadow(bool enable) noexcept { _castShadow = enable; }
        void setReceiveShadow(bool enable) noexcept { _receiveShadow = enable; }

        // RenderQueue Ç…êœÇﬁ
        void render(RenderQueue& queue) noexcept override;

    protected:
        // é¿ draw
        void draw(GraphicsDevice& gfx) const noexcept override;

    private:
        CModel* _model = nullptr;   // îÒèäóL
        IShader* _shader = nullptr;   // îÒèäóL

        bool _castShadow = true;
        bool _receiveShadow = true;
    };
}
