#pragma once

#if defined(__cplusplus)
#ifndef __VD_D3D11_IMPL_HEADER_GUARD
#define __VD_D3D11_IMPL_HEADER_GUARD

#include <Veldrid/config.hpp>

#if !defined(VD_EXCLUDE_D3D11_BACKEND)

#include <Veldrid/vd.hpp>
#include <dxgi.h>
#include <d3d11.h>

namespace vd {

    namespace D3D11Formats {
        DXGI_FORMAT ToDxgiFormat(PixelFormat format, Mochi::Bool depthFormat);
        DXGI_FORMAT GetTypelessFormat(DXGI_FORMAT format);
        D3D11_BIND_FLAG VdToD3D11BindFlags(BufferUsage usage);
        TextureUsage GetVdUsage(D3D11_BIND_FLAG bindFlags, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_RESOURCE_MISC_FLAG optionFlags);
        DXGI_FORMAT ToDxgiFormat(VertexElementFormat format);
    };

    class D3D11Texture : public Texture {
    private:
        ID3D11Device* _device;
        std::string _name;

        PixelFormat        _format;
        Mochi::UInt32      _width;
        Mochi::UInt32      _height;
        Mochi::UInt32      _depth;
        Mochi::UInt32      _mipLevels;
        Mochi::UInt32      _arrayLayers;
        TextureUsage       _usage;
        TextureType        _type;
        TextureSampleCount _sampleCount;

        ID3D11Resource* _deviceTexture;
        DXGI_FORMAT _dxgiFormat;
        DXGI_FORMAT _typelessDxgiFormat;

    public:
        using Ref = std::shared_ptr<D3D11Texture>;

        D3D11Texture(ID3D11Device* device, TextureDescription& description);
        D3D11Texture(ID3D11Texture2D* existingTexture, TextureType type, PixelFormat format);

        PixelFormat        GetFormat()      override;
        Mochi::UInt32      GetWidth()       override;
        Mochi::UInt32      GetHeight()      override;
        Mochi::UInt32      GetDepth()       override;
        Mochi::UInt32      GetMipLevels()   override;
        Mochi::UInt32      GetArrayLayers() override;
        TextureUsage       GetUsage()       override;
        TextureType        GetType()        override;
        TextureSampleCount GetSampleCount() override;

        Mochi::Bool IsDisposed() override;
        void DisposeCore() override;
    };

    class D3D11Framebuffer : public Framebuffer {
    private:
        std::string _name;
        Mochi::Bool _disposed;

        std::vector<ID3D11RenderTargetView*> _renderTargetViews;
        ID3D11DepthStencilView* _depthStencilView;

    public:
        D3D11Framebuffer(ID3D11Device* device, FramebufferDescription& description);
        std::string GetName() override;
        void SetName(std::string name) override;
        Mochi::Bool IsDisposed() override;
        void Dispose() override;
    };

    class D3D11GraphicsDevice : public GraphicsDevice {
    private:
        IDXGIAdapter* _dxgiAdapter;
        ID3D11Device* _device;
        std::string _deviceName;
        std::string _vendorName;
        GraphicsApiVersionRef _apiVersion;
        int _deviceId;
        ID3D11DeviceContext* _immediateContext;
        Mochi::Bool _supportsConcurrentResources;
        Mochi::Bool _supportsCommandLists;

    public:
        using Ref = __MC_REF_TYPE(D3D11GraphicsDevice);
        D3D11GraphicsDevice(D3D11DeviceOptions options, std::optional<SwapchainDescription> swapchainDesc);
        void InitializeComponents() override;
    };

    class D3D11ResourceLayout : public ResourceLayout {
    public:
        using Ref = __MC_REF_TYPE(D3D11ResourceLayout);
    };

    class D3D11ResourceCache : public Mochi::IDisposable {
    private:
        ID3D11Device* _device;
        std::mutex _lock;

