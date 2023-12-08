#pragma once

#if defined(__cplusplus)
#ifndef __VD_GL_IMPL_HEADER_GUARD
#define __VD_GL_IMPL_HEADER_GUARD

#include <Veldrid/config.hpp>

#if !defined(VD_EXCLUDE_D3D11_BACKEND)

#include <Veldrid/vd.hpp>
#include <dxgi.h>
#include <d3d11.h>

namespace vd {

	class D3D11GraphicsDevice : public GraphicsDevice {
	private:
		IDXGIAdapter* _dxgiAdapter;
		ID3D11Device* _device;
		std::string _deviceName;
		std::string _vendorName;
		GraphicsApiVersionRef _apiVersion;
		int _deviceId;
		ID3D11DeviceContext* _immediateContext;
		Mochi::Bool _supportsConcurrentResources;
		Mochi::Bool _supportsCommandLists;

	public:
		D3D11GraphicsDevice(D3D11DeviceOptions options, std::optional<SwapchainDescription> swapchainDesc);
		void InitializeComponents() override;
	};

}

#endif
#endif
#endif