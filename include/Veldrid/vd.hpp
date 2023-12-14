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

namespace Illegal {

template <class T>
std::exception Value() {
    std::string name = typeid(T).name();
    return VeldridException("Illegal " + name + " value.");
}

};

template <class T, class Base>
__MC_REF_TYPE(T) AssertSubType(__MC_REF_TYPE(Base) value) {
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
public:
    using Ref = __MC_REF_TYPE(GraphicsApiVersion);
    
    GraphicsApiVersion(int major, int minor, int subminor, int patch);
    int GetMajor() const;
    int GetMinor() const;
    int GetSubminor() const;
    int GetPatch() const;
    
    friend std::ostream& operator<<(std::ostream& os, const GraphicsApiVersion& version);
    static Mochi::Bool TryParseGLVersion(std::string versionString, GraphicsApiVersion* outVersion);
    static Ref CreateRef(int major, int minor, int subminor, int patch);

private:
    int _major;
    int _minor;
    int _subminor;
    int _patch;
};

enum class PixelFormat {
    /// RGBA component order. Each component is an 8-bit unsigned normalized integer.
    R8_G8_B8_A8_UNorm,
    /// BGRA component order. Each component is an 8-bit unsigned normalized integer.
    B8_G8_R8_A8_UNorm,
    /// Single-channel, 8-bit unsigned normalized integer.
    R8_UNorm,
    /// Single-channel, 16-bit unsigned normalized integer.
    /// Can be used as a depth format.
    R16_UNorm,
    R32_G32_B32_A32_Float,
    R32_Float,
    BC3_UNorm,
    D24_UNorm_S8_UInt,
    D32_Float_S8_UInt,
    R32_G32_B32_A32_UInt,
    R8_G8_SNorm,
    BC1_Rgb_UNorm,
    BC1_Rgba_UNorm,
    BC2_UNorm,
    R10_G10_B10_A2_UNorm,
    R10_G10_B10_A2_UInt,
    R11_G11_B10_Float,
    R8_SNorm,
    R8_UInt,
    R8_SInt,
    R16_SNorm,
    R16_UInt,
    R16_SInt,
    R16_Float,
    R32_UInt,
    R32_SInt,
    R8_G8_UNorm,
    R8_G8_UInt,
    R8_G8_SInt,
    R16_G16_UNorm,
    R16_G16_SNorm,
    R16_G16_UInt,
    R16_G16_SInt,
    R16_G16_Float,
    R32_G32_UInt,
    R32_G32_SInt,
    R32_G32_Float,
    R8_G8_B8_A8_SNorm,
    R8_G8_B8_A8_UInt,
    R8_G8_B8_A8_SInt,
    R16_G16_B16_A16_UNorm,
    R16_G16_B16_A16_SNorm,
    R16_G16_B16_A16_UInt,
    R16_G16_B16_A16_SInt,
    R16_G16_B16_A16_Float,
    R32_G32_B32_A32_SInt,
    ETC2_R8_G8_B8_UNorm,
    ETC2_R8_G8_B8_A1_UNorm,
    ETC2_R8_G8_B8_A8_UNorm,
    BC4_UNorm,
    BC4_SNorm,
    BC5_UNorm,
    BC5_SNorm,
    BC7_UNorm,
    R8_G8_B8_A8_UNorm_SRgb,
    B8_G8_R8_A8_UNorm_SRgb,
    BC1_Rgb_UNorm_SRgb,
    BC1_Rgba_UNorm_SRgb,
    BC2_UNorm_SRgb,
    BC3_UNorm_SRgb,
    BC7_UNorm_SRgb
};

Mochi::UInt32 GetSizeInBytes(PixelFormat format);
Mochi::Bool IsCompressedFormat(PixelFormat format);

// MARK: ResourceBindingModel
enum class ResourceBindingModel {
    Default, Improved
};

