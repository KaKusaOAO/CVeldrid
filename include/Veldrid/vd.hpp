//
//  vd.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#if defined(__cplusplus)
#ifndef __VD_HPP_HEADER_GUARD
#define __VD_HPP_HEADER_GUARD

#include <Mochi/common.hpp>
#include <Mochi/core.hpp>
#include <vector>
#include <Veldrid/config.hpp>
#include <optional>
#include <sstream>

#if defined(__APPLE__)
#   if defined(__VD_TARGET_MACOS)
#       include <AppKit/AppKit.h>
#   else
#       include <UIKit/UIKit.h>
#   endif // defined(__VD_TARGET_MACOS)
#endif // defined(__APPLE__)

namespace vd {

class VeldridException : public std::exception {
private:
    std::string _message;
    std::optional<std::exception> _inner;
public:
    VeldridException();
    VeldridException(std::string message);
    VeldridException(std::string message, std::exception &inner);
    
    const char* what() const noexcept override;
    std::optional<std::exception> GetInnerException();
};

template <class T, class Base>
std::shared_ptr<T> AssertSubType(std::shared_ptr<Base> value) {
    static_assert(std::is_base_of<Base, T>::value, "The given T class must derive from Base (argument).");
    
    std::stringstream str;
    
    if (!value) {
        str << "Expected object of type " << typeid(T).name() << " but received null instead.";
        throw VeldridException(str.str());
    }
    
    auto result = std::dynamic_pointer_cast<T>(value);
    if (!result) {
        str << "Object " << value << " (typeof " << typeid(Base).name() << ")";
        str << " must be derived type " << typeid(T).name() << " to be used in this context.";
        throw VeldridException(str.str());
    }
    
    return result;
}

struct GraphicsApiVersion {
private:
    int _major;
    int _minor;
    int _subminor;
    int _patch;
    
public:
    GraphicsApiVersion(int major, int minor, int subminor, int patch);
    int GetMajor() const;
    int GetMinor() const;
    int GetSubminor() const;
    int GetPatch() const;
    
