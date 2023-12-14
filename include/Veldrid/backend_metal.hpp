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
public:
    using Ref = vd::Ref<MTLFeatureSupport>;
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
    
private:
    std::set<MTLGPUFamily> _supportedFeatureSets;
    MTLGPUFamily _maxGPUFamily;
    Mochi::Bool _isMacOS;
};

class MTLCommandList : public CommandList {
public:
    using Ref = vd::Ref<MTLCommandList>;
    
    id<MTLCommandBuffer> GetCommandBuffer();
    id<MTLCommandBuffer> Commit();
    
private:
    id<MTLCommandBuffer> _cb;
};

class MTLFence : public Fence {
public:
    using Ref = vd::Ref<MTLFence>;
};

class MTLGraphicsDevice : public GraphicsDevice {
public:
    using Ref = vd::Ref<MTLGraphicsDevice>;
    
    MTLGraphicsDevice(GraphicsDeviceOptions options,
                      std::optional<SwapchainDescription> swapchainDesc);
    
    void InitializeComponents() override;
    std::string GetDeviceName() override;
    std::string GetVendorName() override;
    GraphicsApiVersion::Ref GetApiVersion() override;
    GraphicsBackend GetBackendType() override;
    Mochi::Bool IsUvOriginTopLeft() override;
    Mochi::Bool IsDepthRangeZeroToOne() override;
    Mochi::Bool IsClipSpaceYInverted() override;
    GraphicsDeviceFeatures GetFeatures() override;
    ResourceFactory::Ref GetResourceFactory() override;
    ResourceBindingModel GetResourceBindingModel();
    
private:
    id<MTLDevice> _device;
    NSString *_deviceName;
    GraphicsApiVersion::Ref _apiVersion;
    id<MTLCommandQueue> _commandQueue;
    
    std::mutex _submittedCommandsLock;
    std::map<id<MTLCommandBuffer>, MTLFence::Ref> _submittedCBs;
    id<MTLCommandBuffer> _latestSubmittedCB;
    
    std::mutex _resetEventsLock;
    
    MTLFeatureSupport::Ref _metalFeatures;
    GraphicsDeviceFeatures _features;
    ResourceFactory::Ref _resourceFactory;
    ResourceBindingModel _resourceBindingModel;
    
    void OnCommandBufferCompleted(id<MTLCommandBuffer> cb);
    
protected:
    void SubmitCommandsCore(CommandList::Ref commandList, Fence::Ref fence) override;
};

class MTLResourceFactory : public ResourceFactory {
public:
    using Ref = vd::Ref<MTLResourceFactory>;
    
    MTLResourceFactory(MTLGraphicsDevice::Ref device);
    GraphicsBackend GetBackendType() override;
    
    TextureView::Ref CreateTextureViewCore(TextureViewDescription &description) override;
    Pipeline::Ref CreateGraphicsPipelineCore(GraphicsPipelineDescription &description) override;
    
private:
    MTLGraphicsDevice::Ref _gd;
};

class MTLPipeline : public Pipeline {
public:
    using Ref = vd::Ref<MTLPipeline>;
    
    MTLPipeline(GraphicsPipelineDescription &description, MTLGraphicsDevice::Ref gd);
    
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
