//
//  vd.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#if defined(__cplusplus)
#ifndef __VD_HPP_HEADER_GUARD
#define __VD_HPP_HEADER_GUARD

#include <Mochi/foundation.hpp>
#include <vector>
#include <Veldrid/core.hpp>
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

namespace __VD_NAMESPACE {
    [[noreturn]] void ThrowNotImplemented(const std::source_location loc = std::source_location::current());

    using IDisposable = __MC_NAMESPACE::IDisposable;

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

    template <class TSrc, class TDst>
    Bool TryCastRef(const Handle<TSrc> obj, Handle<TDst>& out) {
        return __MC_NAMESPACE::TryCastRef(obj, out);
    }

    template <class TDst, class TSrc>
    Handle<TDst> TryCastRef(const Handle<TSrc> obj) {
        return __MC_NAMESPACE::TryCastRef<TDst>(obj);
    }

    template <class TDst, class TSrc>
    Handle<TDst> CastRef(const Handle<TSrc> obj) {
        return __MC_NAMESPACE::CastRef<TDst>(obj);
    }
    
    template <typename T, typename... TArgs>
    Handle<T> CreateRef(TArgs... args) {
        return __MC_NAMESPACE::CreateRef<T>(args...);
    }

    template <typename TBase, typename T> requires IsDerived<T, std::enable_shared_from_this<TBase>>
    Handle<T> GetRef(T* obj) {
        return __MC_NAMESPACE::GetRef<T>(obj);
    }

    template <class T, class TBase> requires IsDerived<T, TBase>
    Handle<T> AssertSubType(Handle<TBase> value) {
        std::stringstream str;
        
        if (!value) {
            str << "Expected object of type " << typeid(T).name() << " but received null instead.";
            throw VeldridException(str.str());
        }
        
        auto result = std::dynamic_pointer_cast<T>(value);
        if (!result) {
            str << "Object " << value << " (typeof " << typeid(TBase).name() << ")";
            str << " must be derived type " << typeid(T).name() << " to be used in this context.";
            throw VeldridException(str.str());
        }
        
        return result;
    }

    struct GraphicsApiVersion {
    public:
        using Ref = __VD_NAMESPACE::Handle<GraphicsApiVersion>;
        
        GraphicsApiVersion(int major, int minor, int subminor, int patch);
        int GetMajor() const;
        int GetMinor() const;
        int GetSubminor() const;
        int GetPatch() const;
        
        friend std::ostream& operator<<(std::ostream& os, const GraphicsApiVersion& version);
        static Bool TryParseGLVersion(std::string versionString, GraphicsApiVersion* outVersion);

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

    UInt32 GetSizeInBytes(PixelFormat format);
    Bool IsCompressedFormat(PixelFormat format);

    // MARK: ResourceBindingModel
    enum class ResourceBindingModel {
        Default, Improved
    };

    struct GraphicsDeviceOptions {
        Bool Debug;
        Bool HasMainSwapchain;
        std::optional<PixelFormat> SwapchainDepthFormat;
        Bool SyncToVerticalBlank;
        ResourceBindingModel ResourceBindingModel;
        Bool PreferDepthRangeZeroToOne;
        Bool PreferStandardClipSpaceYDirection;
        Bool SwapchainSrgbFormat;
    };

    // __MC_DEFINE_REF_TYPE(GraphicsDeviceOptions)

    class SwapchainSource {
    public:
        using Ref = __VD_NAMESPACE::Handle<SwapchainSource>;
        
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
        using Ref = __VD_NAMESPACE::Handle<NSWindowSwapchainSource>;

        NSWindowSwapchainSource(NSWindow* window);
        NSWindow* GetNSWindow();

    private:
        NSWindow* _window;
    };

    #   else

    class UIViewSwapchainSource : public SwapchainSource {
    public:
        using Ref = __VD_NAMESPACE::Handle<UIViewSwapchainSource>;

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
        using Ref = __VD_NAMESPACE::Handle<Win32SwapchainSource>;

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
        using Ref = __VD_NAMESPACE::Handle<IMappableResource>;
    };

