//
//  impl.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#if defined(__cplusplus)
#if !defined(VD_EXCLUDE_METAL_BACKEND)
#ifndef __VD_METAL_IMPL_HEADER_GUARD
#define __VD_METAL_IMPL_HEADER_GUARD

#include <Veldrid/vd.hpp>
#include <iterator>
#include <cstddef>
#include <set>

#if defined(__VD_TARGET_MACOS)
#include <AppKit/AppKit.h>
#else
#include <UIKit/UIKit.h>
#endif // TARGET_OS_MAC

#include <Metal/Metal.h>

namespace vd {

#if defined(__VD_TARGET_MACOS)
class NSWindowSwapchainSource : public SwapchainSource {
private:
    NSWindow *_window;
    
public:
    NSWindowSwapchainSource(NSWindow *window);
    NSWindow* GetNSWindow();
};
#else
class UIViewSwapchainSource : public SwapchainSource {
private:
    UIView *_view;
    
public:
    UIViewSwapchainSource(UIView *view);
    UIView* GetUIView();
};
#endif // TARGET_OS_MAC

class MTLFeatureSupport {
private:
    std::set<MTLGPUFamily> _supportedFeatureSets;
    MTLGPUFamily _maxGPUFamily;
    Mochi::Bool _isMacOS;
    
public:
    using Iterator = std::set<MTLGPUFamily>::iterator;
    using SizeType = std::set<MTLGPUFamily>::size_type;
    Iterator begin();
    Iterator end();
    
    MTLFeatureSupport(id<MTLDevice> device);
    Mochi::Bool IsSupported(MTLGPUFamily family);
    Mochi::Bool IsDrawBaseVertexInstanceSupported();
    Mochi::Bool IsMacOS();
    SizeType GetCount();
    MTLGPUFamily GetMaxGPUFamily();
};

typedef std::shared_ptr<MTLFeatureSupport> MTLFeatureSupportRef;

class MTLGraphicsDevice : public GraphicsDevice {
    using Ref = std::shared_ptr<MTLGraphicsDevice>;
    
private:
    id<MTLDevice> _device;
    NSString *_deviceName;
    GraphicsApiVersionRef _apiVersion;
    
    MTLFeatureSupportRef _metalFeatures;
    GraphicsDeviceFeatures _features;
    ResourceFactoryRef _resourceFactory;
    ResourceBindingModel _resourceBindingModel;
    
public:
    MTLGraphicsDevice(GraphicsDeviceOptions options,
                      std::optional<SwapchainDescription> swapchainDesc);
    void InitializeComponents() override;
    std::string GetDeviceName() override;
    std::string GetVendorName() override;
    GraphicsApiVersionRef GetApiVersion() override;
    GraphicsBackend GetBackendType() override;
    Mochi::Bool IsUvOriginTopLeft() override;
    Mochi::Bool IsDepthRangeZeroToOne() override;
    Mochi::Bool IsClipSpaceYInverted() override;
    GraphicsDeviceFeatures GetFeatures() override;
    ResourceFactoryRef GetResourceFactory() override;
    ResourceBindingModel GetResourceBindingModel();
};

__MC_DEFINE_REF_TYPE(MTLGraphicsDevice)

class MTLResourceFactory : public ResourceFactory {
private:
    MTLGraphicsDeviceRef _gd;
    
public:
    MTLResourceFactory(MTLGraphicsDeviceRef device);
    GraphicsBackend GetBackendType() override;
    
    PipelineRef CreateGraphicsPipelineCore(GraphicsPipelineDescription &description) override;
};

__MC_DEFINE_REF_TYPE(MTLResourceFactory)

class MTLPipeline : public Pipeline {
public:
    MTLPipeline(GraphicsPipelineDescription &description, MTLGraphicsDeviceRef gd);
    std::string GetName() override;
    void SetName(std::string name) override;
    Mochi::Bool IsDisposed() override;
    void Dispose() override;
    Mochi::Bool IsComputePipeline() override;
};

}

#endif /* impl_hpp */
#endif // !defined(VD_EXCLUDE_METAL_BACKEND)
#endif
