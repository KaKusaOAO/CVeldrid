#include <Veldrid/backend_d3d11.hpp>

#if !defined(VD_EXCLUDE_D3D11_BACKEND)

#include <iomanip>

namespace vd {

    DXGI_FORMAT D3D11Formats::ToDxgiFormat(PixelFormat format, Bool depthFormat) {
        switch (format) {
        case PixelFormat::R8_UNorm:
            return DXGI_FORMAT_R8_UNORM;
        case PixelFormat::R8_SNorm:
            return DXGI_FORMAT_R8_SNORM;
        case PixelFormat::R8_UInt:
            return DXGI_FORMAT_R8_UINT;
        case PixelFormat::R8_SInt:
            return DXGI_FORMAT_R8_SINT;

        case PixelFormat::R16_UNorm:
            return depthFormat ? DXGI_FORMAT_R16_TYPELESS : DXGI_FORMAT_R16_UNORM;
        case PixelFormat::R16_SNorm:
            return DXGI_FORMAT_R16_SNORM;
        case PixelFormat::R16_UInt:
            return DXGI_FORMAT_R16_UINT;
        case PixelFormat::R16_SInt:
            return DXGI_FORMAT_R16_SINT;
        case PixelFormat::R16_Float:
            return DXGI_FORMAT_R16_FLOAT;

        case PixelFormat::R32_UInt:
            return DXGI_FORMAT_R32_UINT;
        case PixelFormat::R32_SInt:
            return DXGI_FORMAT_R32_SINT;
        case PixelFormat::R32_Float:
            return depthFormat ? DXGI_FORMAT_R32_TYPELESS : DXGI_FORMAT_R32_FLOAT;

        case PixelFormat::R8_G8_UNorm:
            return DXGI_FORMAT_R8G8_UNORM;
        case PixelFormat::R8_G8_SNorm:
            return DXGI_FORMAT_R8G8_SNORM;
        case PixelFormat::R8_G8_UInt:
            return DXGI_FORMAT_R8G8_UINT;
        case PixelFormat::R8_G8_SInt:
            return DXGI_FORMAT_R8G8_SINT;

        case PixelFormat::R16_G16_UNorm:
            return DXGI_FORMAT_R16G16_UNORM;
        case PixelFormat::R16_G16_SNorm:
            return DXGI_FORMAT_R16G16_SNORM;
        case PixelFormat::R16_G16_UInt:
            return DXGI_FORMAT_R16G16_UINT;
        case PixelFormat::R16_G16_SInt:
            return DXGI_FORMAT_R16G16_SINT;
        case PixelFormat::R16_G16_Float:
            return DXGI_FORMAT_R16G16_FLOAT;

        case PixelFormat::R32_G32_UInt:
            return DXGI_FORMAT_R32G32_UINT;
        case PixelFormat::R32_G32_SInt:
            return DXGI_FORMAT_R32G32_SINT;
        case PixelFormat::R32_G32_Float:
            return DXGI_FORMAT_R32G32_FLOAT;

        case PixelFormat::R8_G8_B8_A8_UNorm:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case PixelFormat::R8_G8_B8_A8_UNorm_SRgb:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case PixelFormat::B8_G8_R8_A8_UNorm:
            return DXGI_FORMAT_B8G8R8A8_UNORM;
        case PixelFormat::B8_G8_R8_A8_UNorm_SRgb:
            return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        case PixelFormat::R8_G8_B8_A8_SNorm:
            return DXGI_FORMAT_R8G8B8A8_SNORM;
        case PixelFormat::R8_G8_B8_A8_UInt:
            return DXGI_FORMAT_R8G8B8A8_UINT;
        case PixelFormat::R8_G8_B8_A8_SInt:
            return DXGI_FORMAT_R8G8B8A8_SINT;

        case PixelFormat::R16_G16_B16_A16_UNorm:
            return DXGI_FORMAT_R16G16B16A16_UNORM;
        case PixelFormat::R16_G16_B16_A16_SNorm:
            return DXGI_FORMAT_R16G16B16A16_SNORM;
        case PixelFormat::R16_G16_B16_A16_UInt:
            return DXGI_FORMAT_R16G16B16A16_UINT;
        case PixelFormat::R16_G16_B16_A16_SInt:
            return DXGI_FORMAT_R16G16B16A16_SINT;
        case PixelFormat::R16_G16_B16_A16_Float:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;

        case PixelFormat::R32_G32_B32_A32_UInt:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        case PixelFormat::R32_G32_B32_A32_SInt:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        case PixelFormat::R32_G32_B32_A32_Float:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;

        case PixelFormat::BC1_Rgb_UNorm:
        case PixelFormat::BC1_Rgba_UNorm:
            return DXGI_FORMAT_BC1_UNORM;
        case PixelFormat::BC1_Rgb_UNorm_SRgb:
        case PixelFormat::BC1_Rgba_UNorm_SRgb:
            return DXGI_FORMAT_BC1_UNORM_SRGB;
        case PixelFormat::BC2_UNorm:
            return DXGI_FORMAT_BC2_UNORM;
        case PixelFormat::BC2_UNorm_SRgb:
            return DXGI_FORMAT_BC2_UNORM_SRGB;
        case PixelFormat::BC3_UNorm:
            return DXGI_FORMAT_BC3_UNORM;
        case PixelFormat::BC3_UNorm_SRgb:
            return DXGI_FORMAT_BC3_UNORM_SRGB;
        case PixelFormat::BC4_UNorm:
            return DXGI_FORMAT_BC4_UNORM;
        case PixelFormat::BC4_SNorm:
            return DXGI_FORMAT_BC4_SNORM;
        case PixelFormat::BC5_UNorm:
            return DXGI_FORMAT_BC5_UNORM;
        case PixelFormat::BC5_SNorm:
            return DXGI_FORMAT_BC5_SNORM;
        case PixelFormat::BC7_UNorm:
            return DXGI_FORMAT_BC7_UNORM;
        case PixelFormat::BC7_UNorm_SRgb:
            return DXGI_FORMAT_BC7_UNORM_SRGB;

        case PixelFormat::D24_UNorm_S8_UInt:
            if (!depthFormat) {
                throw VeldridException("depthFormat must be set to true in order to use PixelFormat::D24_UNorm_S8_UInt");
            }
            return DXGI_FORMAT_R24G8_TYPELESS;
        case PixelFormat::D32_Float_S8_UInt:
            if (!depthFormat) {
                throw VeldridException("depthFormat must be set to true in order to use PixelFormat::D32_Float_S8_UInt");
            }
            return DXGI_FORMAT_R32G8X24_TYPELESS;

        case PixelFormat::R10_G10_B10_A2_UNorm:
            return DXGI_FORMAT_R10G10B10A2_UNORM;
        case PixelFormat::R10_G10_B10_A2_UInt:
            return DXGI_FORMAT_R10G10B10A2_UINT;
        case PixelFormat::R11_G11_B10_Float:
            return DXGI_FORMAT_R11G11B10_FLOAT;

        case PixelFormat::ETC2_R8_G8_B8_UNorm:
        case PixelFormat::ETC2_R8_G8_B8_A1_UNorm:
        case PixelFormat::ETC2_R8_G8_B8_A8_UNorm:
            throw VeldridException("ETC2 formats are not supported on Direct3D 11");
        }

        throw Illegal::Value<PixelFormat>();
    }

