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

enum TextureSampleCount {
#define __ENTRY(count) TextureSampleCount##count = count
    __ENTRY(1),
    __ENTRY(2),
    __ENTRY(4),
    __ENTRY(8),
    __ENTRY(16),
    __ENTRY(32)
#undef __ENTRY
};

struct OutputDescription {
    std::optional<OutputAttachmentDescription> DepthAttachment;
    std::vector<OutputAttachmentDescription> ColorAttachments;
    TextureSampleCount SampleCount;
};

class IDeviceResource {
public:
    virtual std::string GetName() = 0;
    virtual void SetName(std::string name) = 0;
};

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

class ResourceFactory {
private:
    GraphicsDeviceFeatures _features;
    
protected:
    ResourceFactory(GraphicsDeviceFeatures features);
    virtual PipelineRef CreateGraphicsPipelineCore(GraphicsPipelineDescription& description) = 0;
    
public:
    virtual GraphicsBackend GetBackendType() = 0;
    GraphicsDeviceFeatures GetFeatures();
    
    PipelineRef CreateGraphicsPipeline(GraphicsPipelineDescription& description);
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