struct GraphicsDeviceOptions {
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
public:
    using Ref = __MC_REF_TYPE(SwapchainSource);
    
#if defined(__APPLE__)
#if defined(__VD_TARGET_MACOS)
    static Ref CreateNSWindow(void *nsWindow);
#else
    static Ref CreateUIKit(void *uiView);
#endif // defined(TARGET_OS_MAC)
#endif // defined(__APPLE__)
};

#if defined(__APPLE__)
#   if defined(__VD_TARGET_MACOS)

class NSWindowSwapchainSource : public SwapchainSource {
public:
    using Ref = __MC_REF_TYPE(NSWindowSwapchainSource);

    NSWindowSwapchainSource(NSWindow* window);
    NSWindow* GetNSWindow();

private:
    NSWindow* _window;
};

#   else

class UIViewSwapchainSource : public SwapchainSource {
public:
    using Ref = __MC_REF_TYPE(UIViewSwapchainSource);

    UIViewSwapchainSource(UIView* view);
    UIView* GetUIView();

private:
    UIView* _view;
};

#   endif // defined(__VD_TARGET_MACOS)
#endif // defined(__APPLE__)

#if defined(_WIN32)

class Win32SwapchainSource : public SwapchainSource {
public:
    using Ref = __MC_REF_TYPE(Win32SwapchainSource);

    Win32SwapchainSource(void* hwnd, void* hInstance);
    void* GetHwnd();
    void* GetHInstance();

private:
    void* _hwnd;
    void* _hInstance;
};

#endif // defined(_WIN32)

class IMappableResource {
public:
    using Ref = __MC_REF_TYPE(IMappableResource);
};

class IBindableResource {
public:
    using Ref = __MC_REF_TYPE(IBindableResource);
};

struct SwapchainDescription {
    SwapchainSource::Ref Source;
    Mochi::UInt32 Width;
    Mochi::UInt32 Height;
    std::optional<PixelFormat> DepthFormat;
    Mochi::Bool SyncToVerticalBlank;
    Mochi::Bool ColorSrgb;
};

template <typename BitType>
class Flags {
public:
    using MaskType = std::underlying_type<BitType>::type;

    Flags() : _mask(0) {}
    Flags(const BitType& value) : _mask(value) {}

    operator BitType() { return _mask; }

    inline Flags<BitType> operator|(const Flags<BitType>& other) {
        return Flags<BitType>(_mask | other._mask);
    }

    inline Flags<BitType> operator&(const Flags<BitType>& other) {
        return Flags<BitType>(_mask & other._mask);
    }

    inline Flags<BitType> operator^(const Flags<BitType>& other) {
        return Flags<BitType>(_mask ^ other._mask);
    }

    bool operator==(const Flags<BitType>& other) const {
        return _mask == other._mask;
    }

    bool operator!=(const Flags<BitType>& other) const {
        return _mask != other._mask;
    }

    Flags<BitType>& operator|=(const Flags<BitType>& other) {
        _mask |= other._mask;
        return *this;
    }

    Flags<BitType>& operator&=(const Flags<BitType>& other) {
        _mask &= other._mask;
        return *this;
    }

    Flags<BitType>& operator^=(const Flags<BitType>& other) {
        _mask ^= other._mask;
        return *this;
    }

    bool HasFlag(const BitType flag) {
        auto f = static_cast<MaskType>(flag);
        return (_mask & f) == f;
    }

private:
    MaskType _mask;
};

enum class GraphicsDeviceFeaturesBits : Mochi::UInt32 {
    None = 0,
    ComputeShader = 1 << 0,
    GeometryShader = 1 << 1,
    TessellationShaders = 1 << 2,
    MultipleViewports = 1 << 3,
    SamplerLodBias = 1 << 4,
    DrawBaseVertex = 1 << 5,
    DrawBaseInstance = 1 << 6,
    DrawIndirect = 1 << 7,
    DrawIndirectBaseInstance = 1 << 8,
    FillModeWireframe = 1 << 9,
    SamplerAnisotropy = 1 << 10,
    DepthClipDisable = 1 << 11,
    Texture1D = 1 << 12,
    IndependentBlend = 1 << 13,
    StructuredBuffer = 1 << 14,
    SubsetTextureView = 1 << 15,
    CommandListDebugMarkers = 1 << 16,
    BufferRangeBinding = 1 << 17,
    ShaderFloat64 = 1 << 18
};

