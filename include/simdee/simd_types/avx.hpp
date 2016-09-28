// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_AVX_HPP
#define SIMDEE_SIMD_TYPES_AVX_HPP

#include "common.hpp"

#define SIMDEE_HAVE_AVX

#if !defined(__AVX__)
#error "The macro '__AVX__' is not set. AVX intrinsics are required to use the AVX SIMD type. Please check your build options."
#endif
#if SIMDEE_NEED_INT && !defined(__AVX2__)
#error "AVX2 intrinsics are required to use the AVX SIMD type with integer operations. Use SIMDEE_NEED_INT 0 to use floating-point operations only."
#endif

#include <immintrin.h>

namespace sd {

    struct avxf;
    struct avxu;
    struct avxs;
    using not_avxu = expr::deferred_not<avxu>;

    template <>
    struct is_simd_type<avxf> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_type<avxu> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_type<avxs> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Vector_t, typename Scalar_t>
    struct avx_traits {
        using simd_t = Simd_t;
        using vector_t = Vector_t;
        using scalar_t = Scalar_t;
        using vec_f = avxf;
        using vec_u = avxu;
        using vec_s = avxs;
        using mask_t = impl::mask<0xffU>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <>
    struct simd_type_traits<avxf> : avx_traits<avxf, __m256, float> {};

    template <>
    struct simd_type_traits<avxu> : avx_traits<avxu, __m256, uint32_t> {};

    template <>
    struct simd_type_traits<avxs> : avx_traits<avxs, __m256, int32_t> {};

    template <typename Crtp>
    struct avx_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using binary_op_t = typename simd_base<Crtp>::binary_op_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(avx_base);

        SIMDEE_BASE_CTOR(avx_base, vector_t, mm = r);
        SIMDEE_BASE_CTOR(avx_base, scalar_t, mm = _mm256_broadcast_ss((float*)&r));
        SIMDEE_BASE_CTOR(avx_base, expr::zero, mm = _mm256_setzero_ps());
        SIMDEE_BASE_CTOR_TPL(avx_base, expr::aligned<T>, aligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(avx_base, expr::unaligned<T>, unaligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(avx_base, expr::init<T>, *this = r.template to<scalar_t>());
        SIMDEE_BASE_CTOR(avx_base, storage_t, aligned_load(r.data()));

        SIMDEE_BASE_CTOR_COMPLEX(avx_base, expr::all_bits) {
#       if defined(__AVX2__)
            mm = _mm256_setzero_ps();
            mm = _mm256_castsi256_ps(_mm256_cmpeq_epi32(_mm256_castps_si256(mm), _mm256_castps_si256(mm)));
#       else
            __m128 temp = _mm_setzero_ps();
            temp = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_castps_si128(temp), _mm_castps_si128(temp)));
            mm = _mm256_set_m128(temp, temp);
#       endif
            return self();
        }

        SIMDEE_INL void aligned_load(const scalar_t* r) { mm = _mm256_load_ps((const float*)r); }
        SIMDEE_INL void aligned_store(scalar_t* r) const { _mm256_store_ps((float*)r, mm); }
        SIMDEE_INL void unaligned_load(const scalar_t* r) { mm = _mm256_loadu_ps((const float*)r); }
        SIMDEE_INL void unaligned_store(scalar_t* r) const { _mm256_storeu_ps((float*)r, mm); }

        void interleaved_load(const scalar_t* r, std::size_t step) {
            alignas(avx_base)scalar_t temp[width];
            for (std::size_t i = 0; i < width; ++i, r += step) {
                temp[i] = *r;
            }
            aligned_load(temp);
        }

        void interleaved_store(scalar_t* r, std::size_t step) const {
            alignas(avx_base)scalar_t temp[width];
            aligned_store(temp);
            for (std::size_t i = 0; i < width; ++i, r += step) {
                *r = temp[i];
            }
        }

        const Crtp reduce(binary_op_t f) const {
            Crtp tmp = f(self(), _mm256_permute_ps(mm, _MM_SHUFFLE(2, 3, 0, 1)));
            tmp = f(tmp, _mm256_permute_ps(tmp.mm, _MM_SHUFFLE(1, 0, 3, 2)));
            return f(tmp, _mm256_permute2f128_ps(tmp.mm, tmp.mm, _MM_SHUFFLE(0, 0, 0, 1)));
        }
    };