    friend std::ostream& operator<<(std::ostream& os, const GraphicsApiVersion& version);
    static Mochi::Bool TryParseGLVersion(std::string versionString, GraphicsApiVersion* outVersion);
};

__MC_DEFINE_REF_TYPE(GraphicsApiVersion)

enum class PixelFormat {
    R8_G8_B8_A8_UNorm,
};

// MARK: ResourceBindingModel
enum class ResourceBindingModel {
    Default, Improved
};

struct GraphicsDeviceOptions {
public:
    Mochi::Bool Debug;
    Mochi::Bool HasMainSwapchain;
    std::optional<PixelFormat> SwapchainDepthFormat;
    Mochi::Bool SyncToVerticalBlank;
    ResourceBindingModel ResourceBindingModel;
    Mochi::Bool PreferDepthRangeZeroToOne;
    Mochi::Bool PreferStandardClipSpaceYDirection;
    Mochi::Bool SwapchainSrgbFormat;
};

// __MC_DEFINE_REF_TYPE(GraphicsDeviceOptions)

class SwapchainSource {
    using Ref = std::shared_ptr<SwapchainSource>;
    
public:
#if defined(__APPLE__)
#if defined(__VD_TARGET_MACOS)
    static Ref CreateNSWindow(void *nsWindow);
#else
    static Ref CreateUIKit(void *uiView);
#endif // defined(TARGET_OS_MAC)
#endif // defined(__APPLE__)
};

__MC_DEFINE_REF_TYPE(SwapchainSource)

class IMappableResource {};
__MC_DEFINE_REF_TYPE(IMappableResource)

class IBindableResource {};
__MC_DEFINE_REF_TYPE(IBindableResource)

struct SwapchainDescription {
    SwapchainSourceRef Source;
    Mochi::UInt32 Width;
    Mochi::UInt32 Height;
    std::optional<PixelFormat> DepthFormat;
    Mochi::Bool SyncToVerticalBlank;
    Mochi::Bool ColorSrgb;
};

enum class GraphicsDeviceFeatures : Mochi::UInt32 {
    None                     = 0,
    ComputeShader            = 1 << 0,
    GeometryShader           = 1 << 1,
    TessellationShaders      = 1 << 2,
    MultipleViewports        = 1 << 3,
    SamplerLodBias           = 1 << 4,
    DrawBaseVertex           = 1 << 5,
    DrawBaseInstance         = 1 << 6,
    DrawIndirect             = 1 << 7,
    DrawIndirectBaseInstance = 1 << 8,
    FillModeWireframe        = 1 << 9,
    SamplerAnisotropy        = 1 << 10,
    DepthClipDisable         = 1 << 11,
    Texture1D                = 1 << 12,
    IndependentBlend         = 1 << 13,
    StructuredBuffer         = 1 << 14,
    SubsetTextureView        = 1 << 15,
    CommandListDebugMarkers  = 1 << 16,
    BufferRangeBinding       = 1 << 17,
    ShaderFloat64            = 1 << 18
};

inline GraphicsDeviceFeatures operator|(GraphicsDeviceFeatures a, GraphicsDeviceFeatures b) {
    return static_cast<GraphicsDeviceFeatures>(static_cast<Mochi::UInt32>(a) |
                                               static_cast<Mochi::UInt32>(b));
}

inline bool operator&(GraphicsDeviceFeatures a, GraphicsDeviceFeatures b) {
    return (static_cast<Mochi::UInt32>(a) & static_cast<Mochi::UInt32>(b)) != 0;
}

enum class GraphicsBackend {
    Direct3D11,
    Vulkan,
    OpenGL,
    Metal,
    OpenGLES
};

struct OutputAttachmentDescription {
    PixelFormat Format;
    Mochi::Bool operator==(const OutputAttachmentDescription& other);
};

enum class TextureSampleCount {
#define __ENTRY(count) Count##count = count
    __ENTRY(1),
    __ENTRY(2),
    __ENTRY(4),
    __ENTRY(8),
    __ENTRY(16),
    __ENTRY(32)
#undef __ENTRY
};

class Framebuffer;
__MC_DEFINE_REF_TYPE(Framebuffer)

struct OutputDescription {
    std::optional<OutputAttachmentDescription> DepthAttachment;
    std::vector<OutputAttachmentDescription> ColorAttachments;
    TextureSampleCount SampleCount;
    
    static OutputDescription CreateFromFramebuffer(FramebufferRef fb);
};

class IDeviceResource {
public:
    virtual std::string GetName() = 0;
    virtual void SetName(std::string name) = 0;
};

class Fence : public IDeviceResource, public Mochi::IDisposable {
public:
    virtual Mochi::Bool IsSignaled() = 0;
    virtual void Reset() = 0;
    virtual std::string GetName() override = 0;
    virtual void SetName(std::string name) override = 0;
    virtual Mochi::Bool IsDisposed() = 0;
    virtual void Dispose() override = 0;
};

__MC_DEFINE_REF_TYPE(Fence)

struct RgbaFloat {
    float R;
    float G;
    float B;
    float A;
};

enum ColorWriteMask : int {
    None,
    Red   = 1 << 0,
    Green = 1 << 1,
    Blue  = 1 << 2,
    Alpha = 1 << 3,
    
    All = Red | Green | Blue | Alpha
};

inline ColorWriteMask operator|(ColorWriteMask a, ColorWriteMask b) {
    return static_cast<ColorWriteMask>(static_cast<int>(a) |
                                       static_cast<int>(b));
}

enum class BlendFactor {
    Zero,
    One,
    SourceAlpha,
    // TODO: Write remaining entries
};

enum class BlendFunction {
    Add,
    Subtract,
    ReverseSubtract,
    // TODO: Write remaining entries
};

struct BlendAttachmentDescription {
    Mochi::Bool BlendEnabled;
    std::optional<ColorWriteMask> ColorWriteMask;
    
    // FIXME: Refactor these into a smaller struct
    