    DXGI_FORMAT D3D11Formats::GetTypelessFormat(DXGI_FORMAT format) {
        switch (format) {
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
            return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        case DXGI_FORMAT_R32G32B32_TYPELESS:
        case DXGI_FORMAT_R32G32B32_FLOAT:
        case DXGI_FORMAT_R32G32B32_UINT:
        case DXGI_FORMAT_R32G32B32_SINT:
            return DXGI_FORMAT_R32G32B32_TYPELESS;
        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_SINT:
            return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        case DXGI_FORMAT_R32G32_TYPELESS:
        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
            return DXGI_FORMAT_R32G32_TYPELESS;
        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
            return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_SINT:
            return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        case DXGI_FORMAT_R32_TYPELESS:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
            return DXGI_FORMAT_R32_TYPELESS;
        case DXGI_FORMAT_R24G8_TYPELESS:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
        case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
            return DXGI_FORMAT_R24G8_TYPELESS;
        case DXGI_FORMAT_R8G8_TYPELESS:
        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_SINT:
            return DXGI_FORMAT_R8G8_TYPELESS;
        case DXGI_FORMAT_R16_TYPELESS:
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_D16_UNORM:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R16_SINT:
            return DXGI_FORMAT_R16_TYPELESS;
        case DXGI_FORMAT_R8_TYPELESS:
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_R8_SINT:
        case DXGI_FORMAT_A8_UNORM:
            return DXGI_FORMAT_R8_TYPELESS;
        case DXGI_FORMAT_BC1_TYPELESS:
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
            return DXGI_FORMAT_BC1_TYPELESS;
        case DXGI_FORMAT_BC2_TYPELESS:
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
            return DXGI_FORMAT_BC2_TYPELESS;
        case DXGI_FORMAT_BC3_TYPELESS:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
            return DXGI_FORMAT_BC3_TYPELESS;
        case DXGI_FORMAT_BC4_TYPELESS:
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
            return DXGI_FORMAT_BC4_TYPELESS;
        case DXGI_FORMAT_BC5_TYPELESS:
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
            return DXGI_FORMAT_BC5_TYPELESS;
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
            return DXGI_FORMAT_B8G8R8A8_TYPELESS;
        case DXGI_FORMAT_BC7_TYPELESS:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return DXGI_FORMAT_BC7_TYPELESS;
        }

        return format;
    }

