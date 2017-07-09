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
    namespace impl {
        SIMDEE_INL uint32x4_t neon_load(const bool32_t* ptr) {
            return vld1q_u32(reinterpret_cast<const uint32_t*>(ptr));
        }
        SIMDEE_INL float32x4_t neon_load(const float* ptr) { return vld1q_f32(ptr); }
        SIMDEE_INL uint32x4_t neon_load(const uint32_t* ptr) { return vld1q_u32(ptr); }
        SIMDEE_INL int32x4_t neon_load(const int32_t* ptr) { return vld1q_s32(ptr); }
        SIMDEE_INL void neon_store(const uint32x4_t& vec, bool32_t* ptr) {
            vst1q_u32(reinterpret_cast<uint32_t*>(ptr), vec);
        }
        SIMDEE_INL void neon_store(const float32x4_t& vec, float* ptr) { vst1q_f32(ptr, vec); }
        SIMDEE_INL void neon_store(const uint32x4_t& vec, uint32_t* ptr) { vst1q_u32(ptr, vec); }
        SIMDEE_INL void neon_store(const int32x4_t& vec, int32_t* ptr) { vst1q_s32(ptr, vec); }
    }

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

    template <typename Crtp>
    struct neon_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_INL void aligned_load(const scalar_t* r) { mm = impl::neon_load(r); }
        SIMDEE_INL void unaligned_load(const scalar_t* r) { mm = impl::neon_load(r); }
        SIMDEE_INL void aligned_store(scalar_t* r) const { impl::neon_store(mm, r); }
        SIMDEE_INL void unaligned_store(scalar_t* r) const { impl::neon_store(mm, r); }
    };

    struct neonb final : neon_base<neonb> {};
    struct neonf final : neon_base<neonf> {};
    struct neonu final : neon_base<neonu> {};
    struct neons final : neon_base<neons> {};
}

#endif // SIMDEE_SIMD_TYPES_NEON_HPP
