//
//  impl.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#if defined(__cplusplus)
#ifndef __VD_GL_IMPL_HEADER_GUARD
#define __VD_GL_IMPL_HEADER_GUARD

#include <Veldrid/vd.hpp>

#if !defined(VD_EXCLUDE_OPENGL_BACKEND)

#if defined(__APPLE__)
#   define GL_SILENCE_DEPRECATION
#   define GLES_SILENCE_DEPRECATION
#   if defined(__VD_TARGET_MACOS)
#       include <OpenGL/gl3.h>
#   else
#       include <OpenGLES/ES3/gl.h>
#   endif
#else
#include "gl/gl.h"
#endif // defined(__APPLE__)

namespace vd {

namespace GL {

void CheckLastError();

}

class IOpenGLDeferredResource {
public:
    virtual Mochi::Bool IsCreated() = 0;
    virtual void EnsureResourceCreated() = 0;
    virtual void DestroyGLResources() = 0;
};

class OpenGLPlatformInfo {
    using GetProcAddressFunc      = std::function<void*(std::string)>;
    using MakeCurrentFunc         = std::function<void(void*)>;
    using GetCurrentContextFunc   = std::function<void*()>;
    using ClearCurrentContextFunc = std::function<void()>;
    using DeleteContextFunc       = std::function<void(void*)>;
    using SwapBuffersFunc         = std::function<void()>;
    using SetSyncToVBlankFunc     = std::function<void(Mochi::Bool)>;
    using SetSwapchainFBFunc      = std::function<void()>;
    using ResizeSwapchainFunc     = std::function<void(Mochi::UInt32, Mochi::UInt32)>;
    
private:
    void *_contextHandle;
    GetProcAddressFunc      _GetProcAddress;
    MakeCurrentFunc         _MakeCurrent;
    GetCurrentContextFunc   _GetCurrentContext;
    ClearCurrentContextFunc _ClearCurrentContext;
    DeleteContextFunc       _DeleteContext;
    SwapBuffersFunc         _SwapBuffers;
    SetSyncToVBlankFunc     _SetSyncToVerticalBlank;
    SetSwapchainFBFunc      _SetSwapchainFramebuffer;
    ResizeSwapchainFunc     _ResizeSwapchain;
    
public:
    void* GetOpenGLContextHandle();
    void* GetProcAddress(std::string funcName);
    void MakeCurrent(void* context);
    void* GetCurrentContext();
    void ClearCurrentContext();
    void DeleteContext(void* context);
    void SwapBuffers();
    void SetSyncToVerticalBlank(Mochi::Bool sync);
    void SetSwapchainFramebuffer();
    void ResizeSwapchain(Mochi::UInt32 width, Mochi::UInt32 height);
};

__MC_DEFINE_REF_TYPE(OpenGLPlatformInfo)

class OpenGLPipeline : public Pipeline, public IOpenGLDeferredResource {
private:
    GLuint _program;
    Mochi::Bool _disposeRequested;
    Mochi::Bool _disposed;
    Mochi::Bool _created;
    
    void CreateGLResources();
    void CreateGraphicsGLResources();
    void CreateComputeGLResources();
public:
    Mochi::Bool IsCreated() override;
    void EnsureResourceCreated() override;
    void DestroyGLResources() override;
};

__MC_DEFINE_REF_TYPE(OpenGLPipeline)

class OpenGLGraphicsDevice : public GraphicsDevice {
private:
    ResourceFactoryRef _resourceFactory;
    std::string _deviceName;
    std::string _vendorName;
    std::string _version;
    std::string _shadingLanguageVersion;
    GraphicsApiVersionRef _apiVersion;
    GraphicsBackend _backendType;
    GraphicsDeviceFeatures _features;
    GLuint _vao;
    void *_glContext;
    
public:
    OpenGLGraphicsDevice(GraphicsDeviceOptions options,
                         OpenGLPlatformInfoRef info,
                         Mochi::UInt32 width,
                         Mochi::UInt32 height);
    
    void InitializeComponents() override;
    std::string GetDeviceName() override;
    std::string GetVendorName() override;
    GraphicsBackend GetBackendType() override;
    GraphicsApiVersionRef GetApiVersion() override;
    Mochi::Bool IsUvOriginTopLeft() override;
    Mochi::Bool IsDepthRangeZeroToOne() override;
    Mochi::Bool IsClipSpaceYInverted() override;
    ResourceFactoryRef GetResourceFactory() override;
    GraphicsDeviceFeatures GetFeatures() override;
};

}


#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)
#endif /* impl_hpp */
#endif
