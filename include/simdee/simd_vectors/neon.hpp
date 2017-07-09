// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_NEON_HPP
#define SIMDEE_SIMD_TYPES_NEON_HPP

#include "common.hpp"

#if !SIMDEE_NEON
#error "NEON intrinsics are required to use the NEON SIMD type. Please check your build options."
#endif

#include <arm_neon.h>

namespace sd {
    struct neonb;
    struct neonf;
    struct neonu;
    struct neons;

    template <>
    struct is_simd_vector<neonb> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<neonf> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<neonu> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<neons> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Scalar_t>
    struct neon_traits {
        using simd_t = Simd_t;
        using scalar_t = Scalar_t;
        using vec_b = neonb;
        using vec_f = neonf;
        using vec_u = neonu;
        using vec_s = neons;
        using mask_t = impl::mask<0xfU>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(int32x4_t)>;
    };

    template <>
    struct simd_vector_traits<neonb> : neon_traits<neonb, bool32_t> {
        using vector_t = uint32x4_t;
    };

    template <>
    struct simd_vector_traits<neonf> : neon_traits<neonf, float> {
        using vector_t = float32x4_t;
    };

    template <>
    struct simd_vector_traits<neonu> : neon_traits<neonu, uint32_t> {
        using vector_t = uint32x4_t;
    };

    template <>
    struct simd_vector_traits<neons> : neon_traits<neons, int32_t> {
        using vector_t = int32x4_t;
    };

    struct neonb : simd_base<neonb> {};
    struct neonf : simd_base<neonf> {};
    struct neonu : simd_base<neonu> {};
    struct neons : simd_base<neons> {};
}

#endif // SIMDEE_SIMD_TYPES_NEON_HPP