    D3D11_BIND_FLAG& operator|=(D3D11_BIND_FLAG& a, const D3D11_BIND_FLAG& b) {
        a = (D3D11_BIND_FLAG) (a | b);
        return a;
    }

    D3D11_BIND_FLAG D3D11Formats::VdToD3D11BindFlags(BufferUsage usage) {
        D3D11_BIND_FLAG flags = (D3D11_BIND_FLAG)0;

        if (usage.HasFlag(BufferUsageBits::VertexBuffer)) {
            flags |= D3D11_BIND_VERTEX_BUFFER;
        }

        if (usage.HasFlag(BufferUsageBits::IndexBuffer)) {
            flags |= D3D11_BIND_INDEX_BUFFER;
        }

        if (usage.HasFlag(BufferUsageBits::UniformBuffer)) {
            flags |= D3D11_BIND_CONSTANT_BUFFER;
        }

        if (usage.HasFlag(BufferUsageBits::StructuredBufferReadOnly) && usage.HasFlag(BufferUsageBits::StructuredBufferReadWrite)) {
            flags |= D3D11_BIND_SHADER_RESOURCE;
        }

        if (usage.HasFlag(BufferUsageBits::StructuredBufferReadWrite)) {
            flags |= D3D11_BIND_UNORDERED_ACCESS;
        }

        return flags;
    }

    TextureUsage D3D11Formats::GetVdUsage(D3D11_BIND_FLAG bindFlags, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_RESOURCE_MISC_FLAG optionFlags) {
        TextureUsage usage;

        if (bindFlags & D3D11_BIND_RENDER_TARGET) {
            usage |= TextureUsageBits::RenderTarget;
        }

        if (bindFlags & D3D11_BIND_DEPTH_STENCIL) {
            usage |= TextureUsageBits::DepthStencil;
        }

        if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {
            usage |= TextureUsageBits::Sampled;
        }

        if (bindFlags & D3D11_BIND_UNORDERED_ACCESS) {
            usage |= TextureUsageBits::Storage;
        }

        if (optionFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) {
            usage |= TextureUsageBits::Cubemap;
        }

        if (optionFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS) {
            usage |= TextureUsageBits::GenerateMipmaps;
        }

        return usage;
    }

