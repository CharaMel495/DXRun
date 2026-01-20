#pragma once
#include "CDevice.h"
#include "CDeviceContext.h"

namespace CaramelEngine
{
    struct DeviceOptions
    {
        bool enableDebugLayer = false;
        bool allowTearing = false;
        bool enableHdr = false;
        bool flipPresent = true;
        D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    };

    struct DeviceCreatedInfo
    {
        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
        DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
    };

    class IDeviceNotify
    {
    public:
        virtual void onDeviceLost() = 0;
        virtual void onDeviceRestored() = 0;
    protected:
        ~IDeviceNotify() = default;
    };

    class GraphicsDevice final
    {
    public:
        GraphicsDevice() = default;
        ~GraphicsDevice() = default;

        void initialize(const DeviceOptions& opt);

        void registerNotify(IDeviceNotify* notify) noexcept { _notify = notify; }

        IDXGIFactory2* factory() const noexcept { return _factory.Get(); }

        CDevice& device() noexcept { return _device; }
        CDeviceContext& context() noexcept { return _context; }

        bool allowTearing() const noexcept { return _opt.allowTearing; }
        bool enableHdr() const noexcept { return _opt.enableHdr; }
        bool flipPresent() const noexcept { return _opt.flipPresent; }

        D3D_FEATURE_LEVEL featureLevel() const noexcept { return _created.featureLevel; }
        DXGI_COLOR_SPACE_TYPE colorSpace() const noexcept { return _created.colorSpace; }

        // device lost entry point
        void handleDeviceLost();

    private:
        void createFactory();
        void getHardwareAdapter(IDXGIAdapter1** outAdapter);

        void validateTearingAndHdrSupport();
        DeviceCreatedInfo createDeviceAndContext();

    private:
        DeviceOptions _opt{};
        DeviceCreatedInfo _created{};

        Microsoft::WRL::ComPtr<IDXGIFactory2> _factory;

        // thin wrappers you already built
        CDevice _device;
        CDeviceContext _context;

        IDeviceNotify* _notify = nullptr;
    };
}