    class IBindableResource {
    public:
        using Ref = __VD_NAMESPACE::Handle<IBindableResource>;
    };

    struct SwapchainDescription {
        SwapchainSource::Ref Source;
        UInt32 Width;
        UInt32 Height;
        std::optional<PixelFormat> DepthFormat;
        Bool SyncToVerticalBlank;
        Bool ColorSrgb;
    };

    template <typename BitType>
    class Flags {
    public:
        using MaskType = std::underlying_type<BitType>::type;

        Flags() : _mask(0) {}
        Flags(const BitType& value) : _mask((MaskType) value) {}
        Flags(const MaskType& value) : _mask(value) {}

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

    enum class GraphicsDeviceFeaturesBits : UInt32 {
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
        Bool operator==(const OutputAttachmentDescription& other);
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
        using Ref = __VD_NAMESPACE::Handle<IDeviceResource>;

        virtual std::string GetName() = 0;
        virtual void SetName(std::string name) = 0;
    };

    enum class BufferUsageBits : UInt8 {
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

    class DeviceBuffer : public IDeviceResource, public IBindableResource, public IMappableResource, public IDisposable {
    public:
        using Ref = __VD_NAMESPACE::Handle<DeviceBuffer>;

        virtual UInt32 GetSizeInBytes() = 0;
        virtual BufferUsage GetUsage() = 0;
    };

    struct BufferDescription {
        UInt32 SizeInBytes;
        BufferUsage Usage;
        UInt32 StructureByteStride;
        Bool RawBuffer;

        BufferDescription(UInt32 sizeInBytes, BufferUsage usage);
        BufferDescription(UInt32 sizeInBytes, BufferUsage usage, UInt32 structureByteStride);
        BufferDescription(UInt32 sizeInBytes, BufferUsage usage, UInt32 structureByteStride, Bool rawBuffer);
        
        Bool operator==(const BufferDescription& other);
    };

    class Fence : public IDeviceResource, public IDisposable {
    public:
        using Ref = __VD_NAMESPACE::Handle<Fence>;

        virtual Bool IsSignaled() = 0;
        virtual void Reset() = 0;
        virtual std::string GetName() override = 0;
        virtual void SetName(std::string name) override = 0;
        virtual Bool IsDisposed() = 0;
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
        Bool BlendEnabled;
        std::optional<ColorWriteMask> ColorWriteMask;
        
        // FIXME: Refactor these into a smaller struct
        
        BlendFactor SourceColorFactor;
        BlendFactor DestinationColorFactor;
        BlendFunction ColorFunction;
        
        BlendFactor SourceAlphaFactor;
        BlendFactor DestinationAlphaFactor;
        BlendFunction AlphaFunction;
        
        Bool operator==(const BlendAttachmentDescription& other);
        Bool operator!=(const BlendAttachmentDescription& other);
    };

    struct BlendStateDescription {
        RgbaFloat BlendFactor;
        std::vector<BlendAttachmentDescription> AttachmentStates;
        Bool AlphaToCoverageEnabled;
    };

    enum class ResourceKind {
        UniformBuffer,
        StructuredBufferReadOnly,
        StructuredBufferReadWrite,
        TextureReadOnly,
        TextureReadWrite,
        Sampler
    };

    enum class ShaderStagesBits : UInt8 {
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
        Bool operator==(const ResourceLayoutElementDescription& other) const;
    };

    struct ResourceLayoutDescription {
        std::vector<ResourceLayoutElementDescription> Elements;
        Bool operator==(const ResourceLayoutDescription& other) const;
    };

    class ResourceLayout : public IDeviceResource, public IDisposable {
    public:
        using Ref = __VD_NAMESPACE::Handle<ResourceLayout>;

    private:
    #if defined(VD_VALIDATE_USAGE)
        ResourceLayoutDescription _description;
        UInt32 _dynamicBufferCount;
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

