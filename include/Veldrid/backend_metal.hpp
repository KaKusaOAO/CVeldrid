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

namespace __VD_NAMESPACE {
    
    class MTLFeatureSupport {
    public:
        using Ref = __VD_NAMESPACE::Handle<MTLFeatureSupport>;
        using Iterator = std::set<MTLGPUFamily>::iterator;
        using SizeType = std::set<MTLGPUFamily>::size_type;
        
        // These two methods are for range loop syntactic sugar.
        // DON'T RENAME THEM.
        Iterator begin();
        Iterator end();
        
        MTLFeatureSupport(id<MTLDevice> device);
        Bool IsSupported(MTLGPUFamily family);
        Bool IsDrawBaseVertexInstanceSupported();
        Bool IsMacOS();
        SizeType GetCount();
        MTLGPUFamily GetMaxGPUFamily();
        
    private:
        std::set<MTLGPUFamily> _supportedFeatureSets;
        MTLGPUFamily _maxGPUFamily;
        Mochi::Bool _isMacOS;
    };

    class MTLCommandList : public CommandList {
    public:
        using Ref = __VD_NAMESPACE::Handle<MTLCommandList>;
        
        id<MTLCommandBuffer> GetCommandBuffer();
        id<MTLCommandBuffer> Commit();
        
    private:
        id<MTLCommandBuffer> _cb;
    };

    class MTLFence : public Fence {
    public:
        using Ref = __VD_NAMESPACE::Handle<MTLFence>;
    };

    class MTLGraphicsDevice : public GraphicsDevice {
    public:
        using Ref = __VD_NAMESPACE::Handle<MTLGraphicsDevice>;
        
        MTLGraphicsDevice(GraphicsDeviceOptions options,
                          std::optional<SwapchainDescription> swapchainDesc);
        
        void InitializeComponents();
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
        using Ref = __VD_NAMESPACE::Handle<MTLResourceFactory>;
        
        MTLResourceFactory(MTLGraphicsDevice::Ref device);
        GraphicsBackend GetBackendType() override;
        
        TextureView::Ref CreateTextureViewCore(TextureViewDescription &description) override;
        Pipeline::Ref CreateGraphicsPipelineCore(GraphicsPipelineDescription &description) override;
        
    private:
        MTLGraphicsDevice::Ref _gd;
    };

    class MTLPipeline : public Pipeline {
    public:
        using Ref = __VD_NAMESPACE::Handle<MTLPipeline>;
        
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
