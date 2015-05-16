#ifndef SIMDIFY_SSE
#define SIMDIFY_SSE

// ensure SSE2 is on
#ifndef __SSE2__
#error "The macro '__SSE2__' is not set. SSE2 intrinsics are required to use the SSE SIMD type. Please check your build options."
#endif

#include "common.hpp"
#include <emmintrin.h>

//#if defined(__SSE3__)
//#include <pmmintrin.h>
//#endif
//#if defined(__SSSE3__)
//#include <tmmintrin.h>
//#endif
//#if defined (__SSE4_1__)
//#include <smmintrin.h>
//#endif
//#if defined (__SSE4_2__)
//#include <nmmintrin.h>
//#endif

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    // SIMD with SSE
    struct sse : simd_base<__m128, float, uint32_t, sse> {
        INL sse() {}
        INL sse(mm_t r) : simd_base(r) {}
        INL explicit sse(fp_t r) : simd_base(_mm_set_ps1(r)) {}
        INL explicit sse(zero_t) : simd_base(_mm_setzero_ps()) {}
        INL explicit sse(mask_t r) : sse(r.f) {}

        INL fp_t front() const { return _mm_cvtss_f32(mm); }
        INL fp_t back() const { return f.back(); }
    };

    // bitwise not with lazy evaluation
    template <>
    struct bitwise_not<sse> {
        sse neg;
        INL explicit bitwise_not(const sse& r) : neg(r) {}
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
    struct horizontal_impl<sse> : horizontal_impl_base<sse>{
        static INL bit_field find(const sse& in) { return  uint(_mm_movemask_ps(in.mm)); }
        static INL bool any(const sse& in) { return _mm_movemask_ps(in.mm) != 0; }
        static INL bool all(const sse& in) { return _mm_movemask_ps(in.mm) == 0xF; }

        template <binary_op_t F>
        static INL const sse reduce_vector(const sse& in) {
            sse tmp = F(in, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(in.mm), _MM_SHUFFLE(2, 3, 0, 1))));
            return F(tmp, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp.mm), _MM_SHUFFLE(1, 0, 3, 2))));
        }
    };

}

#undef INL

#endif // SIMDIFY_SSE
