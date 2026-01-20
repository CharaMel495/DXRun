#pragma once

namespace CaramelEngine
{
    class GraphicsDevice;
    class RenderQueue;
    class IRenderTarget;

    class IRenderPipeline
    {
    public:
        virtual ~IRenderPipeline() = default;

        virtual void begin(GraphicsDevice& gfx, IRenderTarget& target) = 0;
        virtual void buildQueue(RenderQueue& queue) = 0;
        virtual void execute(GraphicsDevice& gfx, IRenderTarget& target, RenderQueue& queue) = 0;
        virtual void end(GraphicsDevice& gfx, IRenderTarget& target) = 0;
    };

    // Ç∆ÇËÇ†Ç¶Ç∏ÅgåqÇ™ÇÈÅhÇΩÇﬂÇÃÉfÉtÉHÉãÉgé¿ëï
    class DefaultGamePipeline final : public IRenderPipeline
    {
    public:
        void begin(GraphicsDevice& gfx, IRenderTarget& target) override;
        void buildQueue(RenderQueue& queue) override;
        void execute(GraphicsDevice& gfx, IRenderTarget& target, RenderQueue& queue) override;
        void end(GraphicsDevice& gfx, IRenderTarget& target) override;
    };

    class DefaultEditorPipeline final : public IRenderPipeline
    {
    public:
        void begin(GraphicsDevice& gfx, IRenderTarget& target) override;
        void buildQueue(RenderQueue& queue) override;
        void execute(GraphicsDevice& gfx, IRenderTarget& target, RenderQueue& queue) override;
        void end(GraphicsDevice& gfx, IRenderTarget& target) override;
    };
}
