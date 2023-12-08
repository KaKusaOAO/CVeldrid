#include <Veldrid/backend_d3d11.hpp>

#if !defined(VD_EXCLUDE_D3D11_BACKEND)

#include <iomanip>

namespace vd {

	void CheckResult(HRESULT result, std::source_location loc = std::source_location::current()) {
		if (result < 0) {
			std::stringstream str;
			str << "HRESULT code " << result << " doesn't indicate success. (at ";
			str << loc.file_name() << ":" << loc.line() << ")";

			throw VeldridException(str.str());
		}
	}

	Mochi::Bool SdkLayersAvailable() {
		return D3D11CreateDevice(0, D3D_DRIVER_TYPE_NULL, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, 0, 0, 0) >= 0;
	}

	D3D11GraphicsDevice::D3D11GraphicsDevice(D3D11DeviceOptions options, std::optional<SwapchainDescription> swapchainDesc) {
		auto flags = options.DeviceCreationFlags;

#if _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

		if ((flags & D3D11_CREATE_DEVICE_DEBUG) && !SdkLayersAvailable()) {
			flags &= ~D3D11_CREATE_DEVICE_DEBUG;
		}

		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		D3D_FEATURE_LEVEL level;

		try {
			CheckResult(D3D11CreateDevice((IDXGIAdapter*)options.AdapterPtr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				D3D11_CREATE_DEVICE_DEBUG,
				levels,
				sizeof(levels) / sizeof(D3D_FEATURE_LEVEL),
				D3D11_SDK_VERSION,
				&_device,
				&level,
				0));
		}
		catch (VeldridException&) {
			CheckResult(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, 0, 0, D3D11_SDK_VERSION, &_device, &level, 0));
		}

		IDXGIDevice* dxgiDevice = dynamic_cast<IDXGIDevice*>(_device);
		CheckResult(dxgiDevice->GetAdapter(&_dxgiAdapter));

		DXGI_ADAPTER_DESC desc;
		CheckResult(_dxgiAdapter->GetDesc(&desc));

		std::wstring deviceName(desc.Description);
		_deviceName = std::string(deviceName.begin(), deviceName.end());
		
		std::stringstream vendorName;
		vendorName << "id:" << std::setw(8) << std::setfill('0') << std::hex << desc.VendorId;
		_vendorName = vendorName.str();

		_deviceId = desc.DeviceId;

		switch (_device->GetFeatureLevel()) {
		case D3D_FEATURE_LEVEL_10_0:
			_apiVersion = std::make_shared<GraphicsApiVersion>(10, 0, 0, 0);
			break;
		case D3D_FEATURE_LEVEL_10_1:
			_apiVersion = std::make_shared<GraphicsApiVersion>(10, 1, 0, 0);
			break;
		case D3D_FEATURE_LEVEL_11_0:
			_apiVersion = std::make_shared<GraphicsApiVersion>(11, 0, 0, 0);
			break;
		case D3D_FEATURE_LEVEL_11_1:
			_apiVersion = std::make_shared<GraphicsApiVersion>(11, 1, 0, 0);
			break;
		case D3D_FEATURE_LEVEL_12_0:
			_apiVersion = std::make_shared<GraphicsApiVersion>(12, 0, 0, 0);
			break;
		case D3D_FEATURE_LEVEL_12_1:
			_apiVersion = std::make_shared<GraphicsApiVersion>(12, 1, 0, 0);
			break;
		case D3D_FEATURE_LEVEL_12_2:
			_apiVersion = std::make_shared<GraphicsApiVersion>(12, 2, 0, 0);
			break;
		}

		if (swapchainDesc.has_value()) {
			auto desc = swapchainDesc.value();
			// TODO: Set main swapchain
		}

		_device->GetImmediateContext(&_immediateContext);
		
		{
			D3D11_FEATURE_DATA_THREADING support = {};
			HRESULT result = _device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &support, sizeof(support));

			if (result < 0) {
				_supportsConcurrentResources = false;
				_supportsCommandLists = false;
			}
			else {
				_supportsConcurrentResources = support.DriverConcurrentCreates != 0;
				_supportsCommandLists = support.DriverCommandLists != 0;
			}
		}
	}

	void D3D11GraphicsDevice::InitializeComponents() {
		// TODO: Resource factory & backend info

		PostDeviceCreated();
	}
}

#endif