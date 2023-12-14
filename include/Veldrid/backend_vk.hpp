#pragma once

#if defined(__cplusplus)

#ifndef __VD_D3D11_IMPL_HEADER_GUARD
#define __VD_D3D11_IMPL_HEADER_GUARD

#include <Veldrid/config.hpp>

#if !defined(VD_EXCLUDE_VK_BACKEND)

#include <Veldrid/vd.hpp>


#define VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_METAL_EXT
#define VK_USE_PLATFORM_MACOS_MVK
#define VK_USE_PLATFORM_IOS_MVK
#define VK_USE_PLATFORM_ANDROID_KHR

#include <vulkan/vulkan.hpp>


// #if defined (_WIN32)
#	include <vulkan/vulkan_win32.h>
// #elif defined(__APPLE__)
#	include <vulkan/vulkan_metal.h>
#	include <vulkan/vulkan_macos.h>
#	include <vulkan/vulkan_ios.h>
// #elif defined(__linux__)
#	include <vulkan/vulkan_xcb.h>
// #elif defined(__ANDROID__)
#	include <vulkan/vulkan_android.h>
// #endif
#include <vulkan/vulkan_xlib.h>
#include <vulkan/vulkan_wayland.h>

#include <vector>
#include <list>

namespace vd {

	class VkGraphicsDevice : public GraphicsDevice {
	public:
		using Ref = __MC_REF_TYPE(VkGraphicsDevice);

		VkGraphicsDevice(GraphicsDeviceOptions options,
						 std::optional<SwapchainDescription> scDesc);
		VkGraphicsDevice(GraphicsDeviceOptions options,
			             std::optional<SwapchainDescription> scDesc,
			             VulkanDeviceOptions vkOptions);

		Mochi::Bool HasSurfaceExtension(std::string extension);
		void EnableDebugCallback(vk::DebugReportFlagsEXT flags = vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::eError);

	private:
		static const char* s_name;

		vk::Instance _instance;
		vk::PhysicalDevice _physicalDevice;
		std::string _deviceName;
		std::string _vendorName;
		GraphicsApiVersion::Ref _apiVersion;
		std::string _driverName;

		std::vector<std::string> _surfaceExtensions;

		Mochi::Bool _standardValidationSupported;
		Mochi::Bool _khronosValidationSupported;
		vk::DebugReportCallbackEXT _debugCallbackHandle;

		Mochi::Bool _supportsGetBufferMemoryRequirements2;
		Mochi::Bool _supportsGetImageMemoryRequirements2;
		Mochi::Bool _supportsGetPhysicalDeviceProperties2;
		Mochi::Bool _supportsCreateMetalSurfaceEXT;

		Mochi::Bool _useKHR_getPhysicalDeviceProperties2;

		void CreateInstance(Mochi::Bool debug, VulkanDeviceOptions options);

		template <class T = void*>
		T GetInstanceProcAddr(std::string name) {
			return (T)_instance.getProcAddr(name);
		}

		static VkBool32 DebugCallback(
			VkDebugReportFlagsEXT                       flags,
			VkDebugReportObjectTypeEXT                  objectType,
			uint64_t                                    object,
			size_t                                      location,
			int32_t                                     messageCode,
			const char* pLayerPrefix,
			const char* pMessage,
			void* pUserData);
	};

};

#endif // !defined(VD_EXCLUDE_VK_BACKEND)
#endif // __VD_D3D11_IMPL_HEADER_GUARD
#endif // defined(__cplusplus)