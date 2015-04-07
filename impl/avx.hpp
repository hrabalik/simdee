#ifndef SIMDIFY_AVX
#define SIMDIFY_AVX

// ensure AVX is on
#ifndef __AVX__
#error "AVX intrinsics are required to include avx.hpp. Please check your build options."
#endif

#include "common.hpp"
#include <immintrin.h>

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    // SIMD with AVX
    struct avx {
        using mm_t = __m256;
        using float_t = float;
        using bitmask_t = uint;
        using mask_t = mask<avx>;
        static const auto W = sizeof(mm_t) / sizeof(float_t);
        using array_t = std::array<float_t, W>;
        using iterator = array_t::iterator;
        using const_iterator = array_t::const_iterator;
        union { mm_t mm; array_t f; };
        INL avx() {}
        INL avx(mm_t r) : mm(r) {}
        INL avx(const avx& r) : mm(r.mm) {}
        INL explicit avx(const float_t& r) : mm(_mm256_broadcast_ss(&r)) {}
        INL explicit avx(zero_t) : mm(_mm256_setzero_ps()) {}
        INL explicit avx(all_bits_t) : avx(mask_t(ALL_BITS)) {}
        INL explicit avx(abs_mask_t) : avx(mask_t(ABS_MASK)) {}
        INL explicit avx(sign_bit_t) : avx(mask_t(SIGN_BIT)) {}
        INL explicit avx(mask_t r) : mm(_mm256_castsi256_ps(_mm256_set1_epi32(r.b))) {}
        INL avx(std::initializer_list<float_t> r) { std::copy(r.begin(), r.end(), f.begin()); }
        INL avx& operator=(const avx& r) { mm = r.mm; return *this; }
        INL iterator begin() { return f.begin(); }
        INL iterator end() { return f.end(); }
        INL const_iterator cbegin() const { return f.cbegin(); }
        INL const_iterator cend() const { return f.cend(); }
        INL float_t front() const { return _mm_cvtss_f32(_mm256_castps256_ps128(mm)); }
        INL float_t back() const { return f.back(); }
    };

    // bitwise not with lazy evaluation
    template <>
    struct bitwise_not<avx> {
        avx neg;
        INL bitwise_not(const avx& r) : neg(r) {}
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