    BlendFactor SourceColorFactor;
    BlendFactor DestinationColorFactor;
    BlendFunction ColorFunction;
    
    BlendFactor SourceAlphaFactor;
    BlendFactor DestinationAlphaFactor;
    BlendFunction AlphaFunction;
    
    Mochi::Bool operator==(const BlendAttachmentDescription& other);
    Mochi::Bool operator!=(const BlendAttachmentDescription& other);
};

struct BlendStateDescription {
    RgbaFloat BlendFactor;
    std::vector<BlendAttachmentDescription> AttachmentStates;
    Mochi::Bool AlphaToCoverageEnabled;
};

enum class ResourceKind {
    UniformBuffer,
    StructuredBufferReadOnly,
    StructuredBufferReadWrite,
    TextureReadOnly,
    TextureReadWrite,
    Sampler
};

enum class ShaderStages : Mochi::UInt8 {
    None,
    Vertex                 = 1 << 0,
    Geometry               = 1 << 1,
    TessellationControl    = 1 << 2,
    TessellationEvaluation = 1 << 3,
    Fragment               = 1 << 4,
    Compute                = 1 << 5,
};

inline ShaderStages operator|(ShaderStages a, ShaderStages b) {
    return static_cast<ShaderStages>(static_cast<Mochi::UInt8>(a) |
                                     static_cast<Mochi::UInt8>(b));
}

enum class ResourceLayoutElementOptions : int {
    None,
    DynamicBinding         = 1 << 0
};

inline ResourceLayoutElementOptions operator|(ResourceLayoutElementOptions a, ResourceLayoutElementOptions b) {
    return static_cast<ResourceLayoutElementOptions>(static_cast<int>(a) |
                                                     static_cast<int>(b));
}

inline bool operator&(ResourceLayoutElementOptions a, ResourceLayoutElementOptions b) {
    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}

struct ResourceLayoutElementDescription {
    std::string Name;
    ResourceKind Kind;
    ShaderStages Stages;
    ResourceLayoutElementOptions Options;
    Mochi::Bool operator==(const ResourceLayoutElementDescription& other) const;
};

struct ResourceLayoutDescription {
    std::vector<ResourceLayoutElementDescription> Elements;
    Mochi::Bool operator==(const ResourceLayoutDescription& other) const;
};

class ResourceLayout : public IDeviceResource, public Mochi::IDisposable {
private:
#if defined(VD_VALIDATE_USAGE)
    ResourceLayoutDescription _description;
    Mochi::UInt32 _dynamicBufferCount;
#endif // defined(VD_VALIDATE_USAGE)
    
protected:
    ResourceLayout(ResourceLayoutDescription& description);
    
public:
    
};

__MC_DEFINE_REF_TYPE(ResourceLayout)

struct GraphicsPipelineDescription {
    BlendStateDescription BlendState;
    OutputDescription Outputs;
    std::optional<ResourceBindingModel> ResourceBindingModel;
    std::vector<ResourceLayoutRef> ResourceLayouts;
};

class Pipeline : public IDeviceResource, public Mochi::IDisposable {
private:
#if defined(VD_VALIDATE_USAGE)
    OutputDescription _graphicsOutputDescription;
#endif
    
public:
    Pipeline(GraphicsPipelineDescription& description);
    Pipeline(std::vector<ResourceLayoutRef> resourceLayouts);
    
    virtual Mochi::Bool IsComputePipeline() = 0;
    virtual Mochi::Bool IsDisposed() = 0;
    virtual void Dispose() override = 0;
    
#if defined(VD_VALIDATE_USAGE)
    OutputDescription GetGraphicsOutputDescription();
#endif
};

__MC_DEFINE_REF_TYPE(Pipeline)

class Texture;
__MC_DEFINE_REF_TYPE(Texture)

struct TextureViewDescription {
    TextureRef Target;
    Mochi::UInt32 BaseMipLevel;
    Mochi::UInt32 MipLevels;
    Mochi::UInt32 BaseArrayLayer;
    Mochi::UInt32 ArrayLayers;
    std::optional<PixelFormat> Format;
    
