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

    struct ssef;
    struct sseu;
    struct sses;

    template<>
    struct is_simd_type<ssef> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<sseu> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<sses> : std::integral_constant<bool, true> {};

    template <typename Mm_t, typename E_t>
    struct sse_traits {
        using mm_t = Mm_t;
        using e_t = E_t;
        using f_t = float;
        using u_t = uint32_t;
        using s_t = int32_t;
        using vec_f = ssef;
        using vec_u = sseu;
        using vec_s = sses;
    };

    template <>
    struct simd_type_traits<ssef> : sse_traits<__m128, float>{};
    template <>
    struct simd_type_traits<sseu> : sse_traits<__m128i, uint32_t> {};
    template <>
    struct simd_type_traits<sses> : sse_traits<__m128i, int32_t> {};

    // SIMD with SSE
    struct ssef : simd_base<ssef> {
        SIMDIFY_INL ~ssef() = default;

        SIMDIFY_INL ssef() = default;
        SIMDIFY_INL ssef(const ssef&) = default;
        SIMDIFY_INL ssef(const mm_t& r) { *this = r; }
        SIMDIFY_INL ssef(e_t r) { *this = r; }
        SIMDIFY_INL ssef(const expr::zero& r) { *this = r; }
        SIMDIFY_INL ssef(const expr::bit_not<ssef>& r) { *this = r; }
        template <typename T>
        SIMDIFY_INL ssef(const expr::aligned<T>& r) { *this = r; }
        template <typename T>
        SIMDIFY_INL ssef(const expr::unaligned<T>& r) { *this = r; }
        template <typename T>
        SIMDIFY_INL ssef(const expr::init<T>& r) { *this = r; }

        SIMDIFY_INL ssef& operator=(const ssef&) = default;
        SIMDIFY_INL ssef& operator=(const mm_t& r) { mm = r; return *this; }
        SIMDIFY_INL ssef& operator=(e_t r) { mm = _mm_set_ps1(r); return *this; }
        SIMDIFY_INL ssef& operator=(const expr::zero& r) { mm = _mm_setzero_ps(); return *this; }
        SIMDIFY_INL ssef& operator=(const expr::bit_not<ssef>& r) { mm = _mm_andnot_ps(r.neg.mm, ssef(all_bits()).mm); return *this; }
        template <typename T>
        SIMDIFY_INL ssef& operator=(const expr::aligned<T>& r) { mm = _mm_load_ps(r.ptr); return *this; }
        template <typename T>
        SIMDIFY_INL ssef& operator=(const expr::unaligned<T>& r) { mm = _mm_loadu_ps(r.ptr); return *this; }
        template <typename T>
        SIMDIFY_INL ssef& operator=(const expr::init<T>& r) { *this = r.template to<e_t>(); return *this; }

        SIMDIFY_INL void aligned_load(const e_t* r) { mm = _mm_load_ps(r); }
        SIMDIFY_INL void aligned_store(e_t* r) const { _mm_store_ps(r, mm); }
        SIMDIFY_INL void unaligned_load(const e_t* r) { mm = _mm_loadu_ps(r); }
        SIMDIFY_INL void unaligned_store(e_t* r) const { _mm_storeu_ps(r, mm); }

        void interleaved_load(const e_t* r, std::size_t step) {
            alignas(ssef)e_t temp[W];
            for (std::size_t i = 0; i < W; ++i, r += step) {
                temp[i] = *r;
            }
            aligned_load(temp);
        }

        void interleaved_store(e_t* r, std::size_t step) {
            alignas(ssef)e_t temp[W];
            aligned_store(temp);
            for (std::size_t i = 0; i < W; ++i, r += step) {
                *r = temp[i];
            }
        }
    };

    struct sseu : simd_base<sseu> {

    };

    struct sses : simd_base<sses> {

    };

    SIMDIFY_INL const ssef operator&(const ssef& l, const ssef& r) { return _mm_and_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator|(const ssef& l, const ssef& r) { return _mm_or_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator^(const ssef& l, const ssef& r) { return _mm_xor_ps(l.mm, r.mm); }
    SIMDIFY_INL const expr::bit_not<ssef> operator~(const ssef& l) { return expr::bit_not<ssef>(l); }
    SIMDIFY_INL const ssef operator<(const ssef& l, const ssef& r) { return _mm_cmplt_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator>(const ssef& l, const ssef& r) { return _mm_cmpgt_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator<=(const ssef& l, const ssef& r) { return _mm_cmple_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator>=(const ssef& l, const ssef& r) { return _mm_cmpge_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator==(const ssef& l, const ssef& r) { return _mm_cmpeq_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator!=(const ssef& l, const ssef& r) { return _mm_cmpneq_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator+(const ssef& l, const ssef& r) { return _mm_add_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator-(const ssef& l, const ssef& r) { return _mm_sub_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator*(const ssef& l, const ssef& r) { return _mm_mul_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator/(const ssef& l, const ssef& r) { return _mm_div_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator-(const ssef& in) { return in ^ sign_bit(); }
    SIMDIFY_INL const ssef nand(const ssef& l, const ssef& r) { return _mm_andnot_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef min(const ssef& l, const ssef& r) { return _mm_min_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef max(const ssef& l, const ssef& r) { return _mm_max_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef cond(const ssef& pred, const ssef& if_true, const ssef& if_false) { return (if_true & pred) | (if_false & ~pred); }

    // horizontal operations
    template <>
    struct horizontal_impl<ssef> : horizontal_impl_base<ssef> {
        static SIMDIFY_INL bit_field find(const ssef& in) { return  bit_field(bit_t(_mm_movemask_ps(in.mm))); }
        static SIMDIFY_INL bool any(const ssef& in) { return _mm_movemask_ps(in.mm) != 0; }
        static SIMDIFY_INL bool all(const ssef& in) { return _mm_movemask_ps(in.mm) == 0xF; }

        template <binary_op_t F>
        static SIMDIFY_INL const ssef reduce_vector(const ssef& in) {
            ssef tmp = F(in, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(in.mm), _MM_SHUFFLE(2, 3, 0, 1))));
            return F(tmp, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp.mm), _MM_SHUFFLE(1, 0, 3, 2))));
        }
    };

}

#endif // SIMDIFY_SSE
