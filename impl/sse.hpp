#ifndef SIMDIFY_SSE
#define SIMDIFY_SSE

// ensure SSE is on
#ifndef __SSE2__
#error "SSE2 intrinsics are required to include sse.hpp. Please check your build options."
#endif

#include "common.hpp"
#include <emmintrin.h>

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    // SIMD with SSE
    struct sse {
        using mm_t = __m128;
        using float_t = float;
        using bitmask_t = uint;
        using mask_t = mask<sse>;
        static const auto W = sizeof(mm_t) / sizeof(float_t);
        using array_t = std::array<float_t, W>;
        using iterator = array_t::iterator;
        union { mm_t mm; array_t f; };
        using const_iterator = array_t::const_iterator;
        INL sse() {}
        INL sse(mm_t r) : mm(r) {}
        INL sse(const sse& r) : mm(r.mm) {}
        INL explicit sse(float_t r) : mm(_mm_set_ps1(r)) {}
        INL explicit sse(zero_t) : mm(_mm_setzero_ps()) {}
        INL explicit sse(all_bits_t) : sse(mask_t(ALL_BITS)) {}
        INL explicit sse(abs_mask_t) : sse(mask_t(ABS_MASK)) {}
        INL explicit sse(sign_bit_t) : sse(mask_t(SIGN_BIT)) {}
        INL explicit sse(mask_t r) : mm(_mm_castsi128_ps(_mm_set1_epi32(r.b))) {}
        INL sse(std::initializer_list<float_t> r) { std::copy(r.begin(), r.end(), f.begin()); }
        INL sse& operator=(const sse& r) { mm = r.mm; return *this; }
        INL iterator begin() { return f.begin(); }
        INL iterator end() { return f.end(); }
        INL const_iterator cbegin() const { return f.cbegin(); }
        INL const_iterator cend() const { return f.cend(); }
        INL float_t front() const { return _mm_cvtss_f32(mm); }
        INL float_t back() const { return f.back(); }
    };

    // bitwise not with lazy evaluation
    template <>
    struct bitwise_not<sse> {
        sse neg;
        INL bitwise_not(const sse& r) : neg(r) {}
        INL operator const sse() const { return _mm_andnot_ps(sse(ZERO).mm, neg.mm); }
    };

    INL const sse operator&(const sse& l, const sse& r) { return _mm_and_ps(l.mm, r.mm); }
    INL const sse operator|(const sse& l, const sse& r) { return _mm_or_ps(l.mm, r.mm); }
    INL const sse operator^(const sse& l, const sse& r) { return _mm_xor_ps(l.mm, r.mm); }
    INL const bitwise_not<sse> operator~(const sse& l) { return bitwise_not<sse>(l); }
    INL const sse operator<(const sse& l, const sse& r) { return _mm_cmplt_ps(l.mm, r.mm); }
    INL const sse operator>(const sse& l, const sse& r) { return _mm_cmpgt_ps(l.mm, r.mm); }
    INL const sse operator<=(const sse& l, const sse& r) { return _mm_cmple_ps(l.mm, r.mm); }
    INL const sse operator>=(const sse& l, const sse& r) { return _mm_cmpge_ps(l.mm, r.mm); }
    INL const sse operator==(const sse& l, const sse& r) { return _mm_cmpeq_ps(l.mm, r.mm); }
    INL const sse operator!=(const sse& l, const sse& r) { return _mm_cmpneq_ps(l.mm, r.mm); }
    INL const sse operator+(const sse& l, const sse& r) { return _mm_add_ps(l.mm, r.mm); }
    INL const sse operator-(const sse& l, const sse& r) { return _mm_sub_ps(l.mm, r.mm); }
    INL const sse operator*(const sse& l, const sse& r) { return _mm_mul_ps(l.mm, r.mm); }
    INL const sse operator/(const sse& l, const sse& r) { return _mm_div_ps(l.mm, r.mm); }
    INL const sse andnot(const sse& l, const sse& r) { return _mm_andnot_ps(l.mm, r.mm); }
    INL const sse min(const sse& l, const sse& r) { return _mm_min_ps(l.mm, r.mm); }
    INL const sse max(const sse& l, const sse& r) { return _mm_max_ps(l.mm, r.mm); }
    INL const sse cond(const sse& pred, const sse& if_true, const sse& if_false) { return (if_true & pred) | (if_false & ~pred); }

    // horizontal operations
    template <>
    struct horizontal<sse> : horizontal_base<sse> {
        template <binary_op_t F>
        static INL const sse reduce_wide(const sse& in) {
            sse tmp = F(in, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(in.mm), _MM_SHUFFLE(2, 3, 0, 1))));
            return F(tmp, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp.mm), _MM_SHUFFLE(1, 0, 3, 2))));
        }
        static INL uint find(const sse& in) {
            return ls1b(uint(_mm_movemask_ps(in.mm)));
        }
    };

}

#undef INL

#endif // SIMDIFY_SSE
