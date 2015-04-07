#ifndef SIMDIFY_AVX
#define SIMDIFY_AVX

// ensure AVX is on
#ifndef __AVX__
#error "The macro '__AVX__' is not set. AVX intrinsics are required to use the AVX SIMD type. Please check your build options."
#endif

#include "common.hpp"
#include <immintrin.h>

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    // SIMD with AVX
    struct avx : simd_base<__m256, float, uint32_t, avx> {
        INL avx() {}
        INL avx(mm_t r) : simd_base(r) {}
        INL explicit avx(const fp_t& r) : simd_base(_mm256_broadcast_ss(&r)) {}
        INL explicit avx(zero_t) : simd_base(_mm256_setzero_ps()) {}
        INL explicit avx(mask_t r) : avx(r.f) {}

        INL fp_t front() const { return _mm_cvtss_f32(_mm256_castps256_ps128(mm)); }
        INL fp_t back() const { return f.back(); }
    };

    // bitwise not with lazy evaluation
    template <>
    struct bitwise_not<avx> {
        avx neg;
        INL explicit bitwise_not(const avx& r) : neg(r) {}
        INL operator const avx() const { return _mm256_andnot_ps(avx(ZERO).mm, neg.mm); }
    };

    INL const avx operator&(const avx& l, const avx& r) { return _mm256_and_ps(l.mm, r.mm); }
    INL const avx operator|(const avx& l, const avx& r) { return _mm256_or_ps(l.mm, r.mm); }
    INL const avx operator^(const avx& l, const avx& r) { return _mm256_xor_ps(l.mm, r.mm); }
    INL const bitwise_not<avx> operator~(const avx& l) { return bitwise_not<avx>(l); }
    INL const avx operator<(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LT_OQ); }
    INL const avx operator>(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GT_OQ); }
    INL const avx operator<=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LE_OQ); }
    INL const avx operator>=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GE_OQ); }
    INL const avx operator==(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_EQ_OQ); }
    INL const avx operator!=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_NEQ_OQ); }
    INL const avx operator+(const avx& l, const avx& r) { return _mm256_add_ps(l.mm, r.mm); }
    INL const avx operator-(const avx& l, const avx& r) { return _mm256_sub_ps(l.mm, r.mm); }
    INL const avx operator*(const avx& l, const avx& r) { return _mm256_mul_ps(l.mm, r.mm); }
    INL const avx operator/(const avx& l, const avx& r) { return _mm256_div_ps(l.mm, r.mm); }
    INL const avx andnot(const avx& l, const avx& r) { return _mm256_andnot_ps(l.mm, r.mm); }
    INL const avx min(const avx& l, const avx& r) { return _mm256_min_ps(l.mm, r.mm); }
    INL const avx max(const avx& l, const avx& r) { return _mm256_max_ps(l.mm, r.mm); }
    INL const avx cond(const avx& pred, const avx& if_true, const avx& if_false) { return _mm256_blendv_ps(if_false.mm, if_true.mm, pred.mm); }

    // horizontal operations
    template <>
    struct horizontal<avx> : horizontal_base<avx> {
        template <binary_op_t F>
        static INL const avx reduce_wide(const avx& in) {
            avx tmp = F(in, _mm256_permute_ps(in.mm, _MM_SHUFFLE(2, 3, 0, 1)));
            tmp = F(tmp, _mm256_permute_ps(tmp.mm, _MM_SHUFFLE(1, 0, 3, 2)));
            return F(tmp, _mm256_permute2f128_ps(tmp.mm, tmp.mm, _MM_SHUFFLE(0, 0, 0, 1)));
        }
        static INL uint find(const avx& in) {
            return ls1b(uint(_mm256_movemask_ps(in.mm)));
        }
    };

}

#undef INL

#endif // SIMDIFY_AVX