    TextureViewDescription(TextureRef target);
    TextureViewDescription(TextureRef target, PixelFormat format);
    TextureViewDescription(TextureRef target, Mochi::UInt32 baseMipLevel, Mochi::UInt32 mipLevels, Mochi::UInt32 baseArrayLayer, Mochi::UInt32 arrayLayers);
    TextureViewDescription(TextureRef target, PixelFormat format, Mochi::UInt32 baseMipLevel, Mochi::UInt32 mipLevels, Mochi::UInt32 baseArrayLayer, Mochi::UInt32 arrayLayers);
    
    Mochi::Bool operator=(const TextureViewDescription& other);
};

class TextureView : public IBindableResource, public IDeviceResource, public Mochi::IDisposable {
private:
    TextureRef _target;
    
protected:
    TextureView(TextureViewDescription& description);
    
public:
    TextureRef    GetTarget();
    Mochi::UInt32 GetBaseMipLevel();
    Mochi::UInt32 GetMipLevels();
    Mochi::UInt32 GetBaseArrayLayer();
    Mochi::UInt32 GetArrayLayers();
    PixelFormat   GetFormat();
    
    virtual std::string GetName() override = 0;
    virtual void SetName(std::string name) override = 0;
    virtual Mochi::Bool IsDisposed() = 0;
    virtual void Dispose() override = 0;
};

__MC_DEFINE_REF_TYPE(TextureView)

enum class TextureUsage : char {
    Sampled         = 1 << 0,
    Storage         = 1 << 1,
    RenderTarget    = 1 << 2,
    DepthStencil    = 1 << 3,
    Cubemap         = 1 << 4,
    Staging         = 1 << 5,
    GenerateMipmaps = 1 << 6
};

inline TextureUsage operator|(TextureUsage a, TextureUsage b) {
    return static_cast<TextureUsage>(static_cast<char>(a) | static_cast<char>(b));
}

inline Mochi::Bool operator&(TextureUsage a, TextureUsage b) {
    return (static_cast<char>(a) & static_cast<char>(b)) != 0;
}

enum class TextureType {
    Texture1D,
    Texture2D,
    Texture3D
};

struct TextureDescription {
    Mochi::UInt32 Width;
    Mochi::UInt32 Height;
    Mochi::UInt32 Depth;
    Mochi::UInt32 MipLevels;
    Mochi::UInt32 ArrayLayers;
    PixelFormat Format;
    TextureUsage Usage;
    TextureType Type;
    TextureSampleCount SampleCount;
    
    static TextureDescription Texture1D(Mochi::UInt32 width, Mochi::UInt32 mipLevels, Mochi::UInt32 arrayLayers, PixelFormat format, TextureUsage usage);
    static TextureDescription Texture2D(Mochi::UInt32 width, Mochi::UInt32 height, Mochi::UInt32 mipLevels, Mochi::UInt32 arrayLayers, PixelFormat format, TextureUsage usage);
    static TextureDescription Texture2D(Mochi::UInt32 width, Mochi::UInt32 height, Mochi::UInt32 mipLevels, Mochi::UInt32 arrayLayers, PixelFormat format, TextureUsage usage, TextureSampleCount sampleCount);
    static TextureDescription Texture3D(Mochi::UInt32 width, Mochi::UInt32 height, Mochi::UInt32 depth, Mochi::UInt32 mipLevels, PixelFormat format, TextureUsage usage);
};

class GraphicsDevice;
__MC_DEFINE_REF_TYPE(GraphicsDevice)

class Texture : public IDeviceResource, public IMappableResource, public Mochi::IDisposable, public IBindableResource, public std::enable_shared_from_this<Texture> {
private:
    std::mutex     _fullTextureViewLock;
    TextureViewRef _fullTextureView;
    
protected:
    TextureViewRef CreateFullTextureView(GraphicsDeviceRef gd);
    virtual void DisposeCore() = 0;
    
public:
    Mochi::UInt32 CalculateSubresource(Mochi::UInt32 mipLevel, Mochi::UInt32 arrayLayer);
    TextureViewRef GetFullTextureView(GraphicsDeviceRef gd);
    
