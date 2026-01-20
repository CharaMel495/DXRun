#include "pch.h"
#include "GraphicsDevice.h"

using namespace CaramelEngine;
using Microsoft::WRL::ComPtr;
using namespace DX;

namespace
{
#if defined(_DEBUG)
    inline bool sdkLayersAvailable() noexcept
    {
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            nullptr,
            nullptr,
            nullptr
        );
        return SUCCEEDED(hr);
    }
#endif
}

void GraphicsDevice::initialize(const DeviceOptions& opt)
{
    _opt = opt;

    createFactory();
    validateTearingAndHdrSupport();

    _created = createDeviceAndContext();
}

void GraphicsDevice::createFactory()
{
#if defined(_DEBUG) && !defined(__MINGW32__)
    bool debugDxgi = false;
    {
        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
        {
            debugDxgi = true;

            ThrowIfFailed(CreateDXGIFactory2(
                DXGI_CREATE_FACTORY_DEBUG,
                IID_PPV_ARGS(_factory.ReleaseAndGetAddressOf())
            ));

            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

            DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
            {
                80,
            };
            DXGI_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
            filter.DenyList.pIDList = hide;
            dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
        }
    }
    if (!debugDxgi)
#endif
    {
        ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(_factory.ReleaseAndGetAddressOf())));
    }
}

void GraphicsDevice::validateTearingAndHdrSupport()
{
    // tearing
    if (_opt.allowTearing)
    {
        BOOL allowTearing = FALSE;
        ComPtr<IDXGIFactory5> factory5;
        HRESULT hr = _factory.As(&factory5);
        if (SUCCEEDED(hr))
        {
            hr = factory5->CheckFeatureSupport(
                DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                &allowTearing,
                sizeof(allowTearing)
            );
        }

        if (FAILED(hr) || !allowTearing)
        {
            _opt.allowTearing = false;
#ifdef _DEBUG
            OutputDebugStringA("WARNING: Variable refresh rate displays not supported\n");
#endif
        }
    }

    // HDR requires newer swap effects / factory versions in practice
    if (_opt.enableHdr)
    {
        ComPtr<IDXGIFactory5> factory5;
        if (FAILED(_factory.As(&factory5)))
        {
            _opt.enableHdr = false;
#ifdef _DEBUG
            OutputDebugStringA("WARNING: HDR swap chains not supported\n");
#endif
        }
    }

    // flip present requires newer dxgi
    if (_opt.flipPresent)
    {
        ComPtr<IDXGIFactory4> factory4;
        if (FAILED(_factory.As(&factory4)))
        {
            _opt.flipPresent = false;
#ifdef _DEBUG
            OutputDebugStringA("INFO: Flip swap effects not supported\n");
#endif
        }
    }
}

void GraphicsDevice::getHardwareAdapter(IDXGIAdapter1** outAdapter)
{
    *outAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;

    ComPtr<IDXGIFactory6> factory6;
    HRESULT hr = _factory.As(&factory6);
    if (SUCCEEDED(hr))
    {
        for (UINT i = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                i,
                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())));
                ++i)
        {
            DXGI_ADAPTER_DESC1 desc;
            ThrowIfFailed(adapter->GetDesc1(&desc));
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

#ifdef _DEBUG
            wchar_t buff[256] = {};
            swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n",
                i, desc.VendorId, desc.DeviceId, desc.Description);
            OutputDebugStringW(buff);
#endif
            break;
        }
    }

    if (!adapter)
    {
        for (UINT i = 0;
            SUCCEEDED(_factory->EnumAdapters1(i, adapter.ReleaseAndGetAddressOf()));
            ++i)
        {
            DXGI_ADAPTER_DESC1 desc;
            ThrowIfFailed(adapter->GetDesc1(&desc));
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

#ifdef _DEBUG
            wchar_t buff[256] = {};
            swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n",
                i, desc.VendorId, desc.DeviceId, desc.Description);
            OutputDebugStringW(buff);
#endif
            break;
        }
    }

    *outAdapter = adapter.Detach();
}

DeviceCreatedInfo GraphicsDevice::createDeviceAndContext()
{
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    if (_opt.enableDebugLayer && sdkLayersAvailable())
    {
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    }
#endif

    static const D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    UINT featCount = 0;
    for (; featCount < static_cast<UINT>(std::size(featureLevels)); ++featCount)
    {
        if (featureLevels[featCount] < _opt.minFeatureLevel)
            break;
    }
    if (!featCount)
        throw std::out_of_range("minFeatureLevel too high");

    ComPtr<IDXGIAdapter1> adapter;
    getHardwareAdapter(adapter.GetAddressOf());

    ComPtr<ID3D11Device> dev;
    ComPtr<ID3D11DeviceContext> ctx;

    HRESULT hr = E_FAIL;
    D3D_FEATURE_LEVEL createdLevel{};

    if (adapter)
    {
        hr = D3D11CreateDevice(
            adapter.Get(),
            D3D_DRIVER_TYPE_UNKNOWN,
            nullptr,
            creationFlags,
            featureLevels,
            featCount,
            D3D11_SDK_VERSION,
            dev.GetAddressOf(),
            &createdLevel,
            ctx.GetAddressOf());
    }

#if !defined(NDEBUG)
    if (FAILED(hr))
    {
        hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            creationFlags,
            featureLevels,
            featCount,
            D3D11_SDK_VERSION,
            dev.GetAddressOf(),
            &createdLevel,
            ctx.GetAddressOf());

        if (SUCCEEDED(hr))
            OutputDebugStringA("Direct3D Adapter - WARP\n");
    }
#else
    if (FAILED(hr))
        throw std::runtime_error("No Direct3D hardware device found");
#endif

    ThrowIfFailed(hr);

#ifndef NDEBUG
    // debug info queue (optional, but aligns with DeviceResources)
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(dev.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> infoQueue;
        if (SUCCEEDED(d3dDebug.As(&infoQueue)))
        {
#if defined(_DEBUG)
            infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
            filter.DenyList.pIDList = hide;
            infoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // ---- Bridge into your thin wrappers ----
    // If your CDevice expects ID3D11Device1, do:
    // ComPtr<ID3D11Device1> dev1; ThrowIfFailed(dev.As(&dev1)); _device.attachDevice(dev1);
    // Here we assume CDevice can attach ID3D11Device (recommended).
    _device.attachDevice(dev);       // adjust signature if needed
    _context.attachContext(ctx);     // adjust signature if needed

    DeviceCreatedInfo info{};
    info.featureLevel = createdLevel;
    // HDR colorspace etc can be refined later; keep default for now.
    return info;
}

void GraphicsDevice::handleDeviceLost()
{
    if (_notify)
        _notify->onDeviceLost();

    // Release wrappers (they hold ComPtr inside)
    _context.attachContext(nullptr);
    _device.attachDevice(nullptr);

    // Recreate
    createFactory();
    validateTearingAndHdrSupport();
    _created = createDeviceAndContext();

    if (_notify)
        _notify->onDeviceRestored();
}
