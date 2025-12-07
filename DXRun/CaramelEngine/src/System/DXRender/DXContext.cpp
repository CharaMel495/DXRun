#include "pch.h"
#include "DXInclude.h"
#include "DXContext.h"

bool DXContext::initialize(HWND hwnd) 
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        nullptr, 0, D3D11_SDK_VERSION,
        &scd, &swapChain, &device, &featureLevel, &context);

    return SUCCEEDED(hr);
}

void DXContext::shutdown() 
{
    swapChain.Reset();
    context.Reset();
    device.Reset();
}

ID3D11Device* DXContext::getDevice() 
{
    return device.Get();
}

ID3D11DeviceContext* DXContext::getContext() 
{
    return context.Get();
}