    virtual PixelFormat        GetFormat()      = 0;
    virtual Mochi::UInt32      GetWidth()       = 0;
    virtual Mochi::UInt32      GetHeight()      = 0;
    virtual Mochi::UInt32      GetDepth()       = 0;
    virtual Mochi::UInt32      GetMipLevels()   = 0;
    virtual Mochi::UInt32      GetArrayLayers() = 0;
    virtual TextureUsage       GetUsage()       = 0;
    virtual TextureType        GetType()        = 0;
    virtual TextureSampleCount GetSampleCount() = 0;
    
    virtual std::string GetName() override = 0;
    virtual void SetName(std::string name) override = 0;
    virtual Mochi::Bool IsDisposed() = 0;
    virtual void Dispose() override;
};

struct FramebufferAttachment {
    TextureRef Target;
    Mochi::UInt32 ArrayLayer;
    Mochi::UInt32 MipLevel;
    
    FramebufferAttachment(TextureRef target, Mochi::UInt32 arrayLayer);
    FramebufferAttachment(TextureRef target, Mochi::UInt32 arrayLayer, Mochi::UInt32 mipLevel);
};

struct FramebufferAttachmentDescription {
    TextureRef Target;
    Mochi::UInt32 ArrayLayer;
    Mochi::UInt32 MipLevel;
    
    FramebufferAttachmentDescription(TextureRef target, Mochi::UInt32 arrayLayer);
    FramebufferAttachmentDescription(TextureRef target, Mochi::UInt32 arrayLayer, Mochi::UInt32 mipLevel);
};

struct FramebufferDescription {
    std::optional<FramebufferAttachmentDescription> DepthTarget;
    std::vector<FramebufferAttachmentDescription> ColorTargets;
};

class Framebuffer : public IDeviceResource, public Mochi::IDisposable, public std::enable_shared_from_this<Framebuffer> {
private:
    std::optional<FramebufferAttachment> _depthTarget;
    std::vector<FramebufferAttachment> _colorTargets;
    OutputDescription _outputDescription;
    
protected:
    Framebuffer();
    Framebuffer(std::optional<FramebufferAttachmentDescription> depthTarget,
                std::vector<FramebufferAttachmentDescription> colorTargets);
    void InitializeComponent();
    
public:
    virtual std::optional<FramebufferAttachment> GetDepthTarget();
    virtual std::vector<FramebufferAttachment> GetColorTargets();
    virtual OutputDescription GetOutputDescription();
    
    virtual std::string GetName() override = 0;
    virtual void SetName(std::string name) override = 0;
    virtual Mochi::Bool IsDisposed() = 0;
    virtual void Dispose() override = 0;
};

class CommandList : public IDeviceResource, public Mochi::IDisposable {
private:
    GraphicsDeviceFeatures _features;
    Mochi::UInt32 _uniformBufferAlignment;
    Mochi::UInt32 _structuredBufferAlignment;
    
protected:
    
};

__MC_DEFINE_REF_TYPE(CommandList)

class ResourceFactory {
private:
    GraphicsDeviceFeatures _features;
    
protected:
    ResourceFactory(GraphicsDeviceFeatures features);
    virtual TextureViewRef CreateTextureViewCore(TextureViewDescription& description) = 0;
    virtual PipelineRef    CreateGraphicsPipelineCore(GraphicsPipelineDescription& description) = 0;
    
public:
    virtual GraphicsBackend GetBackendType() = 0;
    GraphicsDeviceFeatures GetFeatures();
    
