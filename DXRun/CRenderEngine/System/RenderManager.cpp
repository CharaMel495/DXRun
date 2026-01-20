#include "pch.h"
#include "RenderManager.h"

using namespace CaramelEngine;

void RenderManager::initialize(HWND hwnd, const RenderSettings& settings)
{
    _hwnd = hwnd;
    _settings = settings;

    _gfx = std::make_unique<GraphicsDevice>();
    _gfx->registerNotify(this);

    DeviceOptions opt{};
#if defined(_DEBUG)
    opt.enableDebugLayer = true;
#endif
    opt.allowTearing = settings.allowTearing;
    opt.enableHdr = settings.enableHdr;
    opt.flipPresent = settings.flipPresent;
    opt.minFeatureLevel = settings.minFeatureLevel;

    _gfx->initialize(opt);

    // main swapchain
    RenderTargetDesc mainDesc{};
    mainDesc.width = settings.width;
    mainDesc.height = settings.height;
    mainDesc.colorFormat = settings.backBufferFormat;
    mainDesc.depthFormat = settings.depthFormat;

    auto sc = std::make_unique<SwapChainTarget>();
    sc->initialize(*_gfx, _hwnd, mainDesc, settings.backBufferCount, settings.flipPresent);
    _mainSwapChain = std::move(sc);

    ensurePipelines();
}

void RenderManager::shutdown() noexcept
{
    _offscreen.clear();
    _gamePipeline.reset();
    _editorPipeline.reset();
    _mainSwapChain.reset();
    _gfx.reset();
}

void RenderManager::update() noexcept
{
    // 将来: frame stats, gpu timers, resource GC, etc
}

void RenderManager::draw()
{
    handleResizeIfNeeded();

    ensurePipelines();

    IRenderPipeline* pipe = _gamePipeline.get();
    pipe->begin(*_gfx, *_mainSwapChain);

    _queue.clear();
    pipe->buildQueue(_queue);
    _queue.sort();

    pipe->execute(*_gfx, *_mainSwapChain, _queue);
    pipe->end(*_gfx, *_mainSwapChain);

    _mainSwapChain->present(*_gfx, _settings.vsync);
}

void RenderManager::drawEditor()
{
    handleResizeIfNeeded();

    ensurePipelines();

    IRenderPipeline* pipe = _editorPipeline.get();

    // editorは例としてオフスクリーンに描く（SceneView想定）
    RenderTargetDesc sceneDesc{};
    sceneDesc.width = _settings.width;
    sceneDesc.height = _settings.height;
    sceneDesc.colorFormat = _settings.backBufferFormat;
    sceneDesc.depthFormat = _settings.depthFormat;

    auto& sceneTarget = getOrCreateOffscreen("editorScene", sceneDesc);

    pipe->begin(*_gfx, sceneTarget);

    _queue.clear();
    pipe->buildQueue(_queue);
    _queue.sort();

    pipe->execute(*_gfx, sceneTarget, _queue);
    pipe->end(*_gfx, sceneTarget);

    // ここで sceneTarget.SRV を使ったエディタUI合成などを行い、
    // 最終的に main swapchain に表示する、という構成に拡張できる。
}

IRenderTarget& RenderManager::getOrCreateOffscreen(const std::string& name, const RenderTargetDesc& desc)
{
    auto it = _offscreen.find(name);
    if (it != _offscreen.end())
    {
        // サイズ違いなら resize
        if (it->second->width() != desc.width || it->second->height() != desc.height)
            it->second->resize(*_gfx, desc.width, desc.height);

        return *(it->second);
    }

    auto off = std::make_unique<OffscreenTarget>();
    off->initialize(*_gfx, desc);

    auto* ptr = off.get();
    _offscreen.emplace(name, std::move(off));
    return *ptr;
}

void RenderManager::ensurePipelines()
{
    if (!_gamePipeline)
    {
        // あとであなたのGamePipeline実装に差し替える
        _gamePipeline = std::make_unique<DefaultGamePipeline>();
    }
    if (!_editorPipeline)
    {
        // あとであなたのEditorPipeline実装に差し替える
        _editorPipeline = std::make_unique<DefaultEditorPipeline>();
    }
}

void RenderManager::handleResizeIfNeeded()
{
    if (!_resizePending)
        return;

    _resizePending = false;

    if (_mainSwapChain)
        _mainSwapChain->resize(*_gfx, _pendingWidth, _pendingHeight);

    // 既存オフスクリーンは必要なタイミングで個別にresizeされる想定
}

void RenderManager::onDeviceLost()
{
    // 1) pipeline 側に解放させるならここで通知
    // 2) 既存リソースを破棄
    _offscreen.clear();
    _gamePipeline.reset();
    _editorPipeline.reset();
    _mainSwapChain.reset();
}

void RenderManager::onDeviceRestored()
{
    // graphicsDevice側でdevice/contextが復旧した後、swapchainやpipelineを再構築
    RenderTargetDesc mainDesc{};
    mainDesc.width = _settings.width;
    mainDesc.height = _settings.height;
    mainDesc.colorFormat = _settings.backBufferFormat;
    mainDesc.depthFormat = _settings.depthFormat;

    auto sc = std::make_unique<SwapChainTarget>();
    sc->initialize(*_gfx, _hwnd, mainDesc, _settings.backBufferCount, _settings.flipPresent);
    _mainSwapChain = std::move(sc);

    ensurePipelines();
}
