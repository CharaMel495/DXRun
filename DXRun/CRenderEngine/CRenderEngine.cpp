#include "pch.h"
#include "CRenderEngine.h"


//void CDevice::createFactory()
//{
//#if defined(_DEBUG) && !defined(__MINGW32__)
//    bool debugDXGI = false;
//    {
//        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
//        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
//        {
//            debugDXGI = true;
//
//            ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf())));
//
//            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
//            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
//
//            DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
//            {
//                80 /* IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides. */,
//            };
//            DXGI_INFO_QUEUE_FILTER filter = {};
//            filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
//            filter.DenyList.pIDList = hide;
//            dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
//        }
//    }
//
//    if (!debugDXGI)
//#endif
//
//        ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf())));
//}

//void CDevice::createHardwareAdapter(IDXGIAdapter1** ppAdapter)
//{
//    *ppAdapter = nullptr;
//
//    ComPtr<IDXGIAdapter1> adapter;
//
//    ComPtr<IDXGIFactory6> factory6;
//    HRESULT hr = _dxgiFactory.As(&factory6);
//    if (SUCCEEDED(hr))
//    {
//        for (UINT adapterIndex = 0;
//            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
//                adapterIndex,
//                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
//                IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())));
//                adapterIndex++)
//        {
//            DXGI_ADAPTER_DESC1 desc;
//            ThrowIfFailed(adapter->GetDesc1(&desc));
//
//            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
//            {
//                // Don't select the Basic Render Driver adapter.
//                continue;
//            }
//
//#ifdef _DEBUG
//            wchar_t buff[256] = {};
//            swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
//            OutputDebugStringW(buff);
//#endif
//
//            break;
//        }
//    }
//
//    if (!adapter)
//    {
//        for (UINT adapterIndex = 0;
//            SUCCEEDED(_dxgiFactory->EnumAdapters1(
//                adapterIndex,
//                adapter.ReleaseAndGetAddressOf()));
//                adapterIndex++)
//        {
//            DXGI_ADAPTER_DESC1 desc;
//            ThrowIfFailed(adapter->GetDesc1(&desc));
//
//            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
//            {
//                // Don't select the Basic Render Driver adapter.
//                continue;
//            }
//
//#ifdef _DEBUG
//            wchar_t buff[256] = {};
//            swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
//            OutputDebugStringW(buff);
//#endif
//
//            break;
//        }
//    }
//
//    *ppAdapter = adapter.Detach();
//}