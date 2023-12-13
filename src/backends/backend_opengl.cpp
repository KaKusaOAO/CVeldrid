//
//  impl.cpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#include <Veldrid/backend_opengl.hpp>

#if !defined(VD_EXCLUDE_OPENGL_BACKEND)

#if !defined(__APPLE__)

#endif

namespace vd {
#define __VD_DEFINE_GL(name, ret, ...) \
    GL:: name##_t GL:: _p_##name ;

    // MARK: Static members of function pointers
    __VD_DEFINE_GL_FUNCS

#undef __VD_DEFINE_GL

    void GL::glGenVertexArrays(GLsizei n, GLuint* arrays) { _p_glGenVertexArrays(n, arrays); }
    GLenum GL::glGetError() { return _p_glGetError(); }
    void GL::glBindVertexArray(GLuint arr) { _p_glBindVertexArray(arr); }
    void GL::glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { _p_glClearColor(red, green, blue, alpha); }
    void GL::glDrawBuffer(GLenum buf) { _p_glDrawBuffer(buf); }
    void GL::glDrawBuffers(GLsizei n, const GLenum* bufs) { _p_glDrawBuffers(n, bufs); }
    void GL::glClear(GLbitfield mask) { _p_glClear(mask); }
    void GL::glClearDepth(GLdouble depth) { _p_glClearDepth(depth); }
    void GL::glClearDepthf(GLfloat depth) { _p_glClearDepthf(depth); }
    void GL::glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) { _p_glDrawElements(mode, count, type, indices); }
    void GL::glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, void* indices, GLint baseVertex) {
        _p_glDrawElementsBaseVertex(mode, count, type, indices, baseVertex);
    }
    void GL::glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instanceCount) {
        _p_glDrawElementsInstanced(mode, count, type, indices, instanceCount);
    }
    void GL::glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, void* indices, GLsizei instanceCount, GLint baseVertex) {
        _p_glDrawElementsInstancedBaseVertex(mode, count, type, indices, instanceCount, baseVertex);
    }
    void GL::glDrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, void* indices, GLsizei instanceCount, GLint baseVertex, GLuint baseInstance) {
        _p_glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instanceCount, baseVertex, baseInstance);
    }
    void GL::glDrawArrays(GLenum mode, GLint first, GLsizei count) { _p_glDrawArrays(mode, first, count); }
    void GL::glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount) { _p_glDrawArraysInstanced(mode, first, count, instanceCount); }
    void GL::glDrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount, GLuint baseInstance) { 
        _p_glDrawArraysInstancedBaseInstance(mode, first, count, instanceCount, baseInstance); 
    }
    void GL::glGenBuffers(GLsizei n, GLuint* buffers) { _p_glGenBuffers(n, buffers); }
    void GL::glDeleteBuffers(GLsizei n, const GLuint* buffers) { _p_glDeleteBuffers(n, buffers); }
    void GL::glGenFramebuffers(GLsizei n, GLuint* ids) { _p_glGenFramebuffers(n, ids); }
    void GL::glActiveTexture(GLenum texture) { _p_glActiveTexture(texture); }
    void GL::glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum texTarget, GLuint texture, GLint level) { 
        _p_glFramebufferTexture1D(target, attachment, texTarget, texture, level); 
    }
    void GL::glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum texTarget, GLuint texture, GLint level) { 
        _p_glFramebufferTexture2D(target, attachment, texTarget, texture, level); 
    }

    GLuint GL::glCreateProgram() { return _p_glCreateProgram(); }

    void GL::CheckLastError() {
        GLenum error = GL::glGetError();
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
        }
        else {
            CreateComputeGLResources();
        }

        _created = true;
    }

    void OpenGLPipeline::CreateGraphicsGLResources() {
        _program = GL::glCreateProgram();
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

    void OpenGLGraphicsDevice::SubmitCommandsCore(CommandListRef commandList, FenceRef fence) {
        Mochi::ThrowNotImplemented();
    }
}

#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)