using GraphicsDeviceFeatures = Flags<GraphicsDeviceFeaturesBits>;

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

class IDeviceResource {
public:
    using Ref = __MC_REF_TYPE(IDeviceResource);

    virtual std::string GetName() = 0;
    virtual void SetName(std::string name) = 0;
};

enum class BufferUsageBits : Mochi::UInt8 {
    /// Indicates that a ``vd::DeviceBuffer`` can be used as the source of vertex data for drawing commands.
    /// This flag enables the use of a ``vd::DeviceBuffer`` in the `CommandList::SetVertexBuffer()`.
    VertexBuffer = 1 << 0,
    IndexBuffer = 1 << 1,
    UniformBuffer = 1 << 2,
    StructuredBufferReadOnly = 1 << 3,
    StructuredBufferReadWrite = 1 << 4,
    IndirectBuffer = 1 << 5,
    Dynamic = 1 << 6,
    Staging = 1 << 7
};

using BufferUsage = Flags<BufferUsageBits>;

class DeviceBuffer : public IDeviceResource, public IBindableResource, public IMappableResource, public Mochi::IDisposable {
public:
    using Ref = __MC_REF_TYPE(DeviceBuffer);

    virtual Mochi::UInt32 GetSizeInBytes() = 0;
    virtual BufferUsage GetUsage() = 0;
};

struct BufferDescription {
    Mochi::UInt32 SizeInBytes;
    BufferUsage Usage;
    Mochi::UInt32 StructureByteStride;
    Mochi::Bool RawBuffer;

    BufferDescription(Mochi::UInt32 sizeInBytes, BufferUsage usage);
    BufferDescription(Mochi::UInt32 sizeInBytes, BufferUsage usage, Mochi::UInt32 structureByteStride);
    BufferDescription(Mochi::UInt32 sizeInBytes, BufferUsage usage, Mochi::UInt32 structureByteStride, Mochi::Bool rawBuffer);
    
    Mochi::Bool operator==(const BufferDescription& other);
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

struct RgbaFloat {
    float R;
    float G;
    float B;
    float A;
};

enum class ColorWriteMaskBits : int {
    None,
    Red   = 1 << 0,
    Green = 1 << 1,
    Blue  = 1 << 2,
    Alpha = 1 << 3,
    
    All = Red | Green | Blue | Alpha
};

using ColorWriteMask = Flags<ColorWriteMaskBits>;

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

enum class ShaderStagesBits : Mochi::UInt8 {
    None,
    Vertex                 = 1 << 0,
    Geometry               = 1 << 1,
    TessellationControl    = 1 << 2,
    TessellationEvaluation = 1 << 3,
    Fragment               = 1 << 4,
    Compute                = 1 << 5,
};

using ShaderStages = Flags<ShaderStagesBits>;

enum class ResourceLayoutElementOptionsBits : int {
    None,
    DynamicBinding         = 1 << 0
};

using ResourceLayoutElementOptions = Flags<ResourceLayoutElementOptionsBits>;

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
public:
    using Ref = __MC_REF_TYPE(ResourceLayout);

private:
#if defined(VD_VALIDATE_USAGE)
    ResourceLayoutDescription _description;
    Mochi::UInt32 _dynamicBufferCount;
#endif // defined(VD_VALIDATE_USAGE)
    
protected:
    ResourceLayout(ResourceLayoutDescription& description);
};

enum class VertexElementSemantic {
    Position,
    Normal,
    TextureCoordinate,
    Color
};

