//
//  vd.cpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#include <Veldrid/vd.hpp>
#include <Veldrid/backend_opengl.hpp>
#include <Veldrid/backend_metal.hpp>

namespace __VD_NAMESPACE {

    [[noreturn]] void ThrowNotImplemented(const std::source_location loc) {
        __MC_NAMESPACE::ThrowNotImplemented(loc);
    }

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

    Bool GraphicsApiVersion::TryParseGLVersion(std::string versionString, GraphicsApiVersion* outVersion) {
        // TODO: Needs implementation
        return false;
    }

    // MARK: -

    UInt32 GetSizeInBytes(PixelFormat format) {
        switch (format) {
        case PixelFormat::R8_UNorm:
        case PixelFormat::R8_SNorm:
        case PixelFormat::R8_UInt:
        case PixelFormat::R8_SInt:
            return 1;

        case PixelFormat::R16_UNorm:
        case PixelFormat::R16_SNorm:
        case PixelFormat::R16_UInt:
        case PixelFormat::R16_SInt:
        case PixelFormat::R16_Float:
        case PixelFormat::R8_G8_UNorm:
        case PixelFormat::R8_G8_SNorm:
        case PixelFormat::R8_G8_UInt:
        case PixelFormat::R8_G8_SInt:
            return 2;
        }

        throw Illegal::Value<PixelFormat>();
    }

    Bool IsCompressedFormat(PixelFormat format) {
        Mochi::ThrowNotImplemented();
    }

    // MARK: -

    UInt32 GetSizeInBytes(VertexElementFormat format) {
        switch (format) {
        case VertexElementFormat::Byte2_Norm:
        case VertexElementFormat::Byte2:
        case VertexElementFormat::SByte2_Norm:
        case VertexElementFormat::SByte2:
        case VertexElementFormat::Half1:
            return 2;
        }

        throw Illegal::Value<VertexElementFormat>();
    }

    // MARK: -

    #if defined(__APPLE__)
    #   if defined(__VD_TARGET_MACOS)
    SwapchainSource::Ref SwapchainSource::CreateNSWindow(void *nsWindow) {
        Mochi::ThrowNotImplemented();
    }

    NSWindowSwapchainSource::NSWindowSwapchainSource(NSWindow *nsWindow)
    : _window(nsWindow) {}

    NSWindow* NSWindowSwapchainSource::GetNSWindow() { return _window; }
    #   else
    SwapchainSource::Ref SwapchainSource::CreateUIKit(void *uiView) {
        Mochi::ThrowNotImplemented();
    }

    UIViewSwapchainSource::UIViewSwapchainSource(UIView *view)
    : _view(view) {}

    UIView* UIViewSwapchainSource::GetUIView() { return _view; }
    #   endif // defined(__VD_TARGET_MACOS)
    #endif // defined(__APPLE__)

    #if defined(_WIN32)

    Win32SwapchainSource::Win32SwapchainSource(void* hwnd, void* hInstance) : _hwnd(hwnd), _hInstance(hInstance) {}
    void* Win32SwapchainSource::GetHwnd() { return _hwnd; }
    void* Win32SwapchainSource::GetHInstance() { return _hInstance; }

    #endif // defined(_WIN32)

    // MARK: -

    Bool OutputAttachmentDescription::operator==(const OutputAttachmentDescription &other) {
        return Format == other.Format;
    }

    // MARK: -

    Bool BufferDescription::operator==(const BufferDescription& other) {
        return SizeInBytes == other.SizeInBytes &&
            Usage == other.Usage &&
            StructureByteStride == other.StructureByteStride &&
            RawBuffer == other.RawBuffer;
    }

    // MARK: -

    OutputDescription OutputDescription::CreateFromFramebuffer(Framebuffer::Ref fb) {
        TextureSampleCount sampleCount = static_cast<TextureSampleCount>(0);
        std::optional<OutputAttachmentDescription> depthAttachment;
        
        if (fb->GetDepthTarget().has_value()) {
            depthAttachment = std::optional<OutputAttachmentDescription>({
                { fb->GetDepthTarget().value().Target->GetFormat() }
            });
            sampleCount = fb->GetDepthTarget().value().Target->GetSampleCount();
        }
        
        std::vector<OutputAttachmentDescription> colorAttachments;
        for (auto attachment : fb->GetColorTargets()) {
            colorAttachments.push_back({ attachment.Target->GetFormat() });
            sampleCount = attachment.Target->GetSampleCount();
        }
        
        return { depthAttachment, colorAttachments, sampleCount };
    }