    DXGI_FORMAT D3D11Formats::ToDxgiFormat(VertexElementFormat format) {
        switch (format) {
        case VertexElementFormat::Float1:
            return DXGI_FORMAT_R32_FLOAT;
        case VertexElementFormat::Float2:
            return DXGI_FORMAT_R32G32_FLOAT;
        case VertexElementFormat::Float3:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case VertexElementFormat::Float4:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        throw Illegal::Value<VertexElementFormat>();
    }

    void CheckResult(HRESULT result, std::source_location loc = std::source_location::current()) {
        if (SUCCEEDED(result)) return;

        std::stringstream str;
        str << "HRESULT code " << result << " doesn't indicate success. (at ";
        str << loc.file_name() << ":" << loc.line() << ")";

        throw VeldridException(str.str());
    }

#pragma region -- D3D11Texture

    D3D11Texture::D3D11Texture(ID3D11Device* device, TextureDescription& description) {
        _device = device;
        _width = description.Width;
        _height = description.Height;
        _depth = description.Depth;
        _mipLevels = description.MipLevels;
        _arrayLayers = description.ArrayLayers;
        _format = description.Format;
        _usage = description.Usage;
        _type = description.Type;
        _sampleCount = description.SampleCount;

        _dxgiFormat = D3D11Formats::ToDxgiFormat(description.Format, description.Usage.HasFlag(TextureUsageBits::DepthStencil));
        _typelessDxgiFormat = D3D11Formats::GetTypelessFormat(_dxgiFormat);

        D3D11_CPU_ACCESS_FLAG    cpuFlags      = (D3D11_CPU_ACCESS_FLAG) 0;
        D3D11_USAGE              resourceUsage = D3D11_USAGE_DEFAULT;
        D3D11_BIND_FLAG          bindFlags     = (D3D11_BIND_FLAG) 0;
        D3D11_RESOURCE_MISC_FLAG optionFlags   = (D3D11_RESOURCE_MISC_FLAG) 0;

        if (description.Usage.HasFlag(TextureUsageBits::RenderTarget)) {
            bindFlags = (D3D11_BIND_FLAG) (bindFlags | D3D11_BIND_RENDER_TARGET);
        }

        if (description.Usage.HasFlag(TextureUsageBits::DepthStencil)) {
            bindFlags = (D3D11_BIND_FLAG)(bindFlags | D3D11_BIND_DEPTH_STENCIL);
        }

        if (description.Usage.HasFlag(TextureUsageBits::Sampled)) {
            bindFlags = (D3D11_BIND_FLAG)(bindFlags | D3D11_BIND_SHADER_RESOURCE);
        }

        if (description.Usage.HasFlag(TextureUsageBits::Storage)) {
            bindFlags = (D3D11_BIND_FLAG)(bindFlags | D3D11_BIND_UNORDERED_ACCESS);
        }

        if (description.Usage.HasFlag(TextureUsageBits::Staging)) {
            cpuFlags = (D3D11_CPU_ACCESS_FLAG)(cpuFlags | D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE);
                resourceUsage = D3D11_USAGE_STAGING;
        }

        if (description.Usage.HasFlag(TextureUsageBits::GenerateMipmaps)) {
            bindFlags = (D3D11_BIND_FLAG) (bindFlags | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            optionFlags = (D3D11_RESOURCE_MISC_FLAG) (optionFlags | D3D11_RESOURCE_MISC_GENERATE_MIPS);
        }

        int arraySize = (int) description.ArrayLayers;
        if (description.Usage.HasFlag(TextureUsageBits::Cubemap)) {
            optionFlags = (D3D11_RESOURCE_MISC_FLAG) (optionFlags | D3D11_RESOURCE_MISC_TEXTURECUBE);
            arraySize *= 6;
        }

        int roundedWidth = (int) description.Width;
        int roundedHeight = (int) description.Height;
        if (IsCompressedFormat(description.Format)) {
            roundedWidth = ((roundedWidth + 3) / 4) * 4;
            roundedHeight = ((roundedHeight + 3) / 4) * 4;
        }

        if (_type == TextureType::Texture1D) {
            D3D11_TEXTURE1D_DESC desc = {};
            desc.Width = roundedWidth;
            desc.MipLevels = description.MipLevels;
            desc.ArraySize = arraySize;
            desc.Format = _typelessDxgiFormat;
            desc.BindFlags = bindFlags;
            desc.CPUAccessFlags = cpuFlags;
            desc.Usage = resourceUsage;
            desc.MiscFlags = optionFlags;
            
            ID3D11Texture1D* texture;
            CheckResult(_device->CreateTexture1D(&desc, 0, &texture));
            CheckResult(texture->QueryInterface<ID3D11Resource>(&_deviceTexture));
        } 
        else if (_type == TextureType::Texture2D) {
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Width = roundedWidth;
            desc.Height = roundedHeight;
            desc.MipLevels = description.MipLevels;
            desc.ArraySize = arraySize;
            desc.Format = _typelessDxgiFormat;
            desc.BindFlags = bindFlags;
            desc.CPUAccessFlags = cpuFlags;
            desc.Usage = resourceUsage;
            desc.MiscFlags = optionFlags;

            DXGI_SAMPLE_DESC sampleDesc = {};
            sampleDesc.Count = (UInt32) _sampleCount;
            sampleDesc.Quality = 0;
            desc.SampleDesc = sampleDesc;

            ID3D11Texture2D* texture;
            CheckResult(_device->CreateTexture2D(&desc, 0, &texture));
            CheckResult(texture->QueryInterface<ID3D11Resource>(&_deviceTexture));
        }
        else if (_type == TextureType::Texture3D) {
            D3D11_TEXTURE3D_DESC desc = {};
            desc.Width = roundedWidth;
            desc.Height = roundedHeight;
            desc.Depth = description.Depth;
            desc.MipLevels = description.MipLevels;
            desc.Format = _typelessDxgiFormat;
            desc.BindFlags = bindFlags;
            desc.CPUAccessFlags = cpuFlags;
            desc.Usage = resourceUsage;
            desc.MiscFlags = optionFlags;

            ID3D11Texture3D* texture;
            CheckResult(_device->CreateTexture3D(&desc, 0, &texture));
            CheckResult(texture->QueryInterface<ID3D11Resource>(&_deviceTexture));
        }
        else {
            throw VeldridException("Invalid texture type provided");
        }
    }
    
    D3D11Texture::D3D11Texture(ID3D11Texture2D* existingTexture, TextureType type, PixelFormat format) {
        if (type != TextureType::Texture2D) {
            throw VeldridException("Texture type must be Texture2D");
        }

        existingTexture->GetDevice(&_device);
        CheckResult(existingTexture->QueryInterface<ID3D11Resource>(&_deviceTexture));

        D3D11_TEXTURE2D_DESC desc;
        existingTexture->GetDesc(&desc);

        _width = desc.Width;
        _height = desc.Height;
        _depth = 1;
        _mipLevels = desc.MipLevels;
        _arrayLayers = desc.ArraySize;
        _format = format;
        _sampleCount = (TextureSampleCount) desc.SampleDesc.Count;
        _type = type;
        _usage = D3D11Formats::GetVdUsage((D3D11_BIND_FLAG) desc.BindFlags, 
                                          (D3D11_CPU_ACCESS_FLAG) desc.CPUAccessFlags, 
                                          (D3D11_RESOURCE_MISC_FLAG) desc.MiscFlags);

        _dxgiFormat = D3D11Formats::ToDxgiFormat(format, _usage.HasFlag(TextureUsageBits::DepthStencil));
        _typelessDxgiFormat = D3D11Formats::GetTypelessFormat(_dxgiFormat);
    }

    PixelFormat        D3D11Texture::GetFormat()      { return _format; }
    UInt32      D3D11Texture::GetWidth()       { return _width; }
    UInt32      D3D11Texture::GetHeight()      { return _height; }
    UInt32      D3D11Texture::GetDepth()       { return _depth; }
    UInt32      D3D11Texture::GetMipLevels()   { return _mipLevels; }
    UInt32      D3D11Texture::GetArrayLayers() { return _arrayLayers; }
    TextureUsage       D3D11Texture::GetUsage()       { return _usage; }
    TextureType        D3D11Texture::GetType()        { return _type; }
    TextureSampleCount D3D11Texture::GetSampleCount() { return _sampleCount; }

    Bool D3D11Texture::IsDisposed() { return _deviceTexture == 0; }

    void D3D11Texture::DisposeCore() {
        _deviceTexture->Release();
        _deviceTexture = 0;
    }

#pragma endregion

#pragma region -- D3D11Framebuffer

    D3D11Framebuffer::D3D11Framebuffer(ID3D11Device* device, FramebufferDescription& description)
    : Framebuffer(description.DepthTarget, description.ColorTargets), _depthStencilView(), _disposed(false) {
        if (description.DepthTarget.has_value()) {
            
        }
    }

#pragma endregion

    Bool SdkLayersAvailable() {
        HRESULT result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_NULL, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, 0, 0, 0);
        return SUCCEEDED(result);
    }

#pragma region -- D3D11GraphicsDevice

    D3D11GraphicsDevice::D3D11GraphicsDevice(D3D11DeviceOptions options, std::optional<SwapchainDescription> swapchainDesc) {
        auto flags = options.DeviceCreationFlags;

#if _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

        if ((flags & D3D11_CREATE_DEVICE_DEBUG) && !SdkLayersAvailable()) {
            flags &= ~D3D11_CREATE_DEVICE_DEBUG;
        }

        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };

        D3D_FEATURE_LEVEL level;

        try {
            CheckResult(D3D11CreateDevice((IDXGIAdapter*)options.AdapterPtr,
                D3D_DRIVER_TYPE_HARDWARE,
                0,
                D3D11_CREATE_DEVICE_DEBUG,
                levels,
                sizeof(levels) / sizeof(D3D_FEATURE_LEVEL),
                D3D11_SDK_VERSION,
                &_device,
                &level,
                0));
        }
        catch (VeldridException&) {
            CheckResult(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, 0, 0, D3D11_SDK_VERSION, &_device, &level, 0));
        }

        IDXGIDevice* dxgiDevice;
        CheckResult(_device->QueryInterface<IDXGIDevice>(&dxgiDevice));
        CheckResult(dxgiDevice->GetAdapter(&_dxgiAdapter));

        DXGI_ADAPTER_DESC desc;
        CheckResult(_dxgiAdapter->GetDesc(&desc));

        std::wstring deviceName(desc.Description);
        _deviceName = std::string(deviceName.begin(), deviceName.end());
        
        std::stringstream vendorName;
        vendorName << "id:" << std::setw(8) << std::setfill('0') << std::hex << desc.VendorId;
        _vendorName = vendorName.str();

        _deviceId = desc.DeviceId;

        switch (_device->GetFeatureLevel()) {
        case D3D_FEATURE_LEVEL_10_0:
            _apiVersion = CreateRef<GraphicsApiVersion>(10, 0, 0, 0);
            break;
        case D3D_FEATURE_LEVEL_10_1:
            _apiVersion = CreateRef<GraphicsApiVersion>(10, 1, 0, 0);
            break;
        case D3D_FEATURE_LEVEL_11_0:
            _apiVersion = CreateRef<GraphicsApiVersion>(11, 0, 0, 0);
            break;
        case D3D_FEATURE_LEVEL_11_1:
            _apiVersion = CreateRef<GraphicsApiVersion>(11, 1, 0, 0);
            break;
        case D3D_FEATURE_LEVEL_12_0:
            _apiVersion = CreateRef<GraphicsApiVersion>(12, 0, 0, 0);
            break;
        case D3D_FEATURE_LEVEL_12_1:
            _apiVersion = CreateRef<GraphicsApiVersion>(12, 1, 0, 0);
            break;
        case D3D_FEATURE_LEVEL_12_2:
            _apiVersion = CreateRef<GraphicsApiVersion>(12, 2, 0, 0);
            break;
        }

