#pragma once

#if defined(__cplusplus)
#ifndef __VD_D3D11_IMPL_HEADER_GUARD
#define __VD_D3D11_IMPL_HEADER_GUARD

#include <Veldrid/config.hpp>

#if !defined(VD_EXCLUDE_D3D11_BACKEND)

#include <Veldrid/vd.hpp>
#include <dxgi.h>
#include <d3d11.h>

namespace __VD_NAMESPACE {

    namespace D3D11Formats {
        DXGI_FORMAT ToDxgiFormat(PixelFormat format, Bool depthFormat);
        DXGI_FORMAT GetTypelessFormat(DXGI_FORMAT format);
        D3D11_BIND_FLAG VdToD3D11BindFlags(BufferUsage usage);
        TextureUsage GetVdUsage(D3D11_BIND_FLAG bindFlags, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_RESOURCE_MISC_FLAG optionFlags);
        DXGI_FORMAT ToDxgiFormat(VertexElementFormat format);
    };

    class D3D11Texture : public Texture {
    public:
        using Ref = __VD_NAMESPACE::Handle<D3D11Texture>;

        D3D11Texture(ID3D11Device* device, TextureDescription& description);
        D3D11Texture(ID3D11Texture2D* existingTexture, TextureType type, PixelFormat format);

        PixelFormat        GetFormat()      override;
        UInt32             GetWidth()       override;
        UInt32             GetHeight()      override;
        UInt32             GetDepth()       override;
        UInt32             GetMipLevels()   override;
        UInt32             GetArrayLayers() override;
        TextureUsage       GetUsage()       override;
        TextureType        GetType()        override;
        TextureSampleCount GetSampleCount() override;

        Bool IsDisposed() override;
        void DisposeCore() override;

    private:
        ID3D11Device* _device;
        std::string _name;

        PixelFormat        _format;
        UInt32             _width;
        UInt32             _height;
        UInt32             _depth;
        UInt32             _mipLevels;
        UInt32             _arrayLayers;
        TextureUsage       _usage;
        TextureType        _type;
        TextureSampleCount _sampleCount;

        ID3D11Resource* _deviceTexture;
        DXGI_FORMAT _dxgiFormat;
        DXGI_FORMAT _typelessDxgiFormat;
    };

    class D3D11Framebuffer : public Framebuffer {
    public:
        using Ref = __VD_NAMESPACE::Handle<D3D11Framebuffer>;

        D3D11Framebuffer(ID3D11Device* device, FramebufferDescription& description);
        std::string GetName() override;
        void SetName(std::string name) override;
        Bool IsDisposed() override;
        void Dispose() override;

    private:
        std::string _name;
        Bool _disposed;

        std::vector<ID3D11RenderTargetView*> _renderTargetViews;
        ID3D11DepthStencilView* _depthStencilView;
    };

    class D3D11GraphicsDevice : public GraphicsDevice {
    public:
        using Ref = __VD_NAMESPACE::Handle<D3D11GraphicsDevice>;
        D3D11GraphicsDevice(D3D11DeviceOptions options, std::optional<SwapchainDescription> swapchainDesc);
        void InitializeComponents();

    private:
        IDXGIAdapter* _dxgiAdapter;
        ID3D11Device* _device;
        std::string _deviceName;
        std::string _vendorName;
        GraphicsApiVersion::Ref _apiVersion;
        int _deviceId;
        ID3D11DeviceContext* _immediateContext;
        Bool _supportsConcurrentResources;
        Bool _supportsCommandLists;
    };

    class D3D11ResourceLayout : public ResourceLayout {
    public:
        using Ref = __VD_NAMESPACE::Handle<D3D11ResourceLayout>;
    };

    class D3D11ResourceCache : public IDisposable {
    public:
        using Ref = __VD_NAMESPACE::Handle<D3D11ResourceCache>;

        D3D11ResourceCache(ID3D11Device* device);

        void GetPipelineResources(BlendStateDescription& blendDesc,
                                  Bool multiSample,
                                  const void* vsBytecode,
                                  size_t vsBytecodeLength,
                                  ID3D11BlendState** outBlendState,
                                  ID3D11DepthStencilState** outDepthState,
                                  ID3D11RasterizerState** outRasterState,
                                  ID3D11InputLayout** outInputLayout);
    private:
        ID3D11Device* _device;
        std::mutex _lock;

        ID3D11InputLayout* GetInputLayout(std::vector<VertexLayoutDescription> vertexLayouts,
                                          const void* vsBytecode,
                                          size_t vsByteCodeLength);
        ID3D11InputLayout* CreateNewInputLayout(std::vector<VertexLayoutDescription> vertexLayouts,
                                                const void* vsBytecode,
                                                size_t vsByteCodeLength);
        const char* GetSemanticString(VertexElementSemantic semantic);
    };

    class D3D11Pipeline : public Pipeline {
    public:
        using Ref = __VD_NAMESPACE::Handle<D3D11Pipeline>;

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

        Bool IsComputePipeline() override;

    private:
        std::string _name;
        Bool _disposed;

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
    };

    class D3D11CommandList : public CommandList {
    public:
        using Ref = __VD_NAMESPACE::Handle<D3D11CommandList>;

    private:
        D3D11GraphicsDevice::Ref _gd;
        ID3D11DeviceContext* _context;
        Bool _begun;
        Bool _disposed;
        ID3D11CommandList* _commandList;

        std::vector<ID3D11Buffer*> _vertexBindings;
        std::vector<int> _vertexStrides;
        std::vector<int> _vertexOffsets;

        DeviceBuffer::Ref _ib;
        UInt32 _ibOffset;
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