    UInt32 GetSizeInBytes(VertexElementFormat format);

    struct VertexElementDescription {
        std::string Name;
        VertexElementSemantic Semantic;
        VertexElementFormat Format;
        UInt32 Offset;

        friend Bool operator==(const VertexElementDescription& a, const VertexElementDescription& b);
    };

    struct VertexLayoutDescription {
        UInt32 Stride;
        std::vector<VertexElementDescription> Elements;
        UInt32 InstanceStepRate;

        friend Bool operator==(const VertexLayoutDescription& a, const VertexLayoutDescription& b);
    };

    class Framebuffer;

    struct OutputDescription {
        std::optional<OutputAttachmentDescription> DepthAttachment;
        std::vector<OutputAttachmentDescription> ColorAttachments;
        TextureSampleCount SampleCount;

        static OutputDescription CreateFromFramebuffer(__VD_NAMESPACE::Handle<Framebuffer> fb);
    };

    struct GraphicsPipelineDescription {
        BlendStateDescription BlendState;
        OutputDescription Outputs;
        std::optional<ResourceBindingModel> ResourceBindingModel;
        std::vector<ResourceLayout::Ref> ResourceLayouts;
    };

    /// A device resource encapsulating all state in a graphics pipeline.
    /// Used in `CommandList::SetPipeline()` to prepare a `CommandList` for draw commands.
    /// See `GraphicsPipelineDescription`.
    class Pipeline : public IDeviceResource, public IDisposable {
    public:
        using Ref = __VD_NAMESPACE::Handle<Pipeline>;

        Pipeline(GraphicsPipelineDescription& description);
        Pipeline(std::vector<ResourceLayout::Ref> resourceLayouts);
        
        /// Gets a value indicating whether this instance represents a compute ``vd::Pipeline``.
        /// If `false`, this instance is a graphics pipeline.
        virtual Bool IsComputePipeline() = 0;

        /// @brief A `bool` indicating whether this instance has been disposed.
        virtual Bool IsDisposed() = 0;

        /// Frees device resources controlled by this instance.
        virtual void Dispose() override = 0;
        
    #if defined(VD_VALIDATE_USAGE)
        OutputDescription GetGraphicsOutputDescription();
        std::vector<ResourceLayout::Ref> GetResourceLayouts();
    #endif

    private:
    #if defined(VD_VALIDATE_USAGE)
        OutputDescription _graphicsOutputDescription;
        std::vector<ResourceLayout::Ref> _resourceLayouts;
    #endif
    };

    class Texture;

    struct TextureViewDescription {
        Handle<Texture> Target;
        UInt32 BaseMipLevel;
        UInt32 MipLevels;
        UInt32 BaseArrayLayer;
        UInt32 ArrayLayers;
        std::optional<PixelFormat> Format;
        
        TextureViewDescription(Handle<Texture> target);
        TextureViewDescription(Handle<Texture> target, PixelFormat format);
        TextureViewDescription(Handle<Texture> target, UInt32 baseMipLevel, UInt32 mipLevels, UInt32 baseArrayLayer, UInt32 arrayLayers);
        TextureViewDescription(Handle<Texture> target, PixelFormat format, UInt32 baseMipLevel, UInt32 mipLevels, UInt32 baseArrayLayer, UInt32 arrayLayers);
        
        Bool operator=(const TextureViewDescription& other);
    };

    class TextureView : public IBindableResource, public IDeviceResource, public IDisposable {
    public:
        using Ref = __VD_NAMESPACE::Handle<TextureView>;

        __VD_NAMESPACE::Handle<Texture>  GetTarget();
        UInt32 GetBaseMipLevel();
        UInt32 GetMipLevels();
        UInt32 GetBaseArrayLayer();
        UInt32 GetArrayLayers();
        PixelFormat   GetFormat();
        
        virtual std::string GetName() override = 0;
        virtual void SetName(std::string name) override = 0;
        virtual Bool IsDisposed() = 0;
        virtual void Dispose() override = 0;

