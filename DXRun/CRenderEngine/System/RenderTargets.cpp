#include "pch.h"
#include "RenderTargets.h"
#include "graphicsDevice.h"

using namespace CaramelEngine;
using namespace DX;
using Microsoft::WRL::ComPtr;

static DXGI_SWAP_EFFECT chooseSwapEffect(bool flipPresent)
{
    return flipPresent ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
}

void SwapChainTarget::initialize(GraphicsDevice& gfx, HWND hwnd, const RenderTargetDesc& desc,
    uint32_t backBufferCount, bool flipPresent)
{
    _hwnd = hwnd;
    _desc = desc;
    _backBufferCount = backBufferCount;
    _flipPresent = flipPresent;

    DXGI_SWAP_CHAIN_DESC1 sd{};
    sd.Width = desc.width;
    sd.Height = desc.height;
    sd.Format = desc.colorFormat;
    sd.SampleDesc.Count = 1; // swapchain MSAA is special; start with 1
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = backBufferCount;
    sd.SwapEffect = chooseSwapEffect(flipPresent);
    sd.Scaling = DXGI_SCALING_STRETCH;
    sd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    if (gfx.allowTearing())
        sd.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    ComPtr<IDXGISwapChain1> sc;
    ThrowIfFailed(gfx.factory()->CreateSwapChainForHwnd(
        gfx.device().getNative(), // if CDevice exposes ID3D11Device*
        hwnd,
        &sd,
        nullptr,
        nullptr,
        sc.GetAddressOf()
    ));

    _swapChain = sc;

    createViews(gfx);
}

void SwapChainTarget::releaseViews()
{
    _dsv.Reset();
    _depth.Reset();
    _rtv.Reset();
    _backBuffer.Reset();
}

void SwapChainTarget::createViews(GraphicsDevice& gfx)
{
    releaseViews();

    ThrowIfFailed(_swapChain->GetBuffer(0, IID_PPV_ARGS(_backBuffer.GetAddressOf())));
    ThrowIfFailed(gfx.device().createRenderTargetView(_backBuffer.Get(), nullptr, _rtv.GetAddressOf())); // add wrapper if missing

    // depth
    D3D11_TEXTURE2D_DESC dd{};
    dd.Width = _desc.width;
    dd.Height = _desc.height;
    dd.MipLevels = 1;
    dd.ArraySize = 1;
    dd.Format = _desc.depthFormat;
    dd.SampleDesc.Count = 1;
    dd.Usage = D3D11_USAGE_DEFAULT;
    dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ThrowIfFailed(gfx.device().createTexture2d(&dd, nullptr, _depth.GetAddressOf()));      // add wrapper if missing
    ThrowIfFailed(gfx.device().createDepthStencilView(_depth.Get(), nullptr, _dsv.GetAddressOf())); // add wrapper if missing

    _vp.TopLeftX = 0.0f;
    _vp.TopLeftY = 0.0f;
    _vp.Width = static_cast<float>(_desc.width);
    _vp.Height = static_cast<float>(_desc.height);
    _vp.MinDepth = 0.0f;
    _vp.MaxDepth = 1.0f;
}

void SwapChainTarget::resize(GraphicsDevice& gfx, uint32_t w, uint32_t h)
{
    if (!_swapChain)
        return;

    if (w == 0 || h == 0)
        return;

    _desc.width = w;
    _desc.height = h;

    releaseViews();

    UINT flags = 0;
    if (gfx.allowTearing())
        flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    ThrowIfFailed(_swapChain->ResizeBuffers(_backBufferCount, w, h, _desc.colorFormat, flags));
    createViews(gfx);
}

void SwapChainTarget::present(GraphicsDevice& gfx, bool vsync)
{
    UINT sync = vsync ? 1 : 0;
    UINT flags = 0;

    if (!vsync && gfx.allowTearing())
        flags |= DXGI_PRESENT_ALLOW_TEARING;

    HRESULT hr = _swapChain->Present(sync, flags);

    // Device removed handling point
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        gfx.handleDeviceLost();
        return;
    }

    ThrowIfFailed(hr);
}

// ---- offscreen ----

void OffscreenTarget::initialize(GraphicsDevice& gfx, const RenderTargetDesc& desc)
{
    _desc = desc;
    createResources(gfx);
}

void OffscreenTarget::releaseResources()
{
    _dsv.Reset();
    _depth.Reset();
    _srv.Reset();
    _rtv.Reset();
    _color.Reset();
}

void OffscreenTarget::createResources(GraphicsDevice& gfx)
{
    releaseResources();

    // color texture (post-process input => SRVïKê{)
    D3D11_TEXTURE2D_DESC cd{};
    cd.Width = _desc.width;
    cd.Height = _desc.height;
    cd.MipLevels = 1;
    cd.ArraySize = 1;
    cd.Format = _desc.colorFormat;
    cd.SampleDesc.Count = _desc.sampleCount;
    cd.Usage = D3D11_USAGE_DEFAULT;
    cd.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    if (_desc.allowUav)
        cd.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

    ThrowIfFailed(gfx.device().createTexture2d(&cd, nullptr, _color.GetAddressOf()));
    ThrowIfFailed(gfx.device().createRenderTargetView(_color.Get(), nullptr, _rtv.GetAddressOf()));
    ThrowIfFailed(gfx.device().createShaderResourceView(_color.Get(), nullptr, _srv.GetAddressOf()));

    // depth
    D3D11_TEXTURE2D_DESC dd{};
    dd.Width = _desc.width;
    dd.Height = _desc.height;
    dd.MipLevels = 1;
    dd.ArraySize = 1;
    dd.Format = _desc.depthFormat;
    dd.SampleDesc.Count = _desc.sampleCount;
    dd.Usage = D3D11_USAGE_DEFAULT;
    dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ThrowIfFailed(gfx.device().createTexture2d(&dd, nullptr, _depth.GetAddressOf()));
    ThrowIfFailed(gfx.device().createDepthStencilView(_depth.Get(), nullptr, _dsv.GetAddressOf()));

    _vp.TopLeftX = 0.0f;
    _vp.TopLeftY = 0.0f;
    _vp.Width = static_cast<float>(_desc.width);
    _vp.Height = static_cast<float>(_desc.height);
    _vp.MinDepth = 0.0f;
    _vp.MaxDepth = 1.0f;
}

void OffscreenTarget::resize(GraphicsDevice& gfx, uint32_t w, uint32_t h)
{
    if (w == 0 || h == 0)
        return;

    _desc.width = w;
    _desc.height = h;
    createResources(gfx);
}
