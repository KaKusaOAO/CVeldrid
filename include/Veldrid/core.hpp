#pragma once

#if defined(__cplusplus)

#if !defined(__VD_CORE_HPP_HEADER_GUARD)
#define __VD_CORE_HPP_HEADER_GUARD

#include <Mochi/core.hpp>
#include <Veldrid/macros.hpp>

namespace __VD_NAMESPACE {

    using     Bool       = __MC_NAMESPACE::Bool;
    constexpr Bool True  = __MC_NAMESPACE::True;
    constexpr Bool False = __MC_NAMESPACE::False;

    using Int8  = __MC_NAMESPACE::Int8;
    using Int16 = __MC_NAMESPACE::Int16;
    using Int32 = __MC_NAMESPACE::Int32;
    using Int64 = __MC_NAMESPACE::Int64;

    using UInt8  = __MC_NAMESPACE::UInt8;
    using UInt16 = __MC_NAMESPACE::UInt16;
    using UInt32 = __MC_NAMESPACE::UInt32;
    using UInt64 = __MC_NAMESPACE::UInt64;

    template <class T>
    using Handle = __MC_NAMESPACE::Handle<T>;

    template <class T, class TBase>
    concept IsDerived = __MC_NAMESPACE::IsDerived<T, TBase>;

    template <int A, int B>
    concept IsGreaterThan = __MC_NAMESPACE::IsGreaterThan<A, B>;
}

#endif
#endif