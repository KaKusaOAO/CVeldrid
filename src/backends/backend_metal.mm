//
//  impl.cpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#if !defined(VD_EXCLUDE_METAL_BACKEND)
#include <Veldrid/backend_metal.hpp>
#include <set>

#pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Wdeprecated-declarations"

namespace __VD_NAMESPACE {
    
    // MARK: - MTLFeatureSupport
    
    MTLGPUFamily MTLGPUFamilies[] = {
        MTLGPUFamilyCommon1,
        MTLGPUFamilyCommon2,
        MTLGPUFamilyCommon3,
        MTLGPUFamilyMetal3,
        
        MTLGPUFamilyApple1,
        MTLGPUFamilyApple2,
        MTLGPUFamilyApple3,
        MTLGPUFamilyApple4,
        MTLGPUFamilyApple5,
        MTLGPUFamilyApple6,
        MTLGPUFamilyApple7,
        MTLGPUFamilyApple8,
        MTLGPUFamilyApple9,
        MTLGPUFamilyMacCatalyst1,
        MTLGPUFamilyMacCatalyst2,
        MTLGPUFamilyMac1,
        MTLGPUFamilyMac2,
    };
    
    // Forwards to the underlying std::set
    MTLFeatureSupport::Iterator MTLFeatureSupport::begin() { return _supportedFeatureSets.begin(); }
    MTLFeatureSupport::Iterator MTLFeatureSupport::end()   { return _supportedFeatureSets.end();   }
    
    Bool  MTLFeatureSupport::IsMacOS()                { return _isMacOS; }
    MTLGPUFamily MTLFeatureSupport::GetMaxGPUFamily() { return _maxGPUFamily; }
    
    MTLFeatureSupport::SizeType MTLFeatureSupport::GetCount() {
        return _supportedFeatureSets.size();
    }
    
    Mochi::Bool MTLFeatureSupport::IsSupported(MTLGPUFamily family) {
        return _supportedFeatureSets.contains(family);
    }
    
    MTLFeatureSupport::MTLFeatureSupport(id<MTLDevice> device) {
        int count = sizeof(MTLGPUFamilies) / sizeof(MTLGPUFamily);
        for (int i = 0; i < count; i++) {
            MTLGPUFamily family = MTLGPUFamilies[i];
            if ([device supportsFamily:family]) {
                _supportedFeatureSets.insert(family);
                _maxGPUFamily = family;
            }
        }
        
        _isMacOS = (IsSupported(MTLGPUFamilyMac1) ||
                    IsSupported(MTLGPUFamilyMac2) ||
                    IsSupported(MTLGPUFamilyMacCatalyst1) ||
                    IsSupported(MTLGPUFamilyMacCatalyst2));
    }
    
    Mochi::Bool MTLFeatureSupport::IsDrawBaseVertexInstanceSupported() {
        return (IsSupported(MTLGPUFamilyApple2) ||
                IsSupported(MTLGPUFamilyApple3) ||
                IsSupported(MTLGPUFamilyApple4) ||
                _isMacOS);
    }
    
    // MARK: -
    
    id<MTLCommandBuffer> MTLCommandList::GetCommandBuffer() { return _cb; }
    
    id<MTLCommandBuffer> MTLCommandList::Commit() {
        [_cb commit];
        
        id<MTLCommandBuffer> ret = _cb;
        _cb = 0;
        return ret;
    }
    
    // MARK: -
    
    MTLGraphicsDevice::MTLGraphicsDevice(GraphicsDeviceOptions options,
                                         std::optional<SwapchainDescription> swapchainDesc) {
        _device = MTLCreateSystemDefaultDevice();
        _deviceName = [_device name];
        _metalFeatures = Mochi::CreateRef<MTLFeatureSupport>(_device);
        
        int major = (int) _metalFeatures->GetMaxGPUFamily() / 10000;
        int minor = (int) _metalFeatures->GetMaxGPUFamily() % 10000;
        _apiVersion = Mochi::CreateRef<GraphicsApiVersion>(major, minor, 0, 0);
        
        _features = (GraphicsDeviceFeatures() |
                     GraphicsDeviceFeaturesBits::ComputeShader |
                     GraphicsDeviceFeaturesBits::DrawIndirect |
                     GraphicsDeviceFeaturesBits::DrawIndirectBaseInstance |
                     GraphicsDeviceFeaturesBits::FillModeWireframe |
                     GraphicsDeviceFeaturesBits::SamplerAnisotropy |
                     GraphicsDeviceFeaturesBits::DepthClipDisable |
                     GraphicsDeviceFeaturesBits::Texture1D |
                     GraphicsDeviceFeaturesBits::IndependentBlend |
                     GraphicsDeviceFeaturesBits::StructuredBuffer |
                     GraphicsDeviceFeaturesBits::SubsetTextureView |
                     GraphicsDeviceFeaturesBits::CommandListDebugMarkers |
                     GraphicsDeviceFeaturesBits::BufferRangeBinding);
        
        if (_metalFeatures->IsSupported(MTLGPUFamilyMac1)) {
            _features = (_features |
                         GraphicsDeviceFeaturesBits::MultipleViewports);
        }
        
        if (_metalFeatures->IsDrawBaseVertexInstanceSupported()) {
            _features = (_features |
                         GraphicsDeviceFeaturesBits::DrawBaseVertex |
                         GraphicsDeviceFeaturesBits::DrawBaseInstance);
        }
        
        _resourceBindingModel = options.ResourceBindingModel;
        
        // TODO: Further implementations
    }
    
    void MTLGraphicsDevice::InitializeComponents() {
        auto device = ::__VD_NAMESPACE::GetRef<GraphicsDevice>(this);
        _resourceFactory = ::__VD_NAMESPACE::CreateRef<MTLResourceFactory>(device);
    }
    
    std::string MTLGraphicsDevice::GetDeviceName() { return [_deviceName cStringUsingEncoding:NSUTF8StringEncoding]; }
    std::string MTLGraphicsDevice::GetVendorName() { return "Apple"; }
    GraphicsApiVersion::Ref MTLGraphicsDevice::GetApiVersion() { return _apiVersion; }
    GraphicsBackend MTLGraphicsDevice::GetBackendType() { return GraphicsBackend::Metal; }
    Mochi::Bool MTLGraphicsDevice::IsUvOriginTopLeft() { return true; }
    Mochi::Bool MTLGraphicsDevice::IsDepthRangeZeroToOne() { return true; }
    Mochi::Bool MTLGraphicsDevice::IsClipSpaceYInverted() { return false; }
    ResourceFactory::Ref MTLGraphicsDevice::GetResourceFactory() { return _resourceFactory; }
    GraphicsDeviceFeatures MTLGraphicsDevice::GetFeatures() { return _features; }
    ResourceBindingModel MTLGraphicsDevice::GetResourceBindingModel() { return _resourceBindingModel; }
    
    void MTLGraphicsDevice::SubmitCommandsCore(CommandList::Ref commandList, Fence::Ref fence) {
        auto mtlCL = vd::AssertSubType<MTLCommandList>(commandList);
        [mtlCL->GetCommandBuffer() addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull cb) {
            OnCommandBufferCompleted(cb);
        }];
        
        std::lock_guard<std::mutex> lock(_submittedCommandsLock);
        if (fence) {
            auto mtlFence = vd::AssertSubType<MTLFence>(fence);
            _submittedCBs.insert({ mtlCL->GetCommandBuffer(), mtlFence });
        }
        
        _latestSubmittedCB = mtlCL->Commit();
    }
    
    void MTLGraphicsDevice::OnCommandBufferCompleted(id<MTLCommandBuffer> cb) {
        Mochi::ThrowNotImplemented();
    }
    
    // MARK: -
    
    MTLResourceFactory::MTLResourceFactory(MTLGraphicsDevice::Ref device)
    : ResourceFactory(device->GetFeatures()), _gd(device) {}
    
    GraphicsBackend MTLResourceFactory::GetBackendType() { return GraphicsBackend::Metal; }
    
    TextureView::Ref MTLResourceFactory::CreateTextureViewCore(TextureViewDescription &description) {
        Mochi::ThrowNotImplemented();
    }
    
    Pipeline::Ref MTLResourceFactory::CreateGraphicsPipelineCore(GraphicsPipelineDescription &description) {
        return Mochi::CreateRef<MTLPipeline>(description, _gd);
    }
    
    // MARK: -
    
    MTLPipeline::MTLPipeline(GraphicsPipelineDescription &description, MTLGraphicsDevice::Ref gd)
    : Pipeline(description) {
        
    }
    
    std::string MTLPipeline::GetName() {
        Mochi::ThrowNotImplemented();
    }
    void MTLPipeline::SetName(std::string name) {
        Mochi::ThrowNotImplemented();
    }
    Mochi::Bool MTLPipeline::IsDisposed() {
        Mochi::ThrowNotImplemented();
    }
    void MTLPipeline::Dispose() {
        Mochi::ThrowNotImplemented();
    }
    Mochi::Bool MTLPipeline::IsComputePipeline() {
        Mochi::ThrowNotImplemented();
    }
    
}
#endif // !defined(VD_EXCLUDE_METAL_BACKEND)
