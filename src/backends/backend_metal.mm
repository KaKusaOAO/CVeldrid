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

namespace vd {

// MARK: - NSWindowSwapchainSource

#if defined(__VD_TARGET_MACOS)
NSWindowSwapchainSource::NSWindowSwapchainSource(NSWindow *nsWindow)
: _window(nsWindow) {}

NSWindow* NSWindowSwapchainSource::GetNSWindow() { return _window; }
#else
UIViewSwapchainSource::UIViewSwapchainSource(UIView *view)
: _view(view) {}

UIView* UIViewSwapchainSource::GetUIView() { return _view; }
#endif // defined(__VD_TARGET_MACOS)

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

MTLFeatureSupport::Iterator MTLFeatureSupport::begin() { return _supportedFeatureSets.begin(); }
MTLFeatureSupport::Iterator MTLFeatureSupport::end()   { return _supportedFeatureSets.end();   }
Mochi::Bool MTLFeatureSupport::IsMacOS() { return _isMacOS; }
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
        
    _isMacOS = IsSupported(MTLGPUFamilyMac1) ||
               IsSupported(MTLGPUFamilyMac2) ||
               IsSupported(MTLGPUFamilyMacCatalyst1) ||
               IsSupported(MTLGPUFamilyMacCatalyst2);
}

Mochi::Bool MTLFeatureSupport::IsDrawBaseVertexInstanceSupported() {
    return IsSupported(MTLGPUFamilyApple2) ||
           IsSupported(MTLGPUFamilyApple3) ||
           IsSupported(MTLGPUFamilyApple4) ||
           _isMacOS;
}

// MARK: - MTLGraphicsDevice

MTLGraphicsDevice::MTLGraphicsDevice(GraphicsDeviceOptions options,
                                     std::optional<SwapchainDescription> swapchainDesc) {
    _device = MTLCreateSystemDefaultDevice();
    _deviceName = [_device name];
    _metalFeatures = std::make_shared<MTLFeatureSupport>(_device);
    
    int major = (int) _metalFeatures->GetMaxGPUFamily() / 10000;
    int minor = (int) _metalFeatures->GetMaxGPUFamily() % 10000;
    _apiVersion = std::make_shared<GraphicsApiVersion>(major, minor, 0, 0);
    
    _features = (GraphicsDeviceFeatures::ComputeShader |
                 GraphicsDeviceFeatures::DrawIndirect |
                 GraphicsDeviceFeatures::DrawIndirectBaseInstance |
                 GraphicsDeviceFeatures::FillModeWireframe |
                 GraphicsDeviceFeatures::SamplerAnisotropy |
                 GraphicsDeviceFeatures::DepthClipDisable |
                 GraphicsDeviceFeatures::Texture1D |
                 GraphicsDeviceFeatures::IndependentBlend |
                 GraphicsDeviceFeatures::StructuredBuffer |
                 GraphicsDeviceFeatures::SubsetTextureView |
                 GraphicsDeviceFeatures::CommandListDebugMarkers |
                 GraphicsDeviceFeatures::BufferRangeBinding);
    
    if (_metalFeatures->IsSupported(MTLGPUFamilyMac1)) {
        _features = (_features |
                     GraphicsDeviceFeatures::MultipleViewports);
    }
    
    if (_metalFeatures->IsDrawBaseVertexInstanceSupported()) {
        _features = (_features |
                     GraphicsDeviceFeatures::DrawBaseVertex |
                     GraphicsDeviceFeatures::DrawBaseInstance);
    }
    
    _resourceBindingModel = options.ResourceBindingModel;
    
    // TODO: Further implementations
}

void MTLGraphicsDevice::InitializeComponents() {
    auto device = vd::AssertSubType<MTLGraphicsDevice>(shared_from_this());
    _resourceFactory = std::make_shared<MTLResourceFactory>(device);
}

std::string MTLGraphicsDevice::GetDeviceName() { return [_deviceName cStringUsingEncoding:NSUTF8StringEncoding]; }
std::string MTLGraphicsDevice::GetVendorName() { return "Apple"; }
GraphicsApiVersionRef MTLGraphicsDevice::GetApiVersion() { return _apiVersion; }
GraphicsBackend MTLGraphicsDevice::GetBackendType() { return GraphicsBackend::Metal; }
Mochi::Bool MTLGraphicsDevice::IsUvOriginTopLeft() { return true; }
Mochi::Bool MTLGraphicsDevice::IsDepthRangeZeroToOne() { return true; }
Mochi::Bool MTLGraphicsDevice::IsClipSpaceYInverted() { return false; }
ResourceFactoryRef MTLGraphicsDevice::GetResourceFactory() { return _resourceFactory; }
GraphicsDeviceFeatures MTLGraphicsDevice::GetFeatures() { return _features; }
ResourceBindingModel MTLGraphicsDevice::GetResourceBindingModel() { return _resourceBindingModel; }

// MARK: - MTLResourceFactory

MTLResourceFactory::MTLResourceFactory(MTLGraphicsDeviceRef device)
: ResourceFactory(device->GetFeatures()), _gd(device) {}

GraphicsBackend MTLResourceFactory::GetBackendType() { return GraphicsBackend::Metal; }

PipelineRef MTLResourceFactory::CreateGraphicsPipelineCore(GraphicsPipelineDescription &description) {
    return std::make_shared<MTLPipeline>(description, _gd);
}

// MARK: - MTLPipeline

MTLPipeline::MTLPipeline(GraphicsPipelineDescription &description, MTLGraphicsDeviceRef gd) 
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
