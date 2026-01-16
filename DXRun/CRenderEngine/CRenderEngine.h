#pragma once
#include "CDevice.h"
#include "CDeviceContext.h"

namespace CaramelEngine
{
	// Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
	interface IDeviceNotify
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		~IDeviceNotify() = default;
	};

	class CRenderEngine
	{
	public:

	private:

		void createHardwareAdapter(IDXGIAdapter1** ppAdapter);
		void createFactory();

		std::unique_ptr<CDevice> _device;
		std::unique_ptr<CDeviceContext> _context;
		Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
	};
}