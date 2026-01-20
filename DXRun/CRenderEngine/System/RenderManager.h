#pragma once
#include "GraphicsDevice.h"
#include "RenderTargets.h"
#include "RenderQueue.h"
#include "RenderPipeline.h"

namespace CaramelEngine
{
    struct RenderSettings
    {
        uint32_t width = 1280;
        uint32_t height = 720;

        bool enableHdr = false;
        bool allowTearing = false;
        bool flipPresent = true;
        bool vsync = true;

        uint32_t backBufferCount = 2;
        DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
        DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    };

    enum class ePipeline : uint8_t
    {
        game,
        editor,
    };

    class RenderManager final : public IDeviceNotify
    {
    public:
        static RenderManager& get() noexcept
        {
            static RenderManager inst;
            return inst;
        }

        RenderManager(const RenderManager&) = delete;
        RenderManager& operator=(const RenderManager&) = delete;

        void initialize(HWND hwnd, const RenderSettings& settings);
        void shutdown() noexcept;

        void update() noexcept;

        void draw();        // main swapchain present
        void drawEditor();  // editor pipeline (offscreen etc)

        // offscreen for post-process
        IRenderTarget& getOrCreateOffscreen(const std::string& name, const RenderTargetDesc& desc);

        ISwapChainTarget& getMainSwapChain() noexcept { return *_mainSwapChain; }
        GraphicsDevice& gfx() noexcept { return *_gfx; }

        void setActivePipeline(ePipeline p) noexcept { _active = p; }

        // iDeviceNotify
        void onDeviceLost() override;
        void onDeviceRestored() override;

    private:
        RenderManager() = default;
        ~RenderManager() = default;

        void ensurePipelines();
        void handleResizeIfNeeded();

        HWND _hwnd = nullptr;
        RenderSettings _settings{};
        ePipeline _active = ePipeline::game;

        std::unique_ptr<GraphicsDevice> _gfx;

        std::unique_ptr<ISwapChainTarget> _mainSwapChain;
        std::unordered_map<std::string, std::unique_ptr<IRenderTarget>> _offscreen;

        std::unique_ptr<IRenderPipeline> _gamePipeline;
        std::unique_ptr<IRenderPipeline> _editorPipeline;

        RenderQueue _queue;

        uint32_t _pendingWidth = 0;
        uint32_t _pendingHeight = 0;
        bool _resizePending = false;
    };
}