    private:
        __VD_NAMESPACE::Handle<Texture> _target;

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

    inline Bool operator&(TextureUsage a, TextureUsage b) {
        return (static_cast<char>(a) & static_cast<char>(b)) != 0;
    }

    enum class TextureType {
        Texture1D,
        Texture2D,
        Texture3D
    };

    struct TextureDescription {
        UInt32 Width;
        UInt32 Height;
        UInt32 Depth;
        UInt32 MipLevels;
        UInt32 ArrayLayers;
        PixelFormat Format;
        TextureUsage Usage;
        TextureType Type;
        TextureSampleCount SampleCount;
        
        static TextureDescription Texture1D(UInt32 width, UInt32 mipLevels, UInt32 arrayLayers, PixelFormat format, TextureUsage usage);
        static TextureDescription Texture2D(UInt32 width, UInt32 height, UInt32 mipLevels, UInt32 arrayLayers, PixelFormat format, TextureUsage usage);
        static TextureDescription Texture2D(UInt32 width, UInt32 height, UInt32 mipLevels, UInt32 arrayLayers, PixelFormat format, TextureUsage usage, TextureSampleCount sampleCount);
        static TextureDescription Texture3D(UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, PixelFormat format, TextureUsage usage);
    };

    class GraphicsDevice;

    class Texture : public IDeviceResource, public IMappableResource, public IDisposable, public IBindableResource, public std::enable_shared_from_this<Texture> {
    public:
        using Ref = __VD_NAMESPACE::Handle<Texture>;

        UInt32 CalculateSubresource(UInt32 mipLevel, UInt32 arrayLayer);
        TextureView::Ref GetFullTextureView(__VD_NAMESPACE::Handle<GraphicsDevice> gd);
        
        virtual PixelFormat        GetFormat()      = 0;
        virtual UInt32      GetWidth()       = 0;
        virtual UInt32      GetHeight()      = 0;
        virtual UInt32      GetDepth()       = 0;
        virtual UInt32      GetMipLevels()   = 0;
        virtual UInt32      GetArrayLayers() = 0;
        virtual TextureUsage       GetUsage()       = 0;
        virtual TextureType        GetType()        = 0;
        virtual TextureSampleCount GetSampleCount() = 0;
        
        virtual std::string GetName() override = 0;
        virtual void SetName(std::string name) override = 0;
        virtual Bool IsDisposed() = 0;
        virtual void Dispose() override;

    private:
        std::mutex     _fullTextureViewLock;
        TextureView::Ref _fullTextureView;

    protected:
        TextureView::Ref CreateFullTextureView(__VD_NAMESPACE::Handle<GraphicsDevice> gd);
        virtual void DisposeCore() = 0;
    };

    struct FramebufferAttachment {
        Texture::Ref Target;
        UInt32 ArrayLayer;
        UInt32 MipLevel;
        
        FramebufferAttachment(Texture::Ref target, UInt32 arrayLayer);
        FramebufferAttachment(Texture::Ref target, UInt32 arrayLayer, UInt32 mipLevel);
    };

    struct FramebufferAttachmentDescription {
        Texture::Ref Target;
        UInt32 ArrayLayer;
        UInt32 MipLevel;
        
        FramebufferAttachmentDescription(Texture::Ref target, UInt32 arrayLayer);
        FramebufferAttachmentDescription(Texture::Ref target, UInt32 arrayLayer, UInt32 mipLevel);
    };

    struct FramebufferDescription {
        std::optional<FramebufferAttachmentDescription> DepthTarget;
        std::vector<FramebufferAttachmentDescription> ColorTargets;
    };

    class Framebuffer : public IDeviceResource, public IDisposable, public std::enable_shared_from_this<Framebuffer> {
    public:
        using Ref = __VD_NAMESPACE::Handle<Framebuffer>;

        virtual std::optional<FramebufferAttachment> GetDepthTarget();
        virtual std::vector<FramebufferAttachment> GetColorTargets();
        virtual OutputDescription GetOutputDescription();