    // MARK: -

    Bool BlendAttachmentDescription::operator==(const BlendAttachmentDescription& other) {
        return (BlendEnabled == other.BlendEnabled &&
                ColorWriteMask == other.ColorWriteMask &&
                SourceColorFactor == other.SourceColorFactor &&
                DestinationColorFactor == other.DestinationColorFactor &&
                ColorFunction == other.ColorFunction &&
                SourceAlphaFactor == other.SourceAlphaFactor &&
                DestinationAlphaFactor == other.DestinationAlphaFactor &&
                AlphaFunction == other.AlphaFunction);
    }

    Bool BlendAttachmentDescription::operator!=(const BlendAttachmentDescription& other) {
        return !(*this == other);
    }

    // MARK: -

    Bool ResourceLayoutElementDescription::operator==(const ResourceLayoutElementDescription &other) const {
        return (Name == other.Name &&
                Kind == other.Kind &&
                Stages == other.Stages &&
                Options == other.Options);
    }

    // MARK: -

    Bool ResourceLayoutDescription::operator==(const ResourceLayoutDescription &other) const {
        return Elements == other.Elements;
    }

    // MARK: -

    ResourceLayout::ResourceLayout(ResourceLayoutDescription& description) {
    #if defined(VD_VALIDATE_USAGE)
        _description = description;
        
        for (auto element : description.Elements) {
            if (element.Options.HasFlag(ResourceLayoutElementOptionsBits::DynamicBinding)) {
                _dynamicBufferCount++;
            }
        }
    #endif // defined(VD_VALIDATE_USAGE)
    }

    // MARK: -

    Bool operator==(const VertexElementDescription& a, const VertexElementDescription& b) {
        return a.Name     == b.Name &&
            a.Format   == b.Format &&
            a.Semantic == b.Semantic &&
            a.Offset   == b.Offset;
    }

    // MARK: -

    Bool operator==(const VertexLayoutDescription& a, const VertexLayoutDescription& b) {
        return a.Stride           == b.Stride &&
            a.Elements         == b.Elements &&
            a.InstanceStepRate == b.InstanceStepRate;
    }

    // MARK: -

    Pipeline::Pipeline(GraphicsPipelineDescription& description) : Pipeline(description.ResourceLayouts) {
    #if defined(VD_VALIDATE_USAGE)
        _graphicsOutputDescription = description.Outputs;
    #endif // defined(VD_VALIDATE_USAGE)
    }

    Pipeline::Pipeline(std::vector<ResourceLayout::Ref> resourceLayouts) {
    #if defined(VD_VALIDATE_USAGE)
        _resourceLayouts = resourceLayouts;
    #endif // defined(VD_VALIDATE_USAGE)
    }

    // MARK: -

    TextureDescription TextureDescription::Texture1D(UInt32 width, UInt32 mipLevels,
                                                     UInt32 arrayLayers, PixelFormat format, TextureUsage usage) {
        return {
            width, 1, 1,
            mipLevels, arrayLayers, format, usage,
            TextureType::Texture1D, TextureSampleCount::Count1
        };
    }

    TextureDescription TextureDescription::Texture2D(UInt32 width, UInt32 height,
                                                     UInt32 mipLevels, UInt32 arrayLayers,
                                                     PixelFormat format, TextureUsage usage) {
        return {
            width, height, 1,
            mipLevels, arrayLayers, format, usage,
            TextureType::Texture2D, TextureSampleCount::Count1
        };
    }

    TextureDescription TextureDescription::Texture2D(UInt32 width, UInt32 height,
                                                     UInt32 mipLevels, UInt32 arrayLayers,
                                                     PixelFormat format, TextureUsage usage, TextureSampleCount sampleCount) {
        return {
            width, height, 1,
            mipLevels, arrayLayers, format, usage,
            TextureType::Texture2D, sampleCount
        };
    }

    TextureDescription TextureDescription::Texture3D(UInt32 width, UInt32 height, UInt32 depth,
                                                     UInt32 mipLevels, PixelFormat format, TextureUsage usage) {
        return {
            width, height, depth,
            mipLevels, 1, format, usage,
            TextureType::Texture3D, TextureSampleCount::Count1
        };
    }

