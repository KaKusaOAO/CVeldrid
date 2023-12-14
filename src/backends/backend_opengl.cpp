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
    std::unordered_map<std::string, void*> GL::_funcMap = std::unordered_map<std::string, void*>();

#define __VD_DEFINE_GL(name, ret, ...) \
    GL:: name##_t GL:: _p_##name ;

    // MARK: Static members of function pointers
    __VD_DEFINE_GL_FUNCS

#undef __VD_DEFINE_GL

#define __VD_DECLARE_GL_0(name, ret) \
    ret GL:: name () { return _p_##name (); }
#define __VD_DECLARE_GL_1(name, ret, t1, n1) \
    ret GL:: name ( t1 n1 ) { return _p_##name ( n1 ); }
#define __VD_DECLARE_GL_2(name, ret, t1, n1, t2, n2) \
    ret GL:: name ( t1 n1, t2 n2 ) { return _p_##name ( n1, n2 ); }
#define __VD_DECLARE_GL_3(name, ret, t1, n1, t2, n2, t3, n3) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3 ) { return _p_##name ( n1, n2, n3 ); }
#define __VD_DECLARE_GL_4(name, ret, t1, n1, t2, n2, t3, n3, t4, n4) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4 ) { return _p_##name ( n1, n2, n3, n4 ); }
#define __VD_DECLARE_GL_5(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4, t5 n5 ) { return _p_##name ( n1, n2, n3, n4, n5 ); }
#define __VD_DECLARE_GL_6(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6 ) { return _p_##name ( n1, n2, n3, n4, n5, n6 ); }
#define __VD_DECLARE_GL_7(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7 ) { return _p_##name ( n1, n2, n3, n4, n5, n6, n7 ); }
#define __VD_DECLARE_GL_8(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7, t8 n8 ) { return _p_##name ( n1, n2, n3, n4, n5, n6, n7, n8 ); }
#define __VD_DECLARE_GL_9(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7, t8 n8, t9 n9 ) { return _p_##name ( n1, n2, n3, n4, n5, n6, n7, n8, n9 ); }
#define __VD_DECLARE_GL_10(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7, t8 n8, t9 n9, t10 n10 ) { return _p_##name ( n1, n2, n3, n4, n5, n6, n7, n8, n9, n10 ); }
#define __VD_DECLARE_GL_11(name, ret, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11) \
    ret GL:: name ( t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7, t8 n8, t9 n9, t10 n10, t11 n11 ) { return _p_##name ( n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11 ); }

    __VD_DECLARE_GL_2(glGenVertexArrays, void, GLsizei, n, GLuint*, arrays);
    __VD_DECLARE_GL_0(glGetError, GLenum);
    __VD_DECLARE_GL_1(glBindVertexArray, void, GLuint, arr);
    __VD_DECLARE_GL_4(glClearColor, void, GLfloat, red, GLfloat, green, GLfloat, blue, GLfloat, alpha);
    __VD_DECLARE_GL_1(glDrawBuffer, void, GLenum, buf);
    __VD_DECLARE_GL_2(glDrawBuffers, void, GLsizei, n, const GLenum*, bufs);
    __VD_DECLARE_GL_1(glClear, void, GLbitfield, mask);
    __VD_DECLARE_GL_1(glClearDepth, void, GLdouble, depth);
    __VD_DECLARE_GL_1(glClearDepthf, void, GLfloat, depth);
    __VD_DECLARE_GL_4(glDrawElements, void, GLenum, mode, GLsizei, count, GLenum, type, const void*, indices);
    __VD_DECLARE_GL_5(glDrawElementsBaseVertex, void, GLenum, mode, GLsizei, count, GLenum, type, void*, indices, GLint, baseVertex);
    __VD_DECLARE_GL_5(glDrawElementsInstanced, void, GLenum, mode, GLsizei, count, GLenum, type, const void*, indices, GLsizei, instanceCount);
    __VD_DECLARE_GL_6(glDrawElementsInstancedBaseVertex, void, GLenum, mode, GLsizei, count, GLenum, type, void*, indices, GLsizei, instanceCount, GLint, baseVertex);
    __VD_DECLARE_GL_7(glDrawElementsInstancedBaseVertexBaseInstance, void, GLenum, mode, GLsizei, count, GLenum, type, void*, indices, GLsizei, instanceCount, GLint, baseVertex, GLuint, baseInstance);
    __VD_DECLARE_GL_3(glDrawArrays, void, GLenum, mode, GLint, first, GLsizei, count);
    __VD_DECLARE_GL_4(glDrawArraysInstanced, void, GLenum, mode, GLint, first, GLsizei, count, GLsizei, instanceCount);
    __VD_DECLARE_GL_5(glDrawArraysInstancedBaseInstance, void, GLenum, mode, GLint, first, GLsizei, count, GLsizei, instanceCount, GLuint, baseInstance);
    __VD_DECLARE_GL_2(glGenBuffers, void, GLsizei, n, GLuint*, buffers);
    __VD_DECLARE_GL_2(glDeleteBuffers, void, GLsizei, n, const GLuint*, buffers);
    __VD_DECLARE_GL_2(glGenFramebuffers, void, GLsizei, n, GLuint*, ids);
    __VD_DECLARE_GL_1(glActiveTexture, void, GLenum, texture);
    __VD_DECLARE_GL_5(glFramebufferTexture1D, void, GLenum, target, GLenum, attachment, GLenum, texTarget, GLuint, texture, GLint, level);
    __VD_DECLARE_GL_5(glFramebufferTexture2D, void, GLenum, target, GLenum, attachment, GLenum, texTarget, GLuint, texture, GLint, level);
    __VD_DECLARE_GL_2(glBindTexture, void, GLenum, target, GLuint, texture);
    __VD_DECLARE_GL_2(glBindFramebuffer, void, GLenum, target, GLuint, framebuffer);
    __VD_DECLARE_GL_2(glDeleteFramebuffers, void, GLsizei, n, GLuint*, framebuffers);
    __VD_DECLARE_GL_2(glGenTextures, void, GLsizei, n, GLuint*, textures);
    __VD_DECLARE_GL_2(glDeleteTextures, void, GLsizei, n, const GLuint*, textures);
    __VD_DECLARE_GL_1(glCheckFramebufferStatus, GLenum, GLenum, target);
    __VD_DECLARE_GL_2(glBindBuffer, void, GLenum, target, GLuint, buffer);
    __VD_DECLARE_GL_5(glViewportIndexed, void, GLuint, index, GLfloat, x, GLfloat, y, GLfloat, w, GLfloat, h);
    __VD_DECLARE_GL_4(glViewport, void, GLfloat, x, GLfloat, y, GLsizei, width, GLsizei, height);
    __VD_DECLARE_GL_3(glDepthRangeIndexed, void, GLuint, index, GLdouble, nearVal, GLdouble, farVal);
    __VD_DECLARE_GL_2(glDepthRangef, void, GLfloat, nearVal, GLfloat, farVal);
    __VD_DECLARE_GL_4(glBufferSubData, void, GLenum, target, GLintptr, offset, GLsizeiptr, size, const void*, data);
    __VD_DECLARE_GL_4(glNamedBufferSubData, void, GLuint, buffer, GLintptr, offset, GLsizeiptr, size, const void*, data);
    __VD_DECLARE_GL_5(glScissorIndexed, void, GLuint, index, GLint, left, GLint, bottom, GLsizei, width, GLsizei, height);
    __VD_DECLARE_GL_4(glScissor, void, GLint, x, GLint, y, GLsizei, width, GLsizei, height);
    __VD_DECLARE_GL_2(glPixelStorei, void, GLenum, pName, GLint, param);
    __VD_DECLARE_GL_7(glTexSubImage1D, void, GLenum, target, GLint, level, GLint, xOffset, GLsizei, width, GLenum, format, GLenum, type, const void*, pixels);
    __VD_DECLARE_GL_9(glTexSubImage2D, void, GLenum, target, GLint, level, GLint, xOffset, GLint, yOffset, GLsizei, width, GLsizei, height, GLenum, format, GLenum, type, const void*, pixels);
    __VD_DECLARE_GL_11(glTexSubImage3D, void, GLenum, target, GLint, level, GLint, xOffset, GLint, yOffset, GLint, zOffset, GLsizei, width, GLsizei, height, GLsizei, depth, GLenum, format, GLenum, type, const void*, pixels);
    __VD_DECLARE_GL_4(glShaderSource, void, GLuint, shader, GLsizei, count, const GLchar**, str, const GLint*, length);
    __VD_DECLARE_GL_1(glCreateShader, GLuint, GLenum, shaderType);
    __VD_DECLARE_GL_1(glCompileShader, void, GLuint, shader);
    __VD_DECLARE_GL_3(glGetShaderiv, void, GLuint, shader, GLenum, pName, GLint*, params);
    __VD_DECLARE_GL_4(glGetShaderInfoLog, void, GLuint, shader, GLsizei, maxLength, GLsizei*, length, GLchar*, infoLog);
    __VD_DECLARE_GL_1(glDeleteShader, void, GLuint, shader);
    __VD_DECLARE_GL_2(glGenSamplers, void, GLsizei, n, GLuint*, samplers);
    __VD_DECLARE_GL_3(glSamplerParameterf, void, GLuint, sampler, GLenum, pname, GLfloat, param);
    __VD_DECLARE_GL_3(glSamplerParameteri, void, GLuint, sampler, GLenum, pname, GLint, param);
    __VD_DECLARE_GL_3(glSamplerParameterfv, void, GLuint, sampler, GLenum, pname, const GLfloat*, params);
    __VD_DECLARE_GL_3(glSamplerParameteriv, void, GLuint, sampler, GLenum, pname, const GLint*, params);
    __VD_DECLARE_GL_2(glBindSampler, void, GLuint, unit, GLuint, sampler);
    __VD_DECLARE_GL_2(glDeleteSamplers, void, GLsizei, n, const GLuint*, samplers);
    __VD_DECLARE_GL_4(glColorMask, void, GLboolean, red, GLboolean, green, GLboolean, blue, GLboolean, alpha);
    __VD_DECLARE_GL_5(glColorMaski, void, GLuint, buf, GLboolean, red, GLboolean, green, GLboolean, blue, GLboolean, alpha);
    __VD_DECLARE_GL_4(glBlendFuncSeparate, void, GLenum, srcRGB, GLenum, dstRGB, GLenum, srcAlpha, GLenum, dstAlpha);
    __VD_DECLARE_GL_5(glBlendFuncSeparatei, void, GLuint, buf, GLenum, srcRGB, GLenum, dstRGB, GLenum, srcAlpha, GLenum, dstAlpha);
    __VD_DECLARE_GL_1(glEnable, void, GLenum, cap);
    __VD_DECLARE_GL_2(glEnablei, void, GLenum, cap, GLuint, index);
    __VD_DECLARE_GL_1(glDisable, void, GLenum, cap);
    __VD_DECLARE_GL_2(glDisablei, void, GLenum, cap, GLuint, index);
    __VD_DECLARE_GL_2(glPushGroupMarker, void, GLsizei, length, const GLchar*, marker);
    __VD_DECLARE_GL_0(glPopGroupMarker, void);
    __VD_DECLARE_GL_0(glCreateProgram, GLuint);

    void GL::glClearDepth_Compat(GLfloat depth) {
        _p_glClearDepthf_Compat(depth);
    }

    void GL::CheckLastError() {
        GLenum error = GL::glGetError();
        if (error != GL_NO_ERROR) {
            std::stringstream str;
            str << "glGetError indicated an error: " << error;
            throw VeldridException(str.str());
        }
    }

    void GL::LoadAllFunctions(void* context, GL::FnGetProcAddress getProcAddress, Mochi::Bool gles) {
        _getProcAddress = getProcAddress;

        // Common functions

#define __VD_LOAD_FUNCTION(name) LoadFunction(#name, _p_##name);
        
        __VD_LOAD_FUNCTION(glGenVertexArrays);
        __VD_LOAD_FUNCTION(glGetError);
        __VD_LOAD_FUNCTION(glBindVertexArray);
        __VD_LOAD_FUNCTION(glClearColor);
        __VD_LOAD_FUNCTION(glDrawBuffer);
        __VD_LOAD_FUNCTION(glDrawBuffers);
        __VD_LOAD_FUNCTION(glClear);
        __VD_LOAD_FUNCTION(glClearDepth);
        __VD_LOAD_FUNCTION(glClearDepthf);

        if (_p_glClearDepthf) {
            _p_glClearDepthf_Compat = _p_glClearDepthf;
        } else {
            _p_glClearDepthf_Compat = [](GLfloat depth) {
                _p_glClearDepth(depth);
            };
        }

        __VD_LOAD_FUNCTION(glDrawElements);
        __VD_LOAD_FUNCTION(glDrawElementsBaseVertex);
        __VD_LOAD_FUNCTION(glDrawElementsInstanced);
        __VD_LOAD_FUNCTION(glDrawElementsInstancedBaseVertex);
        __VD_LOAD_FUNCTION(glDrawArrays);
        __VD_LOAD_FUNCTION(glDrawArraysInstanced);
        __VD_LOAD_FUNCTION(glDrawArraysInstancedBaseInstance);
        __VD_LOAD_FUNCTION(glGenBuffers);
        __VD_LOAD_FUNCTION(glDeleteBuffers);
        __VD_LOAD_FUNCTION(glGenFramebuffers);
        __VD_LOAD_FUNCTION(glActiveTexture);
        __VD_LOAD_FUNCTION(glFramebufferTexture2D);
        __VD_LOAD_FUNCTION(glBindTexture);
        __VD_LOAD_FUNCTION(glBindFramebuffer);
        __VD_LOAD_FUNCTION(glDeleteFramebuffers);
        __VD_LOAD_FUNCTION(glGenTextures);
        __VD_LOAD_FUNCTION(glDeleteTextures);
        __VD_LOAD_FUNCTION(glCheckFramebufferStatus);
        __VD_LOAD_FUNCTION(glBindBuffer);
        __VD_LOAD_FUNCTION(glDepthRangeIndexed);
        __VD_LOAD_FUNCTION(glBufferSubData);
        __VD_LOAD_FUNCTION(glNamedBufferSubData);
        __VD_LOAD_FUNCTION(glScissorIndexed);
        __VD_LOAD_FUNCTION(glTexSubImage1D);
        __VD_LOAD_FUNCTION(glTexSubImage2D);
        __VD_LOAD_FUNCTION(glTexSubImage3D);
        __VD_LOAD_FUNCTION(glPixelStorei);
        __VD_LOAD_FUNCTION(glShaderSource);
        __VD_LOAD_FUNCTION(glCreateShader);
        __VD_LOAD_FUNCTION(glCompileShader);
        __VD_LOAD_FUNCTION(glGetShaderiv);
        __VD_LOAD_FUNCTION(glGetShaderInfoLog);
        __VD_LOAD_FUNCTION(glDeleteShader);
        __VD_LOAD_FUNCTION(glGenSamplers);
        __VD_LOAD_FUNCTION(glSamplerParameterf);
        __VD_LOAD_FUNCTION(glSamplerParameteri);
        __VD_LOAD_FUNCTION(glSamplerParameterfv);
        __VD_LOAD_FUNCTION(glBindSampler);
        __VD_LOAD_FUNCTION(glDeleteSamplers);
        __VD_LOAD_FUNCTION(glColorMaski);
        __VD_LOAD_FUNCTION(glColorMask);
        __VD_LOAD_FUNCTION(glBlendFuncSeparatei);
        __VD_LOAD_FUNCTION(glBlendFuncSeparate);
        __VD_LOAD_FUNCTION(glEnablei);
        __VD_LOAD_FUNCTION(glEnable);
        __VD_LOAD_FUNCTION(glDisablei);
        __VD_LOAD_FUNCTION(glDisable);

#undef __VD_LOAD_FUNCTION
        Mochi::ThrowNotImplemented();
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
        OpenGLPlatformInfo::Ref info,
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

    GraphicsApiVersion::Ref OpenGLGraphicsDevice::GetApiVersion() {
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

    ResourceFactory::Ref OpenGLGraphicsDevice::GetResourceFactory() {
        Mochi::ThrowNotImplemented();
    }

    GraphicsDeviceFeatures OpenGLGraphicsDevice::GetFeatures() {
        Mochi::ThrowNotImplemented();
    }

    void OpenGLGraphicsDevice::SubmitCommandsCore(CommandList::Ref commandList, Fence::Ref fence) {
        Mochi::ThrowNotImplemented();
    }
}

#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)
