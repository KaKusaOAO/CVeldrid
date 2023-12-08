//
//  vd.cpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#include <Veldrid/vd.hpp>
#include <Veldrid/backend_opengl.hpp>
#include <Veldrid/backend_metal.hpp>

namespace vd {

// MARK: -

VeldridException::VeldridException() {}
VeldridException::VeldridException(std::string message) : _message(message) {}
VeldridException::VeldridException(std::string message, std::exception& inner)
: _message(message), _inner{inner} {}

const char* VeldridException::what() const noexcept { return _message.c_str(); }
std::optional<std::exception> VeldridException::GetInnerException() { return _inner; }

// MARK: -

GraphicsApiVersion::GraphicsApiVersion(int major, int minor, int subminor, int patch)
: _major(major), _minor(minor), _subminor(subminor), _patch(patch) {}

int GraphicsApiVersion::GetMajor() const    { return _major; }
int GraphicsApiVersion::GetMinor() const    { return _minor; }
int GraphicsApiVersion::GetSubminor() const { return _subminor; }
int GraphicsApiVersion::GetPatch() const    { return _patch; }

std::ostream& operator<<(std::ostream& os, const GraphicsApiVersion& version) {
    os << version._major << ".";
    os << version._minor << ".";
    os << version._subminor << ".";
    os << version._patch;
    
    return os;
}

Mochi::Bool GraphicsApiVersion::TryParseGLVersion(std::string versionString, GraphicsApiVersion* outVersion) {
    // TODO: Needs implementation
    return false;
}

// MARK: -

#if defined(__APPLE__)
#if defined(__VD_TARGET_MACOS)
SwapchainSourceRef SwapchainSource::CreateNSWindow(void *uiView) {
    Mochi::ThrowNotImplemented();
}
#else
SwapchainSourceRef SwapchainSource::CreateUIKit(void *uiView) {
    Mochi::ThrowNotImplemented();
}
#endif // defined(__VD_TARGET_MACOS)
#endif // defined(__APPLE__)

// MARK: -

Mochi::Bool OutputAttachmentDescription::operator==(const OutputAttachmentDescription &other) {
    return Format == other.Format;
}

// MARK: -

Mochi::Bool BlendAttachmentDescription::operator==(const BlendAttachmentDescription& other) {
    return (BlendEnabled == other.BlendEnabled &&
            ColorWriteMask == other.ColorWriteMask &&
            SourceColorFactor == other.SourceColorFactor &&
            DestinationColorFactor == other.DestinationColorFactor &&
            ColorFunction == other.ColorFunction &&
            SourceAlphaFactor == other.SourceAlphaFactor &&
            DestinationAlphaFactor == other.DestinationAlphaFactor &&
            AlphaFunction == other.AlphaFunction);
}

Mochi::Bool BlendAttachmentDescription::operator!=(const BlendAttachmentDescription& other) {
    return !(*this == other);
}

// MARK: -

Mochi::Bool ResourceLayoutElementDescription::operator==(const ResourceLayoutElementDescription &other) const {
    return (Name == other.Name &&
            Kind == other.Kind &&
            Stages == other.Stages &&
            Options == other.Options);
}

// MARK: -

Mochi::Bool ResourceLayoutDescription::operator==(const ResourceLayoutDescription &other) const {
    return Elements == other.Elements;
}

// MARK: -

ResourceLayout::ResourceLayout(ResourceLayoutDescription& description) {
#if defined(VD_VALIDATE_USAGE)
    _description = description;
    
    for (auto element : description.Elements) {
        if ((element.Options & ResourceLayoutElementOptionDynamicBinding) != 0) {
            _dynamicBufferCount++;
        }
    }
#endif // defined(VD_VALIDATE_USAGE)
}

// MARK: -

Pipeline::Pipeline(GraphicsPipelineDescription& description) : Pipeline(description.ResourceLayouts) {
#if defined(VD_VALIDATE_USAGE)
    _graphicsOutputDescription = description.Outputs;
#endif // defined(VD_VALIDATE_USAGE)
}

Pipeline::Pipeline(std::vector<ResourceLayoutRef> resourceLayouts) {
#if defined(VD_VALIDATE_USAGE)
    // TODO: Copy resource layouts and store
#endif
}

// MARK: -

ResourceFactory::ResourceFactory(GraphicsDeviceFeatures features)
: _features(features) {}

PipelineRef ResourceFactory::CreateGraphicsPipeline(GraphicsPipelineDescription &description) {
#if defined(VD_VALIDATE_USAGE)
    if ((_features & GDFeatureIndependentBlend) == 0) {
        if (description.BlendState.AttachmentStates.size() > 0) {
            auto attachmentState = description.BlendState.AttachmentStates[0];
            for (int i = 1; i < description.BlendState.AttachmentStates.size(); i++) {
                if (attachmentState != description.BlendState.AttachmentStates[i]) {
                    throw VeldridException("If GraphicsDeviceFeatures.IndependentBlend is false, then all members of BlendState.AttachmentStates must be equal.");
                }
            }
        }
    }
#endif // defined(VD_VALIDATE_USAGE)
    
    return CreateGraphicsPipelineCore(description);
}

// MARK: -

[[noreturn]] inline void ThrowPlatformExcludedException(std::string name) {
    throw VeldridException(name + " support has not been included in this configuration of Veldrid");
}

#if !defined(VD_EXCLUDE_OPENGL_BACKEND)
std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateOpenGL(GraphicsDeviceOptions options,
                                                             OpenGLPlatformInfoRef info,
                                                             Mochi::UInt32 width,
                                                             Mochi::UInt32 height) {
    auto device = std::make_shared<OpenGLGraphicsDevice>(options, info, width, height);
    device->InitializeComponents();
    return device;
}

std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateOpenGLES(GraphicsDeviceOptions options,
                                                               SwapchainDescription description) {
    Mochi::ThrowNotImplemented();
}
#else
std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateOpenGL(GraphicsDeviceOptions options, ...) {
    ThrowPlatformExcludedException("OpenGL");
}

std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateOpenGLES(GraphicsDeviceOptions options, ...) {
    ThrowPlatformExcludedException("OpenGL");
}
#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)

#if !defined(VD_EXCLUDE_METAL_BACKEND)
std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateMetal(GraphicsDeviceOptions options) {
    auto device = std::make_shared<MTLGraphicsDevice>(options, std::optional<SwapchainDescription>());
    device->InitializeComponents();
    return device;
}

std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateMetal(GraphicsDeviceOptions options,
                                                            SwapchainDescription description) {
    auto device = std::make_shared<MTLGraphicsDevice>(options, std::optional<SwapchainDescription>({description}));
    device->InitializeComponents();
    return device;
}

std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateMetal(GraphicsDeviceOptions options,
                                                            MetalContainerRef container) {
    SwapchainDescription desc = {
#if defined(__VD_TARGET_MACOS)
        std::make_shared<NSWindowSwapchainSource>(container),
#else
        std::make_shared<UIViewSwapchainSource>(container),
#endif // defined(__VD_TARGET_MACOS)
        0, 0,
        options.SwapchainDepthFormat,
        options.SyncToVerticalBlank,
        options.SwapchainSrgbFormat
    };
    
    auto device = std::make_shared<MTLGraphicsDevice>(options, std::optional<SwapchainDescription>({desc}));
    device->InitializeComponents();
    return device;
}
#else
std::shared_ptr<GraphicsDevice> GraphicsDevice::CreateMetal(GraphicsDeviceOptions options, ...) {
    ThrowPlatformExcludedException("Metal");
}
#endif // !defined(VD_EXCLUDE_METAL_BACKEND)

}