    struct avxf : avx_base<avxf> {
        SIMDEE_TRIVIAL_TYPE(avxf);

        using avx_base::avx_base;
        SIMDEE_INL explicit avxf(const avxs&);

        SIMDEE_INL scalar_t first_element() const { return  _mm_cvtss_f32(_mm256_castps256_ps128(mm)); }
    };

    struct avxu : avx_base<avxu> {
        SIMDEE_TRIVIAL_TYPE(avxu);

        using avx_base::avx_base;
        SIMDEE_INL explicit avxu(const avxs&);
        SIMDEE_INL avxu(const __m256i& r) { *this = r; }
        SIMDEE_INL avxu(const not_avxu& r) { *this = r; }

        SIMDEE_INL avxu& operator=(const __m256i& r) {
            mm = _mm256_castsi256_ps(r);
            return *this;
        }

        SIMDEE_INL avxu& operator=(const not_avxu& r) {
            mm = _mm256_xor_ps(r.neg.mm, avxu(all_bits()).mm);
            return *this;
        }

        SIMDEE_INL __m256i mmi() const { return _mm256_castps_si256(mm); }
        SIMDEE_INL mask_t mask() const { return mask_t(uint32_t(_mm256_movemask_ps(mm))); }
        SIMDEE_INL scalar_t first_element() const { return tou(_mm_cvtss_f32(_mm256_castps256_ps128(mm))); }
    };

    struct avxs : avx_base<avxs> {
        SIMDEE_TRIVIAL_TYPE(avxs);

        using avx_base::avx_base;
        SIMDEE_INL explicit avxs(const avxf&);
        SIMDEE_INL explicit avxs(const avxu&);
        SIMDEE_INL avxs(const __m256i& r) { *this = r; }

        SIMDEE_INL avxs& operator=(const __m256i& r) {
            mm = _mm256_castsi256_ps(r);
            return *this;
        }

        SIMDEE_INL __m256i mmi() const { return _mm256_castps_si256(mm); }
        SIMDEE_INL scalar_t first_element() const { return tos(_mm_cvtss_f32(_mm256_castps256_ps128(mm))); }
    };

    SIMDEE_INL avxf::avxf(const avxs& r) { mm = _mm256_cvtepi32_ps(_mm256_castps_si256(r.data())); }
    SIMDEE_INL avxs::avxs(const avxf& r) { mm = _mm256_castsi256_ps(_mm256_cvtps_epi32(r.data())); }
    SIMDEE_INL avxu::avxu(const avxs& r) { mm = r.data(); }
    SIMDEE_INL avxs::avxs(const avxu& r) { mm = r.data(); }

    SIMDEE_INL const avxu operator&(const avxu& l, const avxu& r) { return _mm256_and_ps(l.data(), r.data()); }
    SIMDEE_INL const avxu operator|(const avxu& l, const avxu& r) { return _mm256_or_ps(l.data(), r.data()); }
    SIMDEE_INL const avxu operator^(const avxu& l, const avxu& r) { return _mm256_xor_ps(l.data(), r.data()); }
    SIMDEE_INL const not_avxu operator~(const avxu& l) { return not_avxu(l); }
    SIMDEE_INL const avxu andnot(const avxu& l, const avxu& r) { return _mm256_andnot_ps(r.data(), l.data()); }