        if (swapchainDesc.has_value()) {
            auto desc = swapchainDesc.value();
            // TODO: Set main swapchain
        }

        _device->GetImmediateContext(&_immediateContext);
        
        {
            D3D11_FEATURE_DATA_THREADING support = {};
            HRESULT result = _device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &support, sizeof(support));

            if (result < 0) {
                _supportsConcurrentResources = false;
                _supportsCommandLists = false;
            }
            else {
                _supportsConcurrentResources = support.DriverConcurrentCreates != 0;
                _supportsCommandLists = support.DriverCommandLists != 0;
            }
        }
    }

    void D3D11GraphicsDevice::InitializeComponents() {
        // TODO: Resource factory & backend info

        PostDeviceCreated();
    }

#pragma endregion

#pragma region -- D3D11ResourceCache

    struct SemanticIndices {
        int _position;
        int _texCoord;
        int _normal;
        int _color;

        static int GetAndIncrement(SemanticIndices& si, VertexElementSemantic type) {
            switch (type) {
            case VertexElementSemantic::Position:
                return si._position++;
            case VertexElementSemantic::TextureCoordinate:
                return si._texCoord++;
            case VertexElementSemantic::Normal:
                return si._normal++;
            case VertexElementSemantic::Color:
                return si._color++;
            }

            throw Illegal::Value<VertexElementSemantic>();
        }
    };

    

    ID3D11InputLayout* D3D11ResourceCache::CreateNewInputLayout(std::vector<VertexLayoutDescription> vertexLayouts,
                                                                const void* vsBytecode,
                                                                size_t vsByteCodeLength) {
        size_t totalCount = 0;
        for (auto& layout : vertexLayouts) {
            totalCount += layout.Elements.size();
        }

        int element = 0;
        D3D11_INPUT_ELEMENT_DESC* elements = new D3D11_INPUT_ELEMENT_DESC[totalCount];
        memset(elements, 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * totalCount);

        if (totalCount > 0) {
            SemanticIndices si = {};
            for (int slot = 0; slot < vertexLayouts.size(); slot++) {
                auto elementDescs = vertexLayouts[slot].Elements;
                auto stepRate = vertexLayouts[slot].InstanceStepRate;
                int currentOffset = 0;

                for (int i = 0; i < elementDescs.size(); i++) {
                    if (element >= totalCount) {
                        throw VeldridException("element >= totalCount");
                    }

                    auto desc = elementDescs[i];

                    elements[element] = {
                        GetSemanticString(desc.Semantic),
                        (UINT)SemanticIndices::GetAndIncrement(si, desc.Semantic),
                        D3D11Formats::ToDxgiFormat(desc.Format),
                        desc.Offset != 0 ? desc.Offset : currentOffset,
                        (UINT)slot,
                        stepRate == 0 ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA,
                        stepRate
                    };
                    currentOffset += GetSizeInBytes(desc.Format);
                    element++;
                }
            }
        }

        ID3D11InputLayout* layout;
        HRESULT result = _device->CreateInputLayout(elements, (UINT) totalCount, vsBytecode, vsByteCodeLength, &layout);
        // free(elements);
        delete[] elements;

        CheckResult(result);
        return layout;
    }

    const char* D3D11ResourceCache::GetSemanticString(VertexElementSemantic semantic) {
        switch (semantic) {
        case VertexElementSemantic::Position:
            return "POSITION";
        case VertexElementSemantic::Normal:
            return "NORMAL";
        case VertexElementSemantic::TextureCoordinate:
            return "TEXCOORD";
        case VertexElementSemantic::Color:
            return "COLOR";
        }

        throw Illegal::Value<VertexElementSemantic>();
    }

#pragma endregion
}

#endif