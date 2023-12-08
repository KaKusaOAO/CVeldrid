//
//  impl.cpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#include <Veldrid/backend_opengl.hpp>

#if !defined(VD_EXCLUDE_OPENGL_BACKEND)

namespace vd {

void GL::CheckLastError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::stringstream str;
        str << "glGetError indicated an error: " << error;
        throw VeldridException(str.str());
    }
}

// MARK: -

void* OpenGLPlatformInfo::GetOpenGLContextHandle() { return _contextHandle; }
void* OpenGLPlatformInfo::GetProcAddress(std::string funcName) { return _GetProcAddress(funcName); }
void  OpenGLPlatformInfo::MakeCurrent(void* context) { _MakeCurrent(context); }
void* OpenGLPlatformInfo::GetCurrentContext() { return _GetCurrentContext(); }
void  OpenGLPlatformInfo::ClearCurrentContext() { _ClearCurrentContext(); }
void  OpenGLPlatformInfo::DeleteContext(void* context) { _DeleteContext(context); }
void  OpenGLPlatformInfo::SwapBuffers() { _SwapBuffers(); }
void  OpenGLPlatformInfo::SetSyncToVerticalBlank(Mochi::Bool sync) { _SetSyncToVerticalBlank(sync); }
void  OpenGLPlatformInfo::SetSwapchainFramebuffer() { _SetSwapchainFramebuffer(); }
void  OpenGLPlatformInfo::ResizeSwapchain(Mochi::UInt32 width, Mochi::UInt32 height) { _ResizeSwapchain(width, height); }

// MARK: -

void OpenGLPipeline::CreateGLResources() {
    if (!IsComputePipeline()) {
        CreateGraphicsGLResources();
    } else {
        CreateComputeGLResources();
    }
    
    _created = true;
}

void OpenGLPipeline::CreateGraphicsGLResources() {
    _program = glCreateProgram();
    GL::CheckLastError();
}

void OpenGLPipeline::CreateComputeGLResources() {
    Mochi::ThrowNotImplemented();
}

Mochi::Bool OpenGLPipeline::IsCreated() {
    Mochi::ThrowNotImplemented();
}

void OpenGLPipeline::EnsureResourceCreated() {
    Mochi::ThrowNotImplemented();
}

void OpenGLPipeline::DestroyGLResources() {
    Mochi::ThrowNotImplemented();
}

// MARK: -

OpenGLGraphicsDevice::OpenGLGraphicsDevice(GraphicsDeviceOptions options,
                                           OpenGLPlatformInfoRef info,
                                           Mochi::UInt32 width,
                                           Mochi::UInt32 height) {
    Mochi::ThrowNotImplemented();
}

void OpenGLGraphicsDevice::InitializeComponents() {
    Mochi::ThrowNotImplemented();
}

std::string OpenGLGraphicsDevice::GetDeviceName() {
    Mochi::ThrowNotImplemented();
}

std::string OpenGLGraphicsDevice::GetVendorName() {
    Mochi::ThrowNotImplemented();
}

GraphicsApiVersionRef OpenGLGraphicsDevice::GetApiVersion() {
    Mochi::ThrowNotImplemented();
}

GraphicsBackend OpenGLGraphicsDevice::GetBackendType() {
    Mochi::ThrowNotImplemented();
}

Mochi::Bool OpenGLGraphicsDevice::IsUvOriginTopLeft() {
    Mochi::ThrowNotImplemented();
}

Mochi::Bool OpenGLGraphicsDevice::IsDepthRangeZeroToOne() {
    Mochi::ThrowNotImplemented();
}

Mochi::Bool OpenGLGraphicsDevice::IsClipSpaceYInverted() {
    Mochi::ThrowNotImplemented();
}

ResourceFactoryRef OpenGLGraphicsDevice::GetResourceFactory() {
    Mochi::ThrowNotImplemented();
}

GraphicsDeviceFeatures OpenGLGraphicsDevice::GetFeatures() {
    Mochi::ThrowNotImplemented();
}

}

#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)