    SIMDEE_INL const avxu operator<(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.data(), r.data(), _CMP_LT_OQ); }
    SIMDEE_INL const avxu operator>(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.data(), r.data(), _CMP_GT_OQ); }
    SIMDEE_INL const avxu operator<=(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.data(), r.data(), _CMP_LE_OQ); }
    SIMDEE_INL const avxu operator>=(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.data(), r.data(), _CMP_GE_OQ); }
    SIMDEE_INL const avxu operator==(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.data(), r.data(), _CMP_EQ_OQ); }
    SIMDEE_INL const avxu operator!=(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.data(), r.data(), _CMP_NEQ_OQ); }

    SIMDEE_INL const avxf operator-(const avxf& l) { return _mm256_xor_ps(l.data(), avxf(sign_bit()).data()); }
    SIMDEE_INL const avxf operator+(const avxf& l, const avxf& r) { return _mm256_add_ps(l.data(), r.data()); }
    SIMDEE_INL const avxf operator-(const avxf& l, const avxf& r) { return _mm256_sub_ps(l.data(), r.data()); }
    SIMDEE_INL const avxf operator*(const avxf& l, const avxf& r) { return _mm256_mul_ps(l.data(), r.data()); }
    SIMDEE_INL const avxf operator/(const avxf& l, const avxf& r) { return _mm256_div_ps(l.data(), r.data()); }

    SIMDEE_INL const avxf min(const avxf& l, const avxf& r) { return _mm256_min_ps(l.data(), r.data()); }
    SIMDEE_INL const avxf max(const avxf& l, const avxf& r) { return _mm256_max_ps(l.data(), r.data()); }
    SIMDEE_INL const avxf sqrt(const avxf& l) { return _mm256_sqrt_ps(l.data()); }
    SIMDEE_INL const avxf rsqrt(const avxf& l) { return _mm256_rsqrt_ps(l.data()); }
    SIMDEE_INL const avxf rcp(const avxf& l) { return _mm256_rcp_ps(l.data()); }
    SIMDEE_INL const avxf abs(const avxf& l) { return _mm256_and_ps(l.data(), avxf(abs_mask()).data()); }

    SIMDEE_INL const avxf cond(const avxu& pred, const avxf& if_true, const avxf& if_false) {
        return _mm256_blendv_ps(if_false.data(), if_true.data(), pred.data());
    }
    SIMDEE_INL const avxu cond(const avxu& pred, const avxu& if_true, const avxu& if_false) {
        return _mm256_blendv_ps(if_false.data(), if_true.data(), pred.data());
    }
    SIMDEE_INL const avxs cond(const avxu& pred, const avxs& if_true, const avxs& if_false) {
        return _mm256_blendv_ps(if_false.data(), if_true.data(), pred.data());
    }

#if defined(SIMDEE_NEED_INT)
    SIMDEE_INL const avxu operator==(const avxu& l, const avxu& r) { return _mm256_cmpeq_epi32(l.mmi(), r.mmi()); }
    SIMDEE_INL const not_avxu operator!=(const avxu& l, const avxu& r) { return not_avxu(_mm256_cmpeq_epi32(l.mmi(), r.mmi())); }

    SIMDEE_INL const avxu operator<(const avxs& l, const avxs& r) { return _mm256_cmpgt_epi32(r.mmi(), l.mmi()); }
    SIMDEE_INL const avxu operator>(const avxs& l, const avxs& r) { return _mm256_cmpgt_epi32(l.mmi(), r.mmi()); }
    SIMDEE_INL const not_avxu operator<=(const avxs& l, const avxs& r) { return not_avxu(_mm256_cmpgt_epi32(l.mmi(), r.mmi())); }
    SIMDEE_INL const not_avxu operator>=(const avxs& l, const avxs& r) { return not_avxu(_mm256_cmpgt_epi32(r.mmi(), l.mmi())); }
    SIMDEE_INL const avxu operator==(const avxs& l, const avxs& r) { return _mm256_cmpeq_epi32(l.mmi(), r.mmi()); }
    SIMDEE_INL const not_avxu operator!=(const avxs& l, const avxs& r) { return not_avxu(_mm256_cmpeq_epi32(l.mmi(), r.mmi())); }

    SIMDEE_INL const avxs operator-(const avxs& l) { return _mm256_sub_epi32(_mm256_setzero_si256(), l.mmi()); }
    SIMDEE_INL const avxs operator+(const avxs& l, const avxs& r) { return _mm256_add_epi32(l.mmi(), r.mmi()); }
    SIMDEE_INL const avxs operator-(const avxs& l, const avxs& r) { return _mm256_sub_epi32(l.mmi(), r.mmi()); }
    SIMDEE_INL const avxs operator*(const avxs& l, const avxs& r) { return _mm256_mullo_epi32(l.mmi(), r.mmi()); }

    SIMDEE_INL const avxs min(const avxs& l, const avxs& r) { return _mm256_min_epi32(l.mmi(), r.mmi()); }
    SIMDEE_INL const avxs max(const avxs& l, const avxs& r) { return _mm256_max_epi32(l.mmi(), r.mmi()); }
    SIMDEE_INL const avxs abs(const avxs& l) { return _mm256_abs_epi32(l.mmi()); }
#endif

}

#undef SIMDEE_AVX_COMMON_DECLARATIONS

#endif // SIMDEE_SIMD_TYPES_AVX_HPP
