#include <Veldrid/backend_vk.hpp>

#if !defined(VD_EXCLUDE_VK_BACKEND)

#include <Mochi/logging.hpp>
#include <set>
#include <list>
#include <iomanip>

namespace vd {

    namespace CommonStrings {

        using CString = const char[];

        constexpr CString main = "main";
        constexpr CString VK_KHR_protability_subset				= "VK_KHR_protability_subset";

        constexpr CString StandardValidationLayerName           = "VK_LAYER_LUNARG_standard_validation";
        constexpr CString KhronosValidationLayerName            = "VK_LAYER_KNRONOS_validation";

    };

    namespace VkUtils {
        Bool TryLoadVulkan();
        Lazy<Bool> s_isVulkanLoaded(TryLoadVulkan);

        template <class T>
        using EnumerateResult = std::set<T>;

        EnumerateResult<std::string> EnumerateInstanceExtensions();
        Lazy<EnumerateResult<std::string>> s_instanceExtensions(EnumerateInstanceExtensions);
    
        EnumerateResult<std::string> EnumerateInstanceLayers() {
            auto props = vk::enumerateInstanceLayerProperties();
            EnumerateResult<std::string> result;

            for (auto& prop : props) {
                result.insert(prop.layerName);
            }

            return result;
        }

        Bool IsVulkanLoaded() {
            return s_isVulkanLoaded.GetValue();
        }

        Bool TryLoadVulkan() {
            try {
                vk::enumerateInstanceLayerProperties();
                return true;
            }
            catch (std::exception&) {
                return false;
            }
        }

        EnumerateResult<std::string> GetInstanceExtensions() {
            return s_instanceExtensions.GetValue();
        }

        EnumerateResult<std::string> EnumerateInstanceExtensions() {
            EnumerateResult<std::string> result;

            if (!IsVulkanLoaded()) {
                return result;
            }

            auto props = vk::enumerateInstanceExtensionProperties();
            for (auto& prop : props) {
                result.insert(prop.extensionName);
            }

            return result;
        };

        void TransitionImageLayout(
            vk::CommandBuffer cb,
            vk::Image image,
            UInt32 baseMipLevel,
            UInt32 levelCount,
            UInt32 baseArrayLayer,
            UInt32 layerCount,
            vk::ImageAspectFlags aspectMask,
            vk::ImageLayout oldLayout,
            vk::ImageLayout newLayout) {

            vk::ImageMemoryBarrier barrier = {};
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;

            vk::ImageSubresourceRange& range = barrier.subresourceRange;
            range.aspectMask = aspectMask;
            range.baseMipLevel = baseMipLevel;
            range.levelCount = levelCount;
            range.baseArrayLayer = baseArrayLayer;
            range.layerCount = layerCount;

            vk::PipelineStageFlags srcStageFlags;
            vk::PipelineStageFlags dstStageFlags;

            if ((oldLayout == vk::ImageLayout::eUndefined || oldLayout == vk::ImageLayout::ePreinitialized) && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eNone;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eShaderReadOnlyOptimal && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
                srcStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eShaderReadOnlyOptimal && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                srcStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eNone;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eGeneral) {
                barrier.srcAccessMask = vk::AccessFlagBits::eNone;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe;
                dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (oldLayout == vk::ImageLayout::eGeneral && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (oldLayout == vk::ImageLayout::eShaderReadOnlyOptimal && newLayout == vk::ImageLayout::eGeneral) {
                barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
                dstStageFlags = vk::PipelineStageFlagBits::eComputeShader;
            }
            else if (oldLayout == vk::ImageLayout::eTransferSrcOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (oldLayout == vk::ImageLayout::eTransferSrcOptimal && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eColorAttachmentOptimal && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
                srcStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eColorAttachmentOptimal && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                srcStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eColorAttachmentOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (oldLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
                srcStageFlags = vk::PipelineStageFlagBits::eLateFragmentTests;
                dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else if (oldLayout == vk::ImageLayout::eColorAttachmentOptimal && newLayout == vk::ImageLayout::ePresentSrcKHR) {
                barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;
                srcStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dstStageFlags = vk::PipelineStageFlagBits::eBottomOfPipe;
            }
            else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::ePresentSrcKHR) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eBottomOfPipe;
            }
            else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eColorAttachmentOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
                srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
                dstStageFlags = vk::PipelineStageFlagBits::eLateFragmentTests;
            }
            else if (oldLayout == vk::ImageLayout::eGeneral && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eShaderWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
                srcStageFlags = vk::PipelineStageFlagBits::eComputeShader;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::eGeneral && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eShaderWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                srcStageFlags = vk::PipelineStageFlagBits::eComputeShader;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (oldLayout == vk::ImageLayout::ePresentSrcKHR && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eMemoryRead;
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
                srcStageFlags = vk::PipelineStageFlagBits::eBottomOfPipe;
                dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
            }
            else {
                throw VeldridException("Invalid image layout transition.");
            }

            cb.pipelineBarrier(srcStageFlags, dstStageFlags, 
                vk::DependencyFlags(), 
                0, 0,         // vk::ArrayProxy<vk::MemoryBarrier>(), 
                0, 0,         // vk::ArrayProxy<vk::BufferMemoryBarrier>(), 
                1, &barrier); // vk::ArrayProxy<vk::ImageMemoryBarrier>(barrier));
        }
    };

    class VkSurfaceUtil {
    private:
        [[noreturn]] static inline void ThrowExtensionNotAvailable(std::string extension) {
            throw VeldridException("The required instance extension was not available: " + extension);
        }
    
    public:
#if defined(_WIN32)
        static vk::SurfaceKHR CreateWin32(vk::Instance instance, Win32SwapchainSource::Ref win32Source) {
            vk::Win32SurfaceCreateInfoKHR surfaceCI = {};
            surfaceCI.hwnd = (HWND) win32Source->GetHwnd();
            surfaceCI.hinstance = (HINSTANCE)win32Source->GetHInstance();
            return instance.createWin32SurfaceKHR(surfaceCI);
        }
#endif // defined(_WIN32)

        static vk::SurfaceKHR CreateSurface(VkGraphicsDevice::Ref gd, vk::Instance instance, SwapchainSource::Ref swapchainSource) {
            auto doCheck = gd != nullptr;

#if defined(_WIN32) 
            {
                Win32SwapchainSource::Ref win32Source;
                if (TryCastRef(swapchainSource, win32Source)) {
                    if (doCheck && !gd->HasSurfaceExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)) {
                        ThrowExtensionNotAvailable(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
                    }
                    return CreateWin32(instance, win32Source);
                }
            }
#endif // defined(_WIN32)

            throw VeldridException("The provided SwapchainSource::Ref cannot be used to create a Vulkan surface.");
        }

    };

    const char* VkGraphicsDevice::s_name = "Veldrid-VkGraphicsDevice";
    
    VkGraphicsDevice::VkGraphicsDevice(GraphicsDeviceOptions options,
                                       std::optional<SwapchainDescription> scDesc) 
        : VkGraphicsDevice(options, scDesc, {}) {}

    VkGraphicsDevice::VkGraphicsDevice(GraphicsDeviceOptions options,
                                       std::optional<SwapchainDescription> scDesc,
                                       VulkanDeviceOptions vkOptions) {}

    void VkGraphicsDevice::InitializeComponents(GraphicsDeviceOptions options,
                                                std::optional<SwapchainDescription> scDesc,
                                                VulkanDeviceOptions vkOptions) {
        VkGraphicsDevice::Ref self = vd::AssertSubType<VkGraphicsDevice>(GetRef<GraphicsDevice>(this));
        CreateInstance(options.Debug, vkOptions);
        
        vk::SurfaceKHR surface;
        
        if (scDesc.has_value()) {
            surface = VkSurfaceUtil::CreateSurface(self, _instance, scDesc.value().Source);
        }
        
        
    }

    void VkGraphicsDevice::CreateInstance(Bool debug, VulkanDeviceOptions options) {
        auto availableInstanceLayers = VkUtils::EnumerateInstanceLayers();
        auto availableInstanceExtensions = VkUtils::GetInstanceExtensions();

        vk::ApplicationInfo applicationInfo = {};
        applicationInfo.apiVersion = VK_API_VERSION_1_0;
        applicationInfo.applicationVersion = VK_API_VERSION_1_0;
        applicationInfo.engineVersion = VK_API_VERSION_1_0;
        applicationInfo.pApplicationName = s_name;
        applicationInfo.pEngineName = s_name;

        vk::InstanceCreateInfo instanceCI = {};
        instanceCI.pApplicationInfo = &applicationInfo;

        std::vector<const char*> instanceExtensions;
        std::vector<const char*> instanceLayers;

        if (availableInstanceExtensions.contains(CommonStrings::VK_KHR_protability_subset)) {
            _surfaceExtensions.push_back(CommonStrings::VK_KHR_protability_subset);
        }

        if (availableInstanceExtensions.contains(VK_KHR_SURFACE_EXTENSION_NAME)) {
            _surfaceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        }

        #if defined(VK_USE_PLATFORM_WIN32_KHR)

        if (availableInstanceExtensions.contains(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)) {
            _surfaceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        }

        #endif // defined(VK_USE_PLATFORM_WIN32_KHR)

        #if defined(VK_USE_PLATFORM_ANDROID_KHR)

        if (availableInstanceExtensions.contains(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME)) {
            _surfaceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
        }

#if defined(VULKAN_XLIB_H_)
        if (availableInstanceExtensions.contains(VK_KHR_XLIB_SURFACE_EXTENSION_NAME)) {
            _surfaceExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
        }
#endif // defined(VULKAN_XLIB_H_)

#if defined(VULKAN_WAYLAND_H_)
        if (availableInstanceExtensions.contains(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME)) {
            _surfaceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
        }
#endif // defined(VULKAN_WAYLAND_H_)

        #endif // defined(VK_USE_PLATFORM_ANDROID_KHR)

        #if defined(VK_USE_PLATFORM_METAL_EXT)

        if (availableInstanceExtensions.contains(VK_EXT_METAL_SURFACE_EXTENSION_NAME)) {
            _surfaceExtensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
        }
        else {
            // Legacy MoltenVK extensions

#if defined(VK_USE_PLATFORM_MACOS_MVK)
            if (availableInstanceExtensions.contains(VK_MVK_MACOS_SURFACE_EXTENSION_NAME)) {
                _surfaceExtensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
            }
#endif // defined(VK_USE_PLATFORM_MACOS_MVK)

#if defined(VK_USE_PLATFORM_IOS_MVK)
            if (availableInstanceExtensions.contains(VK_MVK_IOS_SURFACE_EXTENSION_NAME)) {
                _surfaceExtensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
            }
#endif // defined(VK_USE_PLATFORM_IOS_MVK)

        }

        #endif // defined(VK_USE_PLATFORM_METAL_EXT)

        for (auto& ext : _surfaceExtensions) {
            instanceExtensions.push_back(ext.c_str());
        }

        bool hasDeviceProperties2 = availableInstanceExtensions.contains("VK_KHR_get_physical_device_properties2");
        if (hasDeviceProperties2) {
            instanceExtensions.push_back("VK_KHR_get_physical_device_properties2");
        }

        auto requestedInstanceExtensions = options.InstanceExtensions;
        for (auto& requiredExt : requestedInstanceExtensions) {
            if (!availableInstanceExtensions.contains(requiredExt)) {
                throw VeldridException("The required instance extension was not available: " + requiredExt);
            }
        }

        bool debugReportExtensionAvailable = false;
        if (debug) {
            if (availableInstanceExtensions.contains(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
                debugReportExtensionAvailable = true;
                instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            }

            if (availableInstanceExtensions.contains(CommonStrings::StandardValidationLayerName)) {
                _standardValidationSupported = true;
                instanceLayers.push_back(CommonStrings::StandardValidationLayerName);
            }

            if (availableInstanceExtensions.contains(CommonStrings::KhronosValidationLayerName)) {
                _khronosValidationSupported = true;
                instanceLayers.push_back(CommonStrings::KhronosValidationLayerName);
            }
        }

        instanceCI.enabledExtensionCount = instanceExtensions.size();
        instanceCI.ppEnabledExtensionNames = instanceExtensions.data();

        instanceCI.enabledLayerCount = instanceLayers.size();
        if (instanceLayers.size() > 0) {
            instanceCI.ppEnabledLayerNames = instanceLayers.data();
        }

        _instance = vk::createInstance(instanceCI);

        if (debug && debugReportExtensionAvailable) {
            EnableDebugCallback();
        }

        if (hasDeviceProperties2) {
            _supportsGetPhysicalDeviceProperties2 = true;

            if (!GetInstanceProcAddr("vkGetPhysicalDeviceProperties2")) {
                if (GetInstanceProcAddr("vkGetPhysicalDeviceProperties2KHR")) {
                    _useKHR_getPhysicalDeviceProperties2 = true;
                }
                else {
                    _supportsGetPhysicalDeviceProperties2 = false;
                }
            }
        }
    }

    void VkGraphicsDevice::CreatePhysicalDevice() {
        auto physicalDevices = _instance.enumeratePhysicalDevices();
        
        // Just use the first one.
        _physicalDevice = physicalDevices[0];
        
        _physicalDeviceProperties = _physicalDevice.getProperties();
        _deviceName = _physicalDeviceProperties.deviceName.data();
        
        std::stringstream vendorName;
        vendorName << "id:";
        vendorName << std::hex << std::setw(8) << std::setfill('0') << _physicalDeviceProperties.vendorID;
        _vendorName = vendorName.str();
        
        // _apiVersion =
        
        std::stringstream driverInfo;
        driverInfo << "version:";
        driverInfo << std::hex << std::setw(8) << std::setfill('0') << _physicalDeviceProperties.driverVersion;
        _driverInfo = driverInfo.str();
        
        _physicalDeviceFeatures = _physicalDevice.getFeatures();
        _physicalDeviceMemoryProperties = _physicalDevice.getMemoryProperties();
    }

    void VkGraphicsDevice::CreateLogicalDevice(vk::SurfaceKHR surface, Bool preferStandardClipY, VulkanDeviceOptions options) {
        GetQueueFamilyIndices(surface);
    }

    void VkGraphicsDevice::GetQueueFamilyIndices(vk::SurfaceKHR surface) {
        auto qfp = _physicalDevice.getQueueFamilyProperties();

        bool foundGraphics = false;
        bool foundPresent = !surface;

        int index = 0;
        for (auto& prop : qfp) {
            if (prop.queueFlags & vk::QueueFlagBits::eGraphics) {
                _graphicsQueueIndex = index;
                foundGraphics = true;
            }

            if (!foundPresent) {
                if (_physicalDevice.getSurfaceSupportKHR(index, surface)) {
                    _presentQueueIndex = index;
                    foundPresent = true;
                }
            }

            if (foundGraphics && foundPresent) return;

            index++;
        }
    }

    Bool VkGraphicsDevice::HasSurfaceExtension(std::string extension) {
        return std::find(_surfaceExtensions.begin(), _surfaceExtensions.end(), extension) != _surfaceExtensions.end();
    }

    VkBool32 VkGraphicsDevice::DebugCallback(
        VkDebugReportFlagsEXT                       flags,
        VkDebugReportObjectTypeEXT                  objectType,
        uint64_t                                    object,
        size_t                                      location,
        int32_t                                     messageCode,
        const char* pLayerPrefix,
        const char* pMessage,
        void* pUserData) {

        std::stringstream fullMessage;
        fullMessage << "[" << flags << "] " << objectType << " " << pMessage;

        if (flags == VK_DEBUG_REPORT_ERROR_BIT_EXT) {
            throw VeldridException("A Vulkan validation error was encountered: " + fullMessage.str());
        }

        Logger::Info(fullMessage.str(), "VkGraphicsDevice");
        return VK_FALSE;
    }

    void VkGraphicsDevice::EnableDebugCallback(vk::DebugReportFlagsEXT flags) {
        Logger::Info("Enabling Vulkan Debug callbacks.");
        vk::DebugReportCallbackCreateInfoEXT debugCallbackCI = {};
        debugCallbackCI.flags = flags;
        debugCallbackCI.pfnCallback = DebugCallback;

        _debugCallbackHandle = _instance.createDebugReportCallbackEXT(debugCallbackCI);
    }
};

#endif // !defined(VD_EXCLUDE_VK_BACKEND)
