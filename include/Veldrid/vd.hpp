//
//  vd.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#if defined(__cplusplus)
#ifndef __VD_HPP_HEADER_GUARD
#define __VD_HPP_HEADER_GUARD

#include <Mochi/Mochi.hpp>
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

enum PixelFormat {
#define __ENTRY(name) PixelFormat##name
    __ENTRY(R8_G8_B8_A8_UNorm),
#undef __ENTRY
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

enum GraphicsDeviceFeatures : Mochi::UInt32 {
#define __ENTRY(name) GDFeature##name
    __ENTRY(None)                     = 0,
    __ENTRY(ComputeShader)            = 1 << 0,
    __ENTRY(GeometryShader)           = 1 << 1,
    __ENTRY(TessellationShaders)      = 1 << 2,
    __ENTRY(MultipleViewports)        = 1 << 3,
    __ENTRY(SamplerLodBias)           = 1 << 4,
    __ENTRY(DrawBaseVertex)           = 1 << 5,
    __ENTRY(DrawBaseInstance)         = 1 << 6,
    __ENTRY(DrawIndirect)             = 1 << 7,
    __ENTRY(DrawIndirectBaseInstance) = 1 << 8,
    __ENTRY(FillModeWireframe)        = 1 << 9,
    __ENTRY(SamplerAnisotropy)        = 1 << 10,
    __ENTRY(DepthClipDisable)         = 1 << 11,
    __ENTRY(Texture1D)                = 1 << 12,
    __ENTRY(IndependentBlend)         = 1 << 13,
    __ENTRY(StructuredBuffer)         = 1 << 14,
    __ENTRY(SubsetTextureView)        = 1 << 15,
    __ENTRY(CommandListDebugMarkers)  = 1 << 16,
    __ENTRY(BufferRangeBinding)       = 1 << 17,
    __ENTRY(ShaderFloat64)            = 1 << 18
#undef __ENTRY
};

inline GraphicsDeviceFeatures operator|(GraphicsDeviceFeatures a, GraphicsDeviceFeatures b) {
    return static_cast<GraphicsDeviceFeatures>(static_cast<Mochi::UInt32>(a) |
                                               static_cast<Mochi::UInt32>(b));
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
#define __ENTRY(name) ColorWriteMask##name
    __ENTRY(None),
    __ENTRY(Red)   = 1 << 0,
    __ENTRY(Green) = 1 << 1,
    __ENTRY(Blue)  = 1 << 2,
    __ENTRY(Alpha) = 1 << 3,
    
    __ENTRY(All) = __ENTRY(Red) | __ENTRY(Green) | __ENTRY(Blue) | __ENTRY(Alpha)
#undef __ENTRY
};

inline ColorWriteMask operator|(ColorWriteMask a, ColorWriteMask b) {
    return static_cast<ColorWriteMask>(static_cast<int>(a) |
                                       static_cast<int>(b));
}

enum BlendFactor {
#define __ENTRY(name) BlendFactor##name
    __ENTRY(Zero),
    __ENTRY(One),
    __ENTRY(SourceAlpha),
    // TODO: Write remaining entries
#undef __ENTRY
};

enum BlendFunction {
#define __ENTRY(name) BlendFunction##name
    __ENTRY(Add),
    __ENTRY(Subtract),
    __ENTRY(ReverseSubtract),
    // TODO: Write remaining entries
#undef __ENTRY
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

enum ResourceKind {
#define __ENTRY(name) ResourceKind##name
    __ENTRY(UniformBuffer),
    __ENTRY(StructuredBufferReadOnly),
    __ENTRY(StructuredBufferReadWrite),
    __ENTRY(TextureReadOnly),
    __ENTRY(TextureReadWrite),
    __ENTRY(Sampler)
#undef __ENTRY
};

enum ShaderStages : Mochi::UInt8 {
#define __ENTRY(name) ShaderStage##name
    __ENTRY(None),
    __ENTRY(Vertex)                 = 1 << 0,
    __ENTRY(Geometry)               = 1 << 1,
    __ENTRY(TessellationControl)    = 1 << 2,
    __ENTRY(TessellationEvaluation) = 1 << 3,
    __ENTRY(Fragment)               = 1 << 4,
    __ENTRY(Compute)                = 1 << 5,
#undef __ENTRY
};

inline ShaderStages operator|(ShaderStages a, ShaderStages b) {
    return static_cast<ShaderStages>(static_cast<Mochi::UInt8>(a) |
                                     static_cast<Mochi::UInt8>(b));
}

enum ResourceLayoutElementOptions : int {
#define __ENTRY(name) ResourceLayoutElementOption##name
    __ENTRY(None),
    __ENTRY(DynamicBinding)         = 1 << 0
#undef __ENTRY
};

inline ResourceLayoutElementOptions operator|(ResourceLayoutElementOptions a, ResourceLayoutElementOptions b) {
    return static_cast<ResourceLayoutElementOptions>(static_cast<int>(a) |
                                                     static_cast<int>(b));
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

class GraphicsDevice :
public Mochi::IDisposable, public std::enable_shared_from_this<GraphicsDevice> {
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
    
#if !defined(VD_EXCLUDE_OPENGL_BACKEND)
    // MARK: Methods with OpenGL included
    static std::shared_ptr<GraphicsDevice> CreateOpenGL(GraphicsDeviceOptions options,
                                                        OpenGLPlatformInfoRef info,
                                                        Mochi::UInt32 width,
                                                        Mochi::UInt32 height);
    
    static std::shared_ptr<GraphicsDevice> CreateOpenGLES(GraphicsDeviceOptions options,
                                                          SwapchainDescription description);
#else
    // MARK: Methods with OpenGL excluded (placeholders)
    static std::shared_ptr<GraphicsDevice> CreateOpenGL(GraphicsDeviceOptions options, ...);
    static std::shared_ptr<GraphicsDevice> CreateOpenGLES(GraphicsDeviceOptions options, ...);
#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)
    
#if !defined(VD_EXCLUDE_METAL_BACKEND)
    
#if defined(__VD_TARGET_MACOS)
    typedef NSWindow* MetalContainerRef;
#else
    typedef UIView* MetalContainerRef;
#endif // defined(__VD_TARGET_MACOS)
    
    // MARK: Methods with Metal included
    static std::shared_ptr<GraphicsDevice> CreateMetal(GraphicsDeviceOptions options);
    static std::shared_ptr<GraphicsDevice> CreateMetal(GraphicsDeviceOptions options,
                                                       SwapchainDescription description);
    static std::shared_ptr<GraphicsDevice> CreateMetal(GraphicsDeviceOptions options,
                                                       MetalContainerRef container);
#else
    // MARK: Methods with Metal excluded (placeholders)
    static std::shared_ptr<GraphicsDevice> CreateMetal(GraphicsDeviceOptions options, ...);
#endif // !defined(VD_EXCLUDE_METAL_BACKEND)
};

}

#endif /* vd_hpp */
#endif