enum class VertexElementFormat {
    Float1,
    Float2,
    Float3,
    Float4,
    Byte2_Norm,
    Byte2,
    Byte4_Norm,
    Byte4,
    SByte2_Norm,
    SByte2,
    SByte4_Norm,
    SByte4,
    UShort2_Norm,
    UShort2,
    UShort4_Norm,
    UShort4,
    Short2_Norm,
    Short2,
    Short4_Norm,
    Short4,
    UInt1,
    UInt2,
    UInt3,
    UInt4,
    Int1,
    Int2,
    Int3,
    Int4,
    Half1,
    Half2,
    Half4
};

Mochi::UInt32 GetSizeInBytes(VertexElementFormat format);

struct VertexElementDescription {
    std::string Name;
    VertexElementSemantic Semantic;
    VertexElementFormat Format;
    Mochi::UInt32 Offset;

    Mochi::Bool operator==(const VertexElementDescription& other);
};

struct VertexLayoutDescription {
    Mochi::UInt32 Stride;
    std::vector<VertexElementDescription> Elements;
    Mochi::UInt32 InstanceStepRate;

    Mochi::Bool operator==(const VertexLayoutDescription& other);
};

struct OutputDescription;

struct GraphicsPipelineDescription {
    BlendStateDescription BlendState;
    OutputDescription Outputs;
    std::optional<ResourceBindingModel> ResourceBindingModel;
    std::vector<ResourceLayout::Ref> ResourceLayouts;
};

/// A device resource encapsulating all state in a graphics pipeline.
/// Used in `CommandList::SetPipeline()` to prepare a `CommandList` for draw commands.
/// See `GraphicsPipelineDescription`.
class Pipeline : public IDeviceResource, public Mochi::IDisposable {
public:
    using Ref = __MC_REF_TYPE(Pipeline);

    Pipeline(GraphicsPipelineDescription& description);
    Pipeline(std::vector<ResourceLayout::Ref> resourceLayouts);
    
    /// Gets a value indicating whether this instance represents a compute ``vd::Pipeline``.
    /// If `false`, this instance is a graphics pipeline.
    virtual Mochi::Bool IsComputePipeline() = 0;

    /// @brief A `bool` indicating whether this instance has been disposed.
    virtual Mochi::Bool IsDisposed() = 0;

    /// Frees device resources controlled by this instance.
    virtual void Dispose() override = 0;
    
#if defined(VD_VALIDATE_USAGE)
    OutputDescription GetGraphicsOutputDescription();
    std::vector<ResourceLayoutRef> GetResourceLayouts();
#endif

private:
#if defined(VD_VALIDATE_USAGE)
    OutputDescription _graphicsOutputDescription;
    std::vector<ResourceLayoutRef> _resourceLayouts;
#endif
};

class Framebuffer;
__MC_DEFINE_REF_TYPE(Framebuffer)

struct OutputDescription {
    std::optional<OutputAttachmentDescription> DepthAttachment;
    std::vector<OutputAttachmentDescription> ColorAttachments;
    TextureSampleCount SampleCount;

    static OutputDescription CreateFromFramebuffer(FramebufferRef fb);
};

class Texture;

struct TextureViewDescription {
    Texture::Ref Target;
    Mochi::UInt32 BaseMipLevel;
    Mochi::UInt32 MipLevels;
    Mochi::UInt32 BaseArrayLayer;
    Mochi::UInt32 ArrayLayers;
    std::optional<PixelFormat> Format;
    
    TextureViewDescription(Texture::Ref target);
    TextureViewDescription(Texture::Ref target, PixelFormat format);
    TextureViewDescription(Texture::Ref target, Mochi::UInt32 baseMipLevel, Mochi::UInt32 mipLevels, Mochi::UInt32 baseArrayLayer, Mochi::UInt32 arrayLayers);
    TextureViewDescription(Texture::Ref target, PixelFormat format, Mochi::UInt32 baseMipLevel, Mochi::UInt32 mipLevels, Mochi::UInt32 baseArrayLayer, Mochi::UInt32 arrayLayers);
    