    // MARK: -

    UInt32 Texture::CalculateSubresource(UInt32 mipLevel, UInt32 arrayLayer) {
        return arrayLayer * GetMipLevels() + mipLevel;
    }

    TextureView::Ref Texture::GetFullTextureView(GraphicsDevice::Ref gd) {
        std::lock_guard<std::mutex> lock(_fullTextureViewLock);
        if (_fullTextureView == nullptr) {
            _fullTextureView = CreateFullTextureView(gd);
        }
        
        return _fullTextureView;
    }

    TextureView::Ref Texture::CreateFullTextureView(GraphicsDevice::Ref gd) {
        return gd->GetResourceFactory()->CreateTextureView(shared_from_this());
    }

    void Texture::Dispose() {
        std::lock_guard<std::mutex> lock(_fullTextureViewLock);
        if (_fullTextureView) {
            _fullTextureView->Dispose();
        }
        
        DisposeCore();
    }

    // MARK:

    FramebufferAttachment::FramebufferAttachment(Texture::Ref target, UInt32 arrayLayer) {
        Target = target;
        ArrayLayer = arrayLayer;
        MipLevel = 0;
    }

    FramebufferAttachment::FramebufferAttachment(Texture::Ref target, UInt32 arrayLayer, UInt32 mipLevel) {
        Target = target;
        ArrayLayer = arrayLayer;
        MipLevel = mipLevel;
    }

    // MARK:

    FramebufferAttachmentDescription::FramebufferAttachmentDescription(Texture::Ref target, UInt32 arrayLayer) {
        Target = target;
        ArrayLayer = arrayLayer;
        MipLevel = 0;
    }

    FramebufferAttachmentDescription::FramebufferAttachmentDescription(Texture::Ref target, UInt32 arrayLayer, UInt32 mipLevel) {
        Target = target;
        ArrayLayer = arrayLayer;
        MipLevel = mipLevel;
    }

    // MARK: -

    Framebuffer::Framebuffer() {}
    Framebuffer::Framebuffer(std::optional<FramebufferAttachmentDescription> depthTargetDesc,
                            std::vector<FramebufferAttachmentDescription> colorTargetDescs) {
        if (depthTargetDesc.has_value()) {
            auto depthAttachment = depthTargetDesc.value();
            _depthTarget = FramebufferAttachment(depthAttachment.Target,
                                                depthAttachment.ArrayLayer,
                                                depthAttachment.MipLevel);
        }
        
        _colorTargets = std::vector<FramebufferAttachment>();
        for (auto& desc : colorTargetDescs) {
            _colorTargets.push_back(FramebufferAttachment(desc.Target,
                                                          desc.ArrayLayer,
                                                          desc.MipLevel));
        }
    }

    void Framebuffer::InitializeComponent() {
        _outputDescription = OutputDescription::CreateFromFramebuffer(shared_from_this());
    }

    std::optional<FramebufferAttachment> Framebuffer::GetDepthTarget() { return _depthTarget; }
    std::vector<FramebufferAttachment> Framebuffer::GetColorTargets() { return _colorTargets; }
    OutputDescription Framebuffer::GetOutputDescription() { return _outputDescription; }

    // MARK: -

    TextureViewDescription::TextureViewDescription(Texture::Ref target) {
        Target = target;
        BaseMipLevel = 0;
        MipLevels = target->GetMipLevels();
        BaseArrayLayer = 0;
        ArrayLayers = target->GetArrayLayers();
        Format = target->GetFormat();
    }

    // MARK: -

    ResourceFactory::ResourceFactory(GraphicsDeviceFeatures features)
    : _features(features) {}

    TextureView::Ref ResourceFactory::CreateTextureView(Texture::Ref target) {
        TextureViewDescription desc(target);
        return CreateTextureView(desc);
    }

