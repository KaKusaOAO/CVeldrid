//
//  impl.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#if defined(__cplusplus)

#include <Veldrid/config.hpp>

#if !defined(VD_EXCLUDE_METAL_BACKEND)
#ifndef __VD_METAL_IMPL_HEADER_GUARD
#define __VD_METAL_IMPL_HEADER_GUARD

#include <Veldrid/vd.hpp>
#include <iterator>
#include <cstddef>
#include <set>
#include <map>

#if defined(__VD_TARGET_MACOS)
#include <AppKit/AppKit.h>
#else
#include <UIKit/UIKit.h>
#endif // TARGET_OS_MAC

#include <Metal/Metal.h>

namespace vd {

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

__MC_DEFINE_REF_TYPE(MTLFeatureSupport)

class MTLCommandList : public CommandList {
private:
    id<MTLCommandBuffer> _cb;
    
public:
    id<MTLCommandBuffer> GetCommandBuffer();
    id<MTLCommandBuffer> Commit();
};

__MC_DEFINE_REF_TYPE(MTLCommandList)

class MTLFence : public Fence {
    
};

__MC_DEFINE_REF_TYPE(MTLFence)

class MTLGraphicsDevice : public GraphicsDevice {
    using Ref = std::shared_ptr<MTLGraphicsDevice>;
    
private:
    id<MTLDevice> _device;
    NSString *_deviceName;
    GraphicsApiVersionRef _apiVersion;
    id<MTLCommandQueue> _commandQueue;
    
    std::mutex _submittedCommandsLock;
    std::map<id<MTLCommandBuffer>, MTLFenceRef> _submittedCBs;
    id<MTLCommandBuffer> _latestSubmittedCB;
    
    std::mutex _resetEventsLock;
    
    MTLFeatureSupportRef _metalFeatures;
    GraphicsDeviceFeatures _features;
    ResourceFactoryRef _resourceFactory;
    ResourceBindingModel _resourceBindingModel;
    
    void OnCommandBufferCompleted(id<MTLCommandBuffer> cb);
    
protected:
    void SubmitCommandsCore(CommandListRef commandList, FenceRef fence) override;
    
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
    
    TextureViewRef CreateTextureViewCore(TextureViewDescription &description) override;
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
