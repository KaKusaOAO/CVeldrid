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

#include <unordered_map>

#if defined(__APPLE__)
#   define GL_SILENCE_DEPRECATION
#   define GLES_SILENCE_DEPRECATION
#   if defined(__VD_TARGET_MACOS)
#       include <OpenGL/gl3.h>
#   else
#       include <OpenGLES/ES3/gl.h>
#   endif
#else
#   include <windows.h>
#   include <GL/GL.h>
#   pragma comment(lib, "opengl32.lib")
#endif // defined(__APPLE__)

// MARK: - Additional data types from https://www.khronos.org/opengl/wiki/OpenGL_Type

#if !defined(_WIN32)
#   define __VD_STDCALL
#   if defined(__APPLE__)
typedef double GLdouble;
#   endif // defined(__APPLE__)
#else
#   define __VD_STDCALL __stdcall
typedef void* GLintptr;
typedef void* GLsizeiptr;
typedef void* GLsync;
#endif // !defined(_WIN32)

typedef char GLchar;

namespace __VD_NAMESPACE {

    class GL {

#define __VD_DEFINE_GL_FUNCS \
        __VD_DEFINE_GL(glGenVertexArrays, void, GLsizei n, GLuint* arrays); \
        __VD_DEFINE_GL(glGetError, GLenum); \
        __VD_DEFINE_GL(glBindVertexArray, void, GLuint arr); \
        __VD_DEFINE_GL(glClearColor, void, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha); \
        __VD_DEFINE_GL(glDrawBuffer, void, GLenum buf); \
        __VD_DEFINE_GL(glDrawBuffers, void, GLsizei n, const GLenum* bufs); \
        __VD_DEFINE_GL(glClear, void, GLbitfield mask); \
        __VD_DEFINE_GL(glClearDepth, void, GLdouble depth); \
        __VD_DEFINE_GL(glClearDepthf, void, GLfloat depth); \
        __VD_DEFINE_GL(glDrawElements, void, GLenum mode, GLsizei count, GLenum type, const void* indices); \
        __VD_DEFINE_GL(glDrawElementsBaseVertex, void, GLenum mode, GLsizei count, GLenum type, void *indices, GLint baseVertex); \
        __VD_DEFINE_GL(glDrawElementsInstanced, void, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instanceCount); \
        __VD_DEFINE_GL(glDrawElementsInstancedBaseVertex, void, GLenum mode, GLsizei count, GLenum type, void *indices, GLsizei instanceCount, GLint baseVertex); \
        __VD_DEFINE_GL(glDrawElementsInstancedBaseVertexBaseInstance, void, GLenum mode, GLsizei count, GLenum type, void* indices, GLsizei instanceCount, GLint baseVertex, GLuint baseInstance); \
        __VD_DEFINE_GL(glDrawArrays, void, GLenum mode, GLint first, GLsizei count); \
        __VD_DEFINE_GL(glDrawArraysInstanced, void, GLenum mode, GLint first, GLsizei count, GLsizei instanceCount); \
        __VD_DEFINE_GL(glDrawArraysInstancedBaseInstance, void, GLenum mode, GLint first, GLsizei count, GLsizei instanceCount, GLuint baseInstance); \
        __VD_DEFINE_GL(glGenBuffers, void, GLsizei n, GLuint *buffers); \
        __VD_DEFINE_GL(glDeleteBuffers, void, GLsizei n, const GLuint *buffers); \
        __VD_DEFINE_GL(glGenFramebuffers, void, GLsizei n, GLuint *ids); \
        __VD_DEFINE_GL(glActiveTexture, void, GLenum texture); \
        __VD_DEFINE_GL(glFramebufferTexture1D, void, GLenum target, GLenum attachment, GLenum texTarget, GLuint texture, GLint level); \
        __VD_DEFINE_GL(glFramebufferTexture2D, void, GLenum target, GLenum attachment, GLenum texTarget, GLuint texture, GLint level); \
        __VD_DEFINE_GL(glBindTexture, void, GLenum target, GLuint texture); \
        __VD_DEFINE_GL(glBindFramebuffer, void, GLenum target, GLuint framebuffer); \
        __VD_DEFINE_GL(glDeleteFramebuffers, void, GLsizei n, GLuint *framebuffers); \
        __VD_DEFINE_GL(glGenTextures, void, GLsizei n, GLuint *textures); \
        __VD_DEFINE_GL(glDeleteTextures, void, GLsizei n, const GLuint* textures); \
        __VD_DEFINE_GL(glCheckFramebufferStatus, GLenum, GLenum target); \
        __VD_DEFINE_GL(glBindBuffer, void, GLenum target, GLuint buffer); \
        __VD_DEFINE_GL(glViewportIndexed, void, GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h); \
        __VD_DEFINE_GL(glViewport, void, GLfloat x, GLfloat y, GLsizei width, GLsizei height); \
        __VD_DEFINE_GL(glDepthRangeIndexed, void, GLuint index, GLdouble nearVal, GLdouble farVal); \
        __VD_DEFINE_GL(glDepthRangef, void, GLfloat nearVal, GLfloat farVal); \
        __VD_DEFINE_GL(glBufferSubData, void, GLenum target, GLintptr offset, GLsizeiptr size, const void *data); \
        __VD_DEFINE_GL(glNamedBufferSubData, void, GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data); \
        __VD_DEFINE_GL(glScissorIndexed, void, GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height); \
        __VD_DEFINE_GL(glScissor, void, GLint x, GLint y, GLsizei width, GLsizei height); \
        __VD_DEFINE_GL(glPixelStorei, void, GLenum pName, GLint param); \
        __VD_DEFINE_GL(glTexSubImage1D, void, GLenum target, GLint level, GLint xOffset, GLsizei width, GLenum format, GLenum type, const void *pixels); \
        __VD_DEFINE_GL(glTexSubImage2D, void, GLenum target, GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels); \
        __VD_DEFINE_GL(glTexSubImage3D, void, GLenum target, GLint level, GLint xOffset, GLint yOffset, GLint zOffset,GLsizei width, GLsizei height, GLsizei depth,GLenum format, GLenum type, const void* pixels); \
        __VD_DEFINE_GL(glShaderSource, void, GLuint shader, GLsizei count, const GLchar **str, const GLint *length); \
        __VD_DEFINE_GL(glCreateShader, GLuint, GLenum shaderType); \
        __VD_DEFINE_GL(glCompileShader, void, GLuint shader); \
        __VD_DEFINE_GL(glGetShaderiv, void, GLuint shader, GLenum pName, GLint *params); \
        __VD_DEFINE_GL(glGetShaderInfoLog, void, GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog); \
        __VD_DEFINE_GL(glDeleteShader, void, GLuint shader); \
        __VD_DEFINE_GL(glGenSamplers, void, GLsizei n, GLuint *samplers); \
        __VD_DEFINE_GL(glSamplerParameterf, void, GLuint sampler, GLenum pname, GLfloat param) \
        __VD_DEFINE_GL(glSamplerParameteri, void, GLuint sampler, GLenum pname, GLint param) \
        __VD_DEFINE_GL(glSamplerParameterfv, void, GLuint sampler, GLenum pname, const GLfloat* params) \
        __VD_DEFINE_GL(glSamplerParameteriv, void, GLuint sampler, GLenum pname, const GLint* params) \
        __VD_DEFINE_GL(glBindSampler, void, GLuint unit, GLuint sampler) \
        __VD_DEFINE_GL(glDeleteSamplers, void, GLsizei n, const GLuint* samplers) \
        __VD_DEFINE_GL(glColorMask, void, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
        __VD_DEFINE_GL(glColorMaski, void, GLuint buf, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
        __VD_DEFINE_GL(glBlendFuncSeparate, void, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
        __VD_DEFINE_GL(glBlendFuncSeparatei, void, GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
        __VD_DEFINE_GL(glEnable, void, GLenum cap) \
        __VD_DEFINE_GL(glEnablei, void, GLenum cap, GLuint index) \
        __VD_DEFINE_GL(glDisable, void, GLenum cap) \
        __VD_DEFINE_GL(glDisablei, void, GLenum cap, GLuint index) \
        __VD_DEFINE_GL(glPushGroupMarker, void, GLsizei length, const GLchar *marker); \
        __VD_DEFINE_GL(glPopGroupMarker, void); \
        __VD_DEFINE_GL(glCreateProgram, GLuint);
        
#define __VD_DEFINE_GL(name, ret, ...) \
    public: \
        using name##_t = ret ( __VD_STDCALL *)(__VA_ARGS__); \
        static ret name (__VA_ARGS__); \
    private: \
        static name##_t _p_##name;
        
        __VD_DEFINE_GL_FUNCS
        
#undef __VD_DEFINE_GL
#undef __VD_STDCALL

    public:
        using FnGetProcAddress = std::function<void* (std::string)>;

    private:
        static std::unordered_map<std::string, void*> _funcMap;
        static glClearDepthf_t _p_glClearDepthf_Compat;
        static FnGetProcAddress _getProcAddress;

        template <class T>
        static void LoadFunction(std::string name, T& field) {
            void* ptr = _getProcAddress(name);
            _funcMap[name] = ptr;
            field = (T) ptr;
        }

    public:
        static void glClearDepth_Compat(GLfloat depth);
        static void CheckLastError();
        static void LoadAllFunctions(void* context, FnGetProcAddress getProcAddress, Bool gles);
        
        template <typename Ret, typename... Args>
        static Ret Call(std::string name, Args... args) {
            auto iter = _funcMap.find(name);
            void* funcPtr;

            if (iter == _funcMap.end()) {
                LoadFunction(name, funcPtr);
                _funcMap[name] = funcPtr;
            }
            else {
                funcPtr = iter->second;
            }

            auto func = reinterpret_cast<Ret(Args...)>(funcPtr);
            return func(args...);
        }
    };

    class IOpenGLDeferredResource {
    public:
        virtual Bool IsCreated() = 0;
        virtual void EnsureResourceCreated() = 0;
        virtual void DestroyGLResources() = 0;
    };

    class OpenGLPlatformInfo {
    public:
        using Ref = __VD_NAMESPACE::Handle<OpenGLPlatformInfo>;

        using GetProcAddressFunc = std::function<void* (std::string)>;
        using MakeCurrentFunc = std::function<void(void*)>;
        using GetCurrentContextFunc = std::function<void* ()>;
        using ClearCurrentContextFunc = std::function<void()>;
        using DeleteContextFunc = std::function<void(void*)>;
        using SwapBuffersFunc = std::function<void()>;
        using SetSyncToVBlankFunc = std::function<void(Bool)>;
        using SetSwapchainFBFunc = std::function<void()>;
        using ResizeSwapchainFunc = std::function<void(UInt32, UInt32)>;

        void* GetOpenGLContextHandle();
        void* GetProcAddress(std::string funcName);
        void MakeCurrent(void* context);
        void* GetCurrentContext();
        void ClearCurrentContext();
        void DeleteContext(void* context);
        void SwapBuffers();
        void SetSyncToVerticalBlank(Bool sync);
        void SetSwapchainFramebuffer();
        void ResizeSwapchain(UInt32 width, UInt32 height);

    private:
        void* _contextHandle;
        GetProcAddressFunc      _GetProcAddress;
        MakeCurrentFunc         _MakeCurrent;
        GetCurrentContextFunc   _GetCurrentContext;
        ClearCurrentContextFunc _ClearCurrentContext;
        DeleteContextFunc       _DeleteContext;
        SwapBuffersFunc         _SwapBuffers;
        SetSyncToVBlankFunc     _SetSyncToVerticalBlank;
        SetSwapchainFBFunc      _SetSwapchainFramebuffer;
        ResizeSwapchainFunc     _ResizeSwapchain;
    };

    class OpenGLPipeline : public Pipeline, public IOpenGLDeferredResource {
    private:
        GLuint _program;
        Bool _disposeRequested;
        Bool _disposed;
        Bool _created;

        void CreateGLResources();
        void CreateGraphicsGLResources();
        void CreateComputeGLResources();
    public:
        Bool IsCreated() override;
        void EnsureResourceCreated() override;
        void DestroyGLResources() override;
    };

    class OpenGLGraphicsDevice : public GraphicsDevice {
    private:
        ResourceFactory::Ref _resourceFactory;
        std::string _deviceName;
        std::string _vendorName;
        std::string _version;
        std::string _shadingLanguageVersion;
        GraphicsApiVersion::Ref _apiVersion;
        GraphicsBackend _backendType;
        GraphicsDeviceFeatures _features;
        GLuint _vao;
        void* _glContext;

    public:
        OpenGLGraphicsDevice(GraphicsDeviceOptions options,
                             OpenGLPlatformInfo::Ref info,
                             UInt32 width,
                             UInt32 height);

        void InitializeComponents();
        std::string GetDeviceName() override;
        std::string GetVendorName() override;
        GraphicsBackend GetBackendType() override;
        GraphicsApiVersion::Ref GetApiVersion() override;
        Bool IsUvOriginTopLeft() override;
        Bool IsDepthRangeZeroToOne() override;
        Bool IsClipSpaceYInverted() override;
        ResourceFactory::Ref GetResourceFactory() override;
        GraphicsDeviceFeatures GetFeatures() override;
            
        void SubmitCommandsCore(CommandList::Ref commandList, Fence::Ref fence) override;
    };

}


#endif // !defined(VD_EXCLUDE_OPENGL_BACKEND)
#endif /* impl_hpp */
#endif
