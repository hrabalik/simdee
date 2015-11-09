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
    struct sse : simd_base<__m128, float, sse> {
        SIMDIFY_FORCE_INLINE ~sse() = default;
        SIMDIFY_FORCE_INLINE sse() = default;
        SIMDIFY_FORCE_INLINE sse(const sse&) = default;
        SIMDIFY_FORCE_INLINE sse(sse&&) = default;
        SIMDIFY_FORCE_INLINE sse& operator=(const sse&) = default;
        SIMDIFY_FORCE_INLINE sse& operator=(sse&&) = default;

        SIMDIFY_FORCE_INLINE sse(mm_t r) : simd_base(r) {}
        SIMDIFY_FORCE_INLINE sse(f_t r) : simd_base(_mm_set_ps1(r)) {}
        SIMDIFY_FORCE_INLINE sse(const expr::zero&) : simd_base(_mm_setzero_ps()) {}
        SIMDIFY_FORCE_INLINE sse(const expr::bit_not<sse>& r) : simd_base(_mm_andnot_ps(r.neg.mm, sse(all_bits()).mm)) {}
        SIMDIFY_FORCE_INLINE void load(const f_t* r) { mm = _mm_load_ps(r); }
        SIMDIFY_FORCE_INLINE void store(f_t* r) const { _mm_store_ps(r, mm); }
        SIMDIFY_FORCE_INLINE f_t front() const { return _mm_cvtss_f32(mm); }
        SIMDIFY_FORCE_INLINE f_t back() const { return f.back(); }

        template <typename T>
        SIMDIFY_FORCE_INLINE sse(const expr::aligned<T>& r) : simd_base(_mm_load_ps(r.template get_load<f_t>())) {}
        template <typename T>
        SIMDIFY_FORCE_INLINE sse(const expr::unaligned<T>& r) : simd_base(_mm_loadu_ps(r.template get_load<f_t>())) {}
        template <typename T>
        SIMDIFY_FORCE_INLINE sse(const expr::tof<T>& r) : sse(r.template to<f_t>()) {}
    };

    SIMDIFY_FORCE_INLINE const sse operator&(const sse& l, const sse& r) { return _mm_and_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator|(const sse& l, const sse& r) { return _mm_or_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse operator^(const sse& l, const sse& r) { return _mm_xor_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const expr::bit_not<sse> operator~(const sse& l) { return expr::bit_not<sse>(l); }
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
    SIMDIFY_FORCE_INLINE const sse operator-(const sse& in) { return in ^ sign_bit(); }
    SIMDIFY_FORCE_INLINE const sse andnot(const sse& l, const sse& r) { return _mm_andnot_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse min(const sse& l, const sse& r) { return _mm_min_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse max(const sse& l, const sse& r) { return _mm_max_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const sse cond(const sse& pred, const sse& if_true, const sse& if_false) { return (if_true & pred) | (if_false & ~pred); }

    // horizontal operations
    template <>
    struct horizontal_impl<sse> : horizontal_impl_base<sse> {
        static SIMDIFY_FORCE_INLINE bit_field find(const sse& in) { return  bit_field(bit_t(_mm_movemask_ps(in.mm))); }
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
