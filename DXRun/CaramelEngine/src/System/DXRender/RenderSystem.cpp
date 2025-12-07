#include "pch.h"
#include "RenderSystem.h"
#include <cassert>

using Microsoft::WRL::ComPtr;

// 簡易 HR チェック
static inline bool HR(HRESULT hr) { return SUCCEEDED(hr); }

namespace CaramelEngine
{
    bool RenderSystem::initialize(HWND hwnd, int width, int height, bool enableDebug)
    {
        _width = width;
        _height = height;

        // D3D11デバイスとスワップチェインの作成
        if (!createDeviceAndSwapChain(hwnd, width, height, enableDebug)) 
            return false;
        
        // バックバッファの作成(RTVなど)
        if (!createBackbufferViews(width, height)) 
            return false;
        
        // ラスタライザにビューポートをセット
        setDefaultViewport(width, height);

        RenderQueue::getInstance().initialize(_device.Get());

        return true;
    }

    void RenderSystem::shutdown()
    {
        _context.Reset();
        _rtv.Reset();
        _dsv.Reset();
        _depthTex.Reset();
        _swapchain.Reset();
        _device.Reset();
    }

    bool RenderSystem::resize(int width, int height)
    {
        if (width <= 0 || height <= 0) 
            return false;

        // 既存ビューを解放
        releaseBackbufferViews();

        // スワップチェインのバッファをリサイズ
        HRESULT hr = _swapchain->ResizeBuffers(
            0, // バッファ数そのまま
            static_cast<UINT>(width),
            static_cast<UINT>(height),
            DXGI_FORMAT_UNKNOWN, // 既定フォーマットのまま
            0
        );

        if (!HR(hr)) 
            return false;

        _width = width;
        _height = height;

        if (!createBackbufferViews(width, height)) 
            return false;

        setDefaultViewport(width, height);
        return true;
    }

    void RenderSystem::beginFrame(const float clearColor[4])
    {
        // 既定ターゲットをセット & クリア
        ID3D11RenderTargetView* rtvs[] = { _rtv.Get() };
        _context->OMSetRenderTargets(1, rtvs, _dsv.Get());

        // ビューポートは都度でもOK（コマンド・リセットの癖を付けたい場合）
        setDefaultViewport(_width, _height);

        _context->ClearRenderTargetView(_rtv.Get(), clearColor);
        _context->ClearDepthStencilView(_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void RenderSystem::endFrame(bool vsync)
    {
        // --- ここでRenderQueueのflushを呼ぶ ---
        RenderQueue::getInstance().flushAll();
        // Flip-Model: SyncInterval=1 で v-sync、0 でティアリング（フルスクリーン例外あり）
        _swapchain->Present(vsync ? 1 : 0, 0);
    }

    bool RenderSystem::createDeviceAndSwapChain(HWND hwnd, int width, int height, bool enableDebug)
    {
        // --- デバイス作成 ---
        UINT flags = 0;
        if (enableDebug) 
            flags |= D3D11_CREATE_DEVICE_DEBUG;

        // デバイスの作成。使用されるデバイスのレベルのテスト順
        D3D_FEATURE_LEVEL featureLevels[] = 
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };
        D3D_FEATURE_LEVEL createdFL = D3D_FEATURE_LEVEL_11_0;

        // 実際のデバイスの作成
        HRESULT hr = D3D11CreateDevice(
            nullptr,                    // デフォルトアダプタ
            D3D_DRIVER_TYPE_HARDWARE,   // 使用するドライバの種別 
            nullptr,                    // Software
            flags,                      // 様々な管理フラグ
            featureLevels,              // デバイスのレベル
            _countof(featureLevels),    // 実際のデバイスのレベル
            D3D11_SDK_VERSION,          // SDKのバージョン
            &_device,                   // 以下参照渡し
            &createdFL, 
            &_context
        );

        if (!HR(hr)) 
            return false;

        // --- SwapChain (Flip-Model) 作成 ---
        ComPtr<IDXGIDevice> dxgiDevice;
        hr = _device.As(&dxgiDevice);
        if (!HR(hr)) 
            return false;

        ComPtr<IDXGIAdapter> adapter;
        hr = dxgiDevice->GetAdapter(&adapter);
        if (!HR(hr)) 
            return false;

        ComPtr<IDXGIFactory2> factory;
        hr = adapter->GetParent(__uuidof(IDXGIFactory2), &factory);
        if (!HR(hr)) 
            return false;

        DXGI_SWAP_CHAIN_DESC1 scd = {};
        scd.Width = static_cast<UINT>(width);
        scd.Height = static_cast<UINT>(height);
        scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // ※ バックバッファは UNORM（最終ガンマはポストFXで）
        scd.SampleDesc.Count = 1;                  // MSAA なし（まずはシンプルに）
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 2;                       // ダブルバッファ（必要なら 3）
        scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 推奨のFlip-Model
        scd.Scaling = DXGI_SCALING_STRETCH;
        scd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        hr = factory->CreateSwapChainForHwnd(
            _device.Get(), hwnd, &scd, nullptr, nullptr, &_swapchain
        );
        if (!HR(hr)) return false;

        // Alt+Enter の自動フルスクリーン遷移を抑止（手動で管理したい場合）
        factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

        return true;
    }

    bool RenderSystem::createBackbufferViews(int width, int height)
    {
        // --- RTV（バックバッファ） ---
        ComPtr<ID3D11Texture2D> backbuf;
        HRESULT hr = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuf);
        if (!HR(hr)) return false;

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // SwapChainと一致
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        hr = _device->CreateRenderTargetView(backbuf.Get(), &rtvDesc, &_rtv);
        if (!HR(hr)) return false;

        // --- DSV（深度ステンシル） ---
        D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width = static_cast<UINT>(width);
        depthDesc.Height = static_cast<UINT>(height);
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 定番の24bit深度+8bit stencil
        depthDesc.SampleDesc.Count = 1;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = _device->CreateTexture2D(&depthDesc, nullptr, &_depthTex);
        if (!HR(hr)) return false;

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = depthDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Flags = 0;

        hr = _device->CreateDepthStencilView(_depthTex.Get(), &dsvDesc, &_dsv);
        if (!HR(hr)) return false;

        return true;
    }

    void RenderSystem::releaseBackbufferViews()
    {
        // 依存順に解放
        _context->OMSetRenderTargets(0, nullptr, nullptr);
        _dsv.Reset();
        _depthTex.Reset();
        _rtv.Reset();
    }

    void RenderSystem::setDefaultViewport(int width, int height)
    {
        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.Width = static_cast<float>(width);
        vp.Height = static_cast<float>(height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        _context->RSSetViewports(1, &vp);
    }
}