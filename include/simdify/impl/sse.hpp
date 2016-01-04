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
    struct simd_type_traits<ssef> : sse_traits<__m128, float> {};
    template <>
    struct simd_type_traits<sseu> : sse_traits<__m128, uint32_t> {};
    template <>
    struct simd_type_traits<sses> : sse_traits<__m128, int32_t> {};

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

        SIMDIFY_INL e_t first_element() const { return _mm_cvtss_f32(mm); }
    };

    struct sseu : simd_base<sseu> {

        SIMDIFY_INL e_t first_element() const { return _mm_cvt_ss2si(mm); }

        SIMDIFY_INL bit_t front() const { return ls1b(bit_t(_mm_movemask_ps(mm)));; }
        SIMDIFY_INL bit_iterator begin() const { return bit_iterator(_mm_movemask_ps(mm)); }
        SIMDIFY_INL bit_iterator end() const { return bit_iterator(0); }
    };

    struct sses : simd_base<sses> {

        SIMDIFY_INL e_t first_element() const { return _mm_cvt_ss2si(mm); }

    };

    SIMDIFY_INL const ssef operator&(const ssef& l, const ssef& r) { return _mm_and_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator|(const ssef& l, const ssef& r) { return _mm_or_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator^(const ssef& l, const ssef& r) { return _mm_xor_ps(l.mm, r.mm); }
    SIMDIFY_INL const expr::bit_not<ssef> operator~(const ssef& l) { return expr::bit_not<ssef>(l); }
    SIMDIFY_INL const ssef nand(const ssef& l, const ssef& r) { return _mm_andnot_ps(l.mm, r.mm); }
    SIMDIFY_INL const bool any(const ssef& l) { return _mm_movemask_ps(l.mm) != 0; }
    SIMDIFY_INL const bool all(const ssef& l) { return _mm_movemask_ps(l.mm) == 0xF; }

    SIMDIFY_INL const ssef operator<(const ssef& l, const ssef& r) { return _mm_cmplt_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator>(const ssef& l, const ssef& r) { return _mm_cmpgt_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator<=(const ssef& l, const ssef& r) { return _mm_cmple_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator>=(const ssef& l, const ssef& r) { return _mm_cmpge_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator==(const ssef& l, const ssef& r) { return _mm_cmpeq_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator!=(const ssef& l, const ssef& r) { return _mm_cmpneq_ps(l.mm, r.mm); }

    SIMDIFY_INL const ssef operator-(const ssef& l) { return _mm_xor_ps(l.mm, ssef(sign_bit()).mm); }
    SIMDIFY_INL const ssef operator+(const ssef& l, const ssef& r) { return _mm_add_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator-(const ssef& l, const ssef& r) { return _mm_sub_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator*(const ssef& l, const ssef& r) { return _mm_mul_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef operator/(const ssef& l, const ssef& r) { return _mm_div_ps(l.mm, r.mm); }

    SIMDIFY_INL const ssef min(const ssef& l, const ssef& r) { return _mm_min_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef max(const ssef& l, const ssef& r) { return _mm_max_ps(l.mm, r.mm); }
    SIMDIFY_INL const ssef sqrt(const ssef& l) { return _mm_sqrt_ps(l.mm); }
    SIMDIFY_INL const ssef rsqrt(const ssef& l) { return _mm_rsqrt_ps(l.mm); }
    SIMDIFY_INL const ssef rcp(const ssef& l) { return _mm_rcp_ps(l.mm); }
    SIMDIFY_INL const ssef abs(const ssef& l) { return _mm_and_ps(l.mm, ssef(abs_mask()).mm); }

    SIMDIFY_INL const ssef cond(const ssef& pred, const ssef& if_true, const ssef& if_false) { return (if_true & pred) | (if_false & ~pred); }

    // horizontal operations
    template <typename Crtp>
    struct sse_horizontal_impl : horizontal_impl_base<Crtp> {
        template <binary_op_t F>
        static SIMDIFY_INL const ssef reduce(const ssef& in) {
            ssef tmp = F(in, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(in.mm), _MM_SHUFFLE(2, 3, 0, 1))));
            return F(tmp, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp.mm), _MM_SHUFFLE(1, 0, 3, 2))));
        }
    };

    template <>
    struct horizontal_impl<ssef> : sse_horizontal_impl<ssef> {};
    template <>
    struct horizontal_impl<sseu> : sse_horizontal_impl<sseu> {};
    template <>
    struct horizontal_impl<sses> : sse_horizontal_impl<sses> {};

}

#endif // SIMDIFY_SSE