    TextureView::Ref ResourceFactory::CreateTextureView(TextureViewDescription &description) {
    #if defined(VD_VALIDATE_USAGE)
        if (description.MipLevels == 0 ||
            description.ArrayLayers == 0 ||
            (description.BaseMipLevel + description.MipLevels) > description.Target->GetMipLevels() ||
            (description.BaseArrayLayer + description.ArrayLayers) > description.Target->GetArrayLayers()) {
            throw VeldridException("TextureView mip level and array layer range must be contained in the target Texture.");
        }
        
        if (!(description.Target->GetUsage() & TextureUsage::Sampled) &&
            !(description.Target->GetUsage() & TextureUsage::Storage)) {
            throw VeldridException("To create a TextureView, the target texture must have either Sampled or Storage usage flags.");
        }
        
        // TODO: More validations
    #endif // defined(VD_VALIDATE_USAGE)
        
        return CreateTextureViewCore(description);
    }

    Pipeline::Ref ResourceFactory::CreateGraphicsPipeline(GraphicsPipelineDescription &description) {
    #if defined(VD_VALIDATE_USAGE)
        if (!_features.HasFlag(GraphicsDeviceFeaturesBits::IndependentBlend)) {
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

    void GraphicsDevice::PostDeviceCreated() {
        
    }

    [[noreturn]] inline void ThrowPlatformExcludedException(const std::string& name) {
        throw VeldridException(name + " support has not been included in this configuration of Veldrid.");
    }

    #if !defined(VD_EXCLUDE_D3D11_BACKEND)
    #else
    GraphicsDevice::Ref GraphicsDevice::CreateD3D11(GraphicsDeviceOptions options, ...) {
        ThrowPlatformExcludedException("DirectX 11");
    }
    #endif // !defined(VD_EXCLUDE_D3D11_BACKEND)

    #if !defined(VD_EXCLUDE_OPENGL_BACKEND)
    GraphicsDevice::Ref GraphicsDevice::CreateOpenGL(GraphicsDeviceOptions options,
                                                    OpenGLPlatformInfo::Ref info,
                                                    UInt32 width,
                                                    UInt32 height) {
        auto device = Mochi::CreateRef<OpenGLGraphicsDevice>(options, info, width, height);
        device->InitializeComponents();
        return device;
    }

    GraphicsDevice::Ref GraphicsDevice::CreateOpenGLES(GraphicsDeviceOptions options,
                                                    SwapchainDescription description) {
        Mochi::ThrowNotImplemented();
    }
    #else
    GraphicsDevice::Ref GraphicsDevice::CreateOpenGL(GraphicsDeviceOptions options, ...) {
        ThrowPlatformExcludedException("OpenGL");
    }

    GraphicsDevice::Ref GraphicsDevice::CreateOpenGLES(GraphicsDeviceOptions options, ...) {
        ThrowPlatformExcludedException("OpenGL");
    }
    #endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)

    #if !defined(VD_EXCLUDE_METAL_BACKEND)
    GraphicsDevice::Ref GraphicsDevice::CreateMetal(GraphicsDeviceOptions options) {
        auto device = Mochi::CreateRef<MTLGraphicsDevice>(options, std::optional<SwapchainDescription>());
        device->InitializeComponents();
        return device;
    }

    GraphicsDevice::Ref GraphicsDevice::CreateMetal(GraphicsDeviceOptions options,
                                                    SwapchainDescription description) {
        auto device = Mochi::CreateRef<MTLGraphicsDevice>(options, std::optional<SwapchainDescription>({description}));
        device->InitializeComponents();
        return device;
    }

    GraphicsDevice::Ref GraphicsDevice::CreateMetal(GraphicsDeviceOptions options,
                                                    MetalContainerRef container) {
        SwapchainDescription desc = {
    #   if defined(__VD_TARGET_MACOS)
            Mochi::CreateRef<NSWindowSwapchainSource>(container),
    #   else
            Mochi::CreateRef<UIViewSwapchainSource>(container),
    #   endif // defined(__VD_TARGET_MACOS)
            0, 0,
            options.SwapchainDepthFormat,
            options.SyncToVerticalBlank,
            options.SwapchainSrgbFormat
        };
        
        auto device = Mochi::CreateRef<MTLGraphicsDevice>(options, std::optional<SwapchainDescription>({desc}));
        device->InitializeComponents();
        return device;
    }
    #else
    GraphicsDevice::Ref GraphicsDevice::CreateMetal(GraphicsDeviceOptions options, ...) {
        ThrowPlatformExcludedException("Metal");
    }
    #endif // !defined(VD_EXCLUDE_METAL_BACKEND)

}