    TextureViewRef CreateTextureView(TextureRef target);
    TextureViewRef CreateTextureView(TextureViewDescription& description);
    PipelineRef    CreateGraphicsPipeline(GraphicsPipelineDescription& description);
};

__MC_DEFINE_REF_TYPE(ResourceFactory)

#if !defined(VD_EXCLUDE_OPENGL_BACKEND)
class OpenGLPlatformInfo;
typedef std::shared_ptr<OpenGLPlatformInfo> OpenGLPlatformInfoRef;
#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)

#if !defined(VD_EXCLUDE_D3D11_BACKEND)
struct D3D11DeviceOptions {
    void*         AdapterPtr;
    Mochi::UInt32 DeviceCreationFlags;
};
#endif // !defined(VD_EXCLUDE_D3D11_BACKEND)

class GraphicsDevice :
public Mochi::IDisposable, public std::enable_shared_from_this<GraphicsDevice> {
    using Ref = std::shared_ptr<GraphicsDevice>;

protected:
    void PostDeviceCreated();
    virtual void SubmitCommandsCore(CommandListRef commandList, FenceRef fence) = 0;

public:
    virtual void InitializeComponents() = 0;
    
    virtual std::string GetDeviceName() = 0;
    virtual std::string GetVendorName() = 0;
    virtual GraphicsApiVersionRef GetApiVersion() = 0;
    virtual GraphicsBackend GetBackendType() = 0;
    virtual Mochi::Bool IsUvOriginTopLeft() = 0;
    virtual Mochi::Bool IsDepthRangeZeroToOne() = 0;
    virtual Mochi::Bool IsClipSpaceYInverted() = 0;
    virtual ResourceFactoryRef GetResourceFactory() = 0;
    virtual GraphicsDeviceFeatures GetFeatures() = 0;

#if !defined(VD_EXCLUDE_D3D11_BACKEND)
    // MARK: Methods with DirectX 11 included
    static Ref CreateD3D11(GraphicsDeviceOptions options);
    static Ref CreateD3D11(GraphicsDeviceOptions options, 
                           SwapchainDescription description);
    static Ref CreateD3D11(GraphicsDeviceOptions options,
                           D3D11DeviceOptions d3d11options);
    static Ref CreateD3D11(GraphicsDeviceOptions options,
                           D3D11DeviceOptions d3d11options,
                           SwapchainDescription description);
    static Ref CreateD3D11(GraphicsDeviceOptions options,
                           void *hWnd, 
                           Mochi::UInt32 width,
                           Mochi::UInt32 height);
#else
    // MARK: Methods with DirectX 11 excluded (placeholders)
    static Ref CreateD3D11(GraphicsDeviceOptions options, ...);
#endif
    
#if !defined(VD_EXCLUDE_OPENGL_BACKEND)
    // MARK: Methods with OpenGL included
    static Ref CreateOpenGL(GraphicsDeviceOptions options,
                            OpenGLPlatformInfoRef info,
                            Mochi::UInt32 width,
                            Mochi::UInt32 height);
    
    static Ref CreateOpenGLES(GraphicsDeviceOptions options,
                              SwapchainDescription description);
#else
    // MARK: Methods with OpenGL excluded (placeholders)
    static Ref CreateOpenGL(GraphicsDeviceOptions options, ...);
    static Ref CreateOpenGLES(GraphicsDeviceOptions options, ...);
#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)
    
#if !defined(VD_EXCLUDE_METAL_BACKEND)
    
#if defined(__VD_TARGET_MACOS)
    typedef NSWindow* MetalContainerRef;
#else
    typedef UIView* MetalContainerRef;
#endif // defined(__VD_TARGET_MACOS)
    
    // MARK: Methods with Metal included
    static Ref CreateMetal(GraphicsDeviceOptions options);
    static Ref CreateMetal(GraphicsDeviceOptions options,
                           SwapchainDescription description);
    static Ref CreateMetal(GraphicsDeviceOptions options,
                           MetalContainerRef container);
#else
    // MARK: Methods with Metal excluded (placeholders)
    static Ref CreateMetal(GraphicsDeviceOptions options, ...);
#endif // !defined(VD_EXCLUDE_METAL_BACKEND)
};

}

#endif /* vd_hpp */
#endif