    Mochi::Bool operator=(const TextureViewDescription& other);
};

class TextureView : public IBindableResource, public IDeviceResource, public Mochi::IDisposable {
public:
    using Ref = __MC_REF_TYPE(TextureView);

    Texture::Ref  GetTarget();
    Mochi::UInt32 GetBaseMipLevel();
    Mochi::UInt32 GetMipLevels();
    Mochi::UInt32 GetBaseArrayLayer();
    Mochi::UInt32 GetArrayLayers();
    PixelFormat   GetFormat();
    
    virtual std::string GetName() override = 0;
    virtual void SetName(std::string name) override = 0;
    virtual Mochi::Bool IsDisposed() = 0;
    virtual void Dispose() override = 0;

private:
    Texture::Ref _target;

protected:
    TextureView(TextureViewDescription& description);

};


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

inline void operator|=(TextureUsage& a, TextureUsage b) {
    a = a | b;
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

class Texture : public IDeviceResource, public IMappableResource, public Mochi::IDisposable, public IBindableResource, public std::enable_shared_from_this<Texture> {
public:
    using Ref = __MC_REF_TYPE(Texture);

    Mochi::UInt32 CalculateSubresource(Mochi::UInt32 mipLevel, Mochi::UInt32 arrayLayer);
    TextureView::Ref GetFullTextureView(GraphicsDevice::Ref gd);
    
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

private:
    std::mutex     _fullTextureViewLock;
    TextureView::Ref _fullTextureView;

protected:
    TextureView::Ref CreateFullTextureView(GraphicsDevice::Ref gd);
    virtual void DisposeCore() = 0;
};

struct FramebufferAttachment {
    Texture::Ref Target;
    Mochi::UInt32 ArrayLayer;
    Mochi::UInt32 MipLevel;
    
    FramebufferAttachment(Texture::Ref target, Mochi::UInt32 arrayLayer);
    FramebufferAttachment(Texture::Ref target, Mochi::UInt32 arrayLayer, Mochi::UInt32 mipLevel);
};

struct FramebufferAttachmentDescription {
    Texture::Ref Target;
    Mochi::UInt32 ArrayLayer;
    Mochi::UInt32 MipLevel;
    
    FramebufferAttachmentDescription(Texture::Ref target, Mochi::UInt32 arrayLayer);
    FramebufferAttachmentDescription(Texture::Ref target, Mochi::UInt32 arrayLayer, Mochi::UInt32 mipLevel);
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
    virtual TextureView::Ref CreateTextureViewCore(TextureViewDescription& description) = 0;
    virtual Pipeline::Ref    CreateGraphicsPipelineCore(GraphicsPipelineDescription& description) = 0;
    
public:
    virtual GraphicsBackend GetBackendType() = 0;
    GraphicsDeviceFeatures GetFeatures();
    
    TextureView::Ref CreateTextureView(Texture::Ref target);
    TextureView::Ref CreateTextureView(TextureViewDescription& description);
    Pipeline::Ref    CreateGraphicsPipeline(GraphicsPipelineDescription& description);
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

#if !defined(VD_EXCLUDE_VK_BACKEND)
struct VulkanDeviceOptions {
    std::vector<std::string> InstanceExtensions;
    std::vector<std::string> DeviceExtensions;
};
#endif // !defined(VD_EXCLUDE_VK_BACKEND)

class GraphicsDevice :
public Mochi::IDisposable, public std::enable_shared_from_this<GraphicsDevice> {
public:
    using Ref = std::shared_ptr<GraphicsDevice>;
    virtual void InitializeComponents() = 0;
    
    virtual std::string GetDeviceName() = 0;
    virtual std::string GetVendorName() = 0;
    virtual GraphicsApiVersion::Ref GetApiVersion() = 0;
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

protected:
    void PostDeviceCreated();
    virtual void SubmitCommandsCore(CommandListRef commandList, FenceRef fence) = 0;
};

}

#endif /* vd_hpp */
#endif