        ID3D11InputLayout* GetInputLayout(std::vector<VertexLayoutDescription> vertexLayouts,
                                          const void* vsBytecode,
                                          size_t vsByteCodeLength);
        ID3D11InputLayout* CreateNewInputLayout(std::vector<VertexLayoutDescription> vertexLayouts,
                                                const void* vsBytecode,
                                                size_t vsByteCodeLength);
        std::string GetSemanticString(VertexElementSemantic semantic);

    public:
        using Ref = __MC_REF_TYPE(D3D11ResourceCache);
        D3D11ResourceCache(ID3D11Device* device);

        void GetPipelineResources(BlendStateDescription& blendDesc,
                                  Mochi::Bool multiSample,
                                  const void* vsBytecode,
                                  size_t vsBytecodeLength,
                                  ID3D11BlendState**        outBlendState,
                                  ID3D11DepthStencilState** outDepthState,
                                  ID3D11RasterizerState**   outRasterState,
                                  ID3D11InputLayout**       outInputLayout);
    };

    class D3D11Pipeline : public Pipeline {
    private:
        std::string _name;
        Mochi::Bool _disposed;

        ID3D11BlendState*        _blendState;
        ID3D11DepthStencilState* _depthStencilState;
        ID3D11RasterizerState*   _rasterizerState;
        D3D_PRIMITIVE_TOPOLOGY   _primitiveTopology;
        ID3D11InputLayout*       _inputLayout;
        ID3D11VertexShader*      _vertexShader;
        ID3D11GeometryShader*    _geometryShader; // May be null.
        ID3D11HullShader*        _hullShader;     // May be null.
        ID3D11DomainShader*      _domainShader;   // May be null.
        ID3D11PixelShader*       _pixelShader;
        ID3D11ComputeShader*     _computeShader;
        
        std::vector<D3D11ResourceLayout::Ref> _resourceLayouts;
        std::vector<int> _vertexStrides;

    public:
        using Ref = __MC_REF_TYPE(D3D11Pipeline);

        ID3D11BlendState*        GetBlendState();
        ID3D11DepthStencilState* GetDepthStencilState();
        ID3D11RasterizerState*   GetRasterizerState();
        D3D_PRIMITIVE_TOPOLOGY   GetPrimitiveTopology();
        ID3D11InputLayout*       GetInputLayout();
        ID3D11VertexShader*      GetVertexShader();
        ID3D11GeometryShader*    GetGeometryShader(); // May be null.
        ID3D11HullShader*        GetHullShader();     // May be null.
        ID3D11DomainShader*      GetDomainShader();   // May be null.
        ID3D11PixelShader*       GetPixelShader();
        ID3D11ComputeShader*     GetComputeShader();

        std::vector<D3D11ResourceLayout::Ref> GetResourceLayouts();
        std::vector<int> GetVertexStrides();

        Mochi::Bool IsComputePipeline() override;
    };

    class D3D11CommandList : public CommandList {
    private:
        D3D11GraphicsDevice::Ref _gd;
        ID3D11DeviceContext* _context;
        Mochi::Bool _begun;
        Mochi::Bool _disposed;
        ID3D11CommandList* _commandList;

        std::vector<ID3D11Buffer*> _vertexBindings;
        std::vector<int> _vertexStrides;
        std::vector<int> _vertexOffsets;

        DeviceBuffer::Ref _ib;
        Mochi::UInt32 _ibOffset;
        ID3D11BlendState*        _blendState;
        ID3D11DepthStencilState* _depthStencilState;
        ID3D11RasterizerState*   _rasterizerState;
        D3D_PRIMITIVE_TOPOLOGY   _primitiveTopology;
        ID3D11InputLayout*       _inputLayout;
        ID3D11VertexShader*      _vertexShader;
        ID3D11GeometryShader*    _geometryShader;
        ID3D11HullShader*        _hullShader;
        ID3D11DomainShader*      _domainShader;
        ID3D11PixelShader*       _pixelShader;
    };
}

#endif // !defined(VD_EXCLUDE_D3D11_BACKEND)
#endif // __VD_D3D11_IMPL_HEADER_GUARD
#endif // defined(__cplusplus)