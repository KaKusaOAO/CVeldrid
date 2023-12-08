//
//  config.hpp
//  MiaCrate++
//
//  Created by 咔咔 on 2023/12/7.
//

#ifndef __VD_CONFIG_HPP_HEADER_GUARD
#define __VD_CONFIG_HPP_HEADER_GUARD

#define VD_VALIDATE_USAGE

#ifndef __APPLE__
#   define VD_EXCLUDE_METAL_BACKEND
#else
#   if __has_include(<AppKit/AppKit.h>)
#       define __VD_TARGET_MACOS
#   else
#       // define VD_EXCLUDE_OPENGL_BACKEND
#   endif // __has_include(<AppKit/AppKit.h>)
#endif // __APPLE__

#endif /* config_h */
