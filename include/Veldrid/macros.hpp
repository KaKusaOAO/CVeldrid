#pragma once

#if !defined(__VD_MACROS_HPP_HEADER_GUARD)
#define __VD_MACROS_HPP_HEADER_GUARD

#if defined(__cplusplus)

#define __VD_NAMESPACE vd

#if defined(_MSVC_LANG) 
#   define VD_CPLUSPLUS_VERSION _MSVC_LANG
#else   // defined(_MSVC_LANG) 
#   define VD_CPLUSPLUS_VERSION __cplusplus
#endif  // defined(_MSVC_LANG)

#if VD_CPLUSPLUS_VERSION >= 202002L
#   define VD_CPLUSPLUS_HAS_CXX20
#endif // VD_CPLUSPLUS_VERSION >= 202002L

#if VD_CPLUSPLUS_VERSION >= 201703L
#   define VD_CPLUSPLUS_HAS_CXX17
#endif // VD_CPLUSPLUS_VERSION >= 201703L

#if VD_CPLUSPLUS_VERSION >= 201402L
#   define VD_CPLUSPLUS_HAS_CXX14
#endif // VD_CPLUSPLUS_VERSION >= 201402L

#if VD_CPLUSPLUS_VERSION >= 201103L
#   define VD_CPLUSPLUS_HAS_CXX11
#endif // VD_CPLUSPLUS_VERSION >= 201103L

#endif

#define VD_UNUSED(x) (void)(x);

#endif