#ifndef SIMDIFY_AVX
#define SIMDIFY_AVX

// ensure AVX is on
#ifndef __AVX__
#error "The macro '__AVX__' is not set. AVX intrinsics are required to use the AVX SIMD type. Please check your build options."
#endif

#include "common.hpp"
#include <immintrin.h>

namespace simd {

    // SIMD with AVX
    struct avx : simd_base<__m256, float, uint32_t, int32_t, avx> {
        SIMDIFY_FORCE_INLINE avx() {}
        SIMDIFY_FORCE_INLINE avx(mm_t r) : simd_base(r) {}
        SIMDIFY_FORCE_INLINE avx(const f_t* r) : simd_base(_mm256_load_ps(r)) {}
        SIMDIFY_FORCE_INLINE explicit avx(zero_t) : simd_base(_mm256_setzero_ps()) {}
        SIMDIFY_FORCE_INLINE explicit avx(const F& r) : simd_base(_mm256_broadcast_ss(&r.f)) {}
        SIMDIFY_FORCE_INLINE explicit avx(const U& r) : simd_base(_mm256_broadcast_ss(&conversions::castf(r.u))) {}
        SIMDIFY_FORCE_INLINE explicit avx(const I& r) : simd_base(_mm256_broadcast_ss(&conversions::castf(r.i))) {}

        SIMDIFY_FORCE_INLINE f_t front() const { return _mm_cvtss_f32(_mm256_castps256_ps128(mm)); }
        SIMDIFY_FORCE_INLINE f_t back() const { return f.back(); }
    };

    // bitwise not with lazy evaluation
    template <>
    struct bitwise_not<avx> {
        avx neg;
        SIMDIFY_FORCE_INLINE explicit bitwise_not(const avx& r) : neg(r) {}
        SIMDIFY_FORCE_INLINE explicit operator const avx() const { return _mm256_andnot_ps(neg.mm, avx(all_bits_t{}).mm); }
    };

    SIMDIFY_FORCE_INLINE const avx operator&(const avx& l, const avx& r) { return _mm256_and_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator|(const avx& l, const avx& r) { return _mm256_or_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator^(const avx& l, const avx& r) { return _mm256_xor_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const bitwise_not<avx> operator~(const avx& l) { return bitwise_not<avx>(l); }
    SIMDIFY_FORCE_INLINE const avx operator<(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LT_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator>(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GT_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator<=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LE_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator>=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GE_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator==(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_EQ_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator!=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_NEQ_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator+(const avx& l, const avx& r) { return _mm256_add_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator-(const avx& l, const avx& r) { return _mm256_sub_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator*(const avx& l, const avx& r) { return _mm256_mul_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator/(const avx& l, const avx& r) { return _mm256_div_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx andnot(const avx& l, const avx& r) { return _mm256_andnot_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx min(const avx& l, const avx& r) { return _mm256_min_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx max(const avx& l, const avx& r) { return _mm256_max_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx cond(const avx& pred, const avx& if_true, const avx& if_false) { return _mm256_blendv_ps(if_false.mm, if_true.mm, pred.mm); }

    // horizontal operations
    template <>
    struct horizontal_impl<avx> : horizontal_impl_base<avx>{
        static SIMDIFY_FORCE_INLINE bit_field find(const avx& in) { return uint(_mm256_movemask_ps(in.mm)); }
        static SIMDIFY_FORCE_INLINE bool any(const avx& in) { return _mm256_movemask_ps(in.mm) != 0; }
        static SIMDIFY_FORCE_INLINE bool all(const avx& in) { return _mm256_movemask_ps(in.mm) == 0xFF; }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE const avx reduce_vector(const avx& in) {
            avx tmp = F(in, _mm256_permute_ps(in.mm, _MM_SHUFFLE(2, 3, 0, 1)));
            tmp = F(tmp, _mm256_permute_ps(tmp.mm, _MM_SHUFFLE(1, 0, 3, 2)));
            return F(tmp, _mm256_permute2f128_ps(tmp.mm, tmp.mm, _MM_SHUFFLE(0, 0, 0, 1)));
        }
    };

}

#endif // SIMDIFY_AVX