        virtual std::string GetName() override = 0;
        virtual void SetName(std::string name) override = 0;
        virtual Bool IsDisposed() = 0;
        virtual void Dispose() override = 0;

    private:
        std::optional<FramebufferAttachment> _depthTarget;
        std::vector<FramebufferAttachment> _colorTargets;
        OutputDescription _outputDescription;
        
    protected:
        Framebuffer();
        Framebuffer(std::optional<FramebufferAttachmentDescription> depthTarget,
                    std::vector<FramebufferAttachmentDescription> colorTargets);
        void InitializeComponent();
    };

    class CommandList : public IDeviceResource, public IDisposable {
    public:
        using Ref = __VD_NAMESPACE::Handle<CommandList>;

    private:
        GraphicsDeviceFeatures _features;
        UInt32 _uniformBufferAlignment;
        UInt32 _structuredBufferAlignment;
        
    protected:
        
    };

    class ResourceFactory {
    public:
        using Ref = __VD_NAMESPACE::Handle<ResourceFactory>;

        virtual GraphicsBackend GetBackendType() = 0;
        GraphicsDeviceFeatures GetFeatures();

        TextureView::Ref CreateTextureView(Texture::Ref target);
        TextureView::Ref CreateTextureView(TextureViewDescription& description);
        Pipeline::Ref    CreateGraphicsPipeline(GraphicsPipelineDescription& description);

    private:
        GraphicsDeviceFeatures _features;
        
    protected:
        ResourceFactory(GraphicsDeviceFeatures features);
        virtual TextureView::Ref CreateTextureViewCore(TextureViewDescription& description) = 0;
        virtual Pipeline::Ref    CreateGraphicsPipelineCore(GraphicsPipelineDescription& description) = 0;
    };

    #if !defined(VD_EXCLUDE_OPENGL_BACKEND)
    class OpenGLPlatformInfo;
    #endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)

    #if !defined(VD_EXCLUDE_D3D11_BACKEND)
    struct D3D11DeviceOptions {
        void*         AdapterPtr;
        UInt32 DeviceCreationFlags;
    };
    #endif // !defined(VD_EXCLUDE_D3D11_BACKEND)

    #if !defined(VD_EXCLUDE_VK_BACKEND)
    struct VulkanDeviceOptions {
        std::vector<std::string> InstanceExtensions;
        std::vector<std::string> DeviceExtensions;
    };
    #endif // !defined(VD_EXCLUDE_VK_BACKEND)

    class GraphicsDevice :
    public IDisposable, public std::enable_shared_from_this<GraphicsDevice> {
    public:
        using Ref = __VD_NAMESPACE::Handle<GraphicsDevice>;
        
        virtual std::string GetDeviceName() = 0;
        virtual std::string GetVendorName() = 0;
        virtual GraphicsApiVersion::Ref GetApiVersion() = 0;
        virtual GraphicsBackend GetBackendType() = 0;
        virtual Bool IsUvOriginTopLeft() = 0;
        virtual Bool IsDepthRangeZeroToOne() = 0;
        virtual Bool IsClipSpaceYInverted() = 0;
        virtual ResourceFactory::Ref GetResourceFactory() = 0;
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
                            UInt32 width,
                            UInt32 height);
    #else
        // MARK: Methods with DirectX 11 excluded (placeholders)
        static Ref CreateD3D11(GraphicsDeviceOptions options, ...);
    #endif
        
    #if !defined(VD_EXCLUDE_OPENGL_BACKEND)
        // MARK: Methods with OpenGL included
        static Ref CreateOpenGL(GraphicsDeviceOptions options,
                                __VD_NAMESPACE::Handle<OpenGLPlatformInfo> info,
                                UInt32 width,
                                UInt32 height);
        
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
        virtual void SubmitCommandsCore(CommandList::Ref commandList, Fence::Ref fence) = 0;
    };

}

#endif /* vd_hpp */
#endif
