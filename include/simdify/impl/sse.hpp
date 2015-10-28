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

namespace simd {

    // SIMD with SSE
    struct sse : simd_base<__m128, float, uint32_t, int32_t, sse> {
        SIMDIFY_FORCE_INLINE sse() {}
        SIMDIFY_FORCE_INLINE sse(mm_t r) : simd_base(r) {}
        SIMDIFY_FORCE_INLINE explicit sse(zero_t) : simd_base(_mm_setzero_ps()) {}
        SIMDIFY_FORCE_INLINE explicit sse(const F& r) : simd_base(_mm_set_ps1(r.f)) {}
        SIMDIFY_FORCE_INLINE explicit sse(const U& r) : simd_base(_mm_set_ps1(conversions::castf(r.u))) {}
        SIMDIFY_FORCE_INLINE explicit sse(const I& r) : simd_base(_mm_set_ps1(conversions::castf(r.i))) {}

        SIMDIFY_FORCE_INLINE f_t front() const { return _mm_cvtss_f32(mm); }
        SIMDIFY_FORCE_INLINE f_t back() const { return f.back(); }
    };

    // bitwise not with lazy evaluation
    template <>
    struct bitwise_not<sse> {
        sse neg;
        SIMDIFY_FORCE_INLINE explicit bitwise_not(const sse& r) : neg(r) {}
        SIMDIFY_FORCE_INLINE explicit operator const sse() const { return _mm_andnot_ps(neg.mm, sse(all_bits_t{}).mm); }
    };

    SIMDIFY_FORCE_INLINE const sse operator&(const sse& l, const sse& r) { return _mm_and_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator|(const sse& l, const sse& r) { return _mm_or_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator^(const sse& l, const sse& r) { return _mm_xor_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const bitwise_not<sse> operator~(const sse& l) { return bitwise_not<sse>(l); }
    SIMDIFY_FORCE_INLINE const sse operator<(const sse& l, const sse& r) { return _mm_cmplt_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator>(const sse& l, const sse& r) { return _mm_cmpgt_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator<=(const sse& l, const sse& r) { return _mm_cmple_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator>=(const sse& l, const sse& r) { return _mm_cmpge_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator==(const sse& l, const sse& r) { return _mm_cmpeq_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator!=(const sse& l, const sse& r) { return _mm_cmpneq_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator+(const sse& l, const sse& r) { return _mm_add_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator-(const sse& l, const sse& r) { return _mm_sub_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator*(const sse& l, const sse& r) { return _mm_mul_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator/(const sse& l, const sse& r) { return _mm_div_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse andnot(const sse& l, const sse& r) { return _mm_andnot_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse min(const sse& l, const sse& r) { return _mm_min_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse max(const sse& l, const sse& r) { return _mm_max_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse cond(const sse& pred, const sse& if_true, const sse& if_false) { return (if_true & pred) | (if_false & ~pred); }

    // horizontal operations
    template <>
    struct horizontal_impl<sse> : horizontal_impl_base<sse>{
        static SIMDIFY_FORCE_INLINE bit_field find(const sse& in) { return  uint(_mm_movemask_ps(in.mm)); }
        static SIMDIFY_FORCE_INLINE bool any(const sse& in) { return _mm_movemask_ps(in.mm) != 0; }
        static SIMDIFY_FORCE_INLINE bool all(const sse& in) { return _mm_movemask_ps(in.mm) == 0xF; }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE const sse reduce_vector(const sse& in) {
            sse tmp = F(in, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(in.mm), _MM_SHUFFLE(2, 3, 0, 1))));
            return F(tmp, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp.mm), _MM_SHUFFLE(1, 0, 3, 2))));
        }
    };

}

#endif // SIMDIFY_SSE
