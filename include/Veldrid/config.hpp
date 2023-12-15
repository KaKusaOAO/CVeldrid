//
//  config.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#ifndef __VD_CONFIG_HPP_HEADER_GUARD
#define __VD_CONFIG_HPP_HEADER_GUARD

#define VD_VALIDATE_USAGE

#if !defined(_WIN32)
#	define VD_EXCLUDE_D3D_BACKEND
#endif // !defined(_WIN32)

#if defined(VD_EXCLUDE_D3D_BACKEND)
#	define VD_EXCLUDE_D3D9_BACKEND
#	define VD_EXCLUDE_D3D10_BACKEND
#	define VD_EXCLUDE_D3D11_BACKEND
#	define VD_EXCLUDE_D3D12_BACKEND
#endif // defined(VD_EXLUDED_D3D_BACKEND)

#if !defined(__APPLE__)
#   define VD_EXCLUDE_METAL_BACKEND
#else
// Exclude Vulkan from Apple platforms for now
#   define VD_EXCLUDE_VK_BACKEND
#   if __has_include(<AppKit/AppKit.h>)
#       define __VD_TARGET_MACOS
#   else
#       // define VD_EXCLUDE_OPENGL_BACKEND
#   endif // __has_include(<AppKit/AppKit.h>)
#endif // !defined(__APPLE__)

#endif /* config_h */
