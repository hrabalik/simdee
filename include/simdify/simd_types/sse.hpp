#ifndef SIMDIFY_SIMD_TYPES_SSE_HPP
#define SIMDIFY_SIMD_TYPES_SSE_HPP
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

    template <typename Crtp>
    struct sse_base : simd_base<Crtp> {
        SIMDIFY_TRIVIAL_TYPE(sse_base);

        using mm_t = typename simd_base<Crtp>::mm_t;
        using e_t = typename simd_base<Crtp>::e_t;
        using simd_base<Crtp>::mm;
        using simd_base<Crtp>::W;
        using simd_base<Crtp>::self;

        SIMDIFY_INL sse_base(const mm_t& r) {
            mm = r;
        }

        SIMDIFY_INL Crtp& operator=(const mm_t& r) {
            mm = r;
            return self();
        }

        SIMDIFY_INL sse_base(e_t r) {
            mm = _mm_set_ps1(reinterpret_cast<float&>(r));
        }

        SIMDIFY_INL Crtp& operator=(e_t r) {
            mm = _mm_set_ps1(reinterpret_cast<float&>(r));
            return self();
        }

        SIMDIFY_INL sse_base(const expr::zero& r) {
            mm = _mm_setzero_ps();
        }

        SIMDIFY_INL Crtp& operator=(const expr::zero& r) {
            mm = _mm_setzero_ps();
            return self();
        }

        template <typename T>
        SIMDIFY_INL sse_base(const expr::aligned<T>& r) {
            aligned_load(r.ptr);
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::aligned<T>& r) {
            aligned_load(r.ptr);
            return self();
        }

        template <typename T>
        SIMDIFY_INL sse_base(const expr::unaligned<T>& r) {
            unaligned_load(r.ptr);
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::unaligned<T>& r) {
            unaligned_load(r.ptr);
            return self();
        }

        template <typename T>
        SIMDIFY_INL sse_base(const expr::init<T>& r) {
            *this = r.template to<e_t>();
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::init<T>& r) {
            *this = r.template to<e_t>();
            return self();
        }

        SIMDIFY_INL void aligned_load(const e_t* r) {
            mm = _mm_load_ps((const float*)r);
        }

        SIMDIFY_INL void aligned_store(e_t* r) const {
            _mm_store_ps((float*)r, mm);
        }

        SIMDIFY_INL void unaligned_load(const e_t* r) {
            mm = _mm_loadu_ps((const float*)r);
        }

        SIMDIFY_INL void unaligned_store(e_t* r) {
            _mm_storeu_ps((float*)r, mm);
        }

        void interleaved_load(const e_t* r, std::size_t step) {
            alignas(Crtp)e_t temp[W];
            for (std::size_t i = 0; i < W; ++i, r += step) {
                temp[i] = *r;
            }
            aligned_load(temp);
        }

        void interleaved_store(e_t* r, std::size_t step) const {
            alignas(Crtp)e_t temp[W];
            aligned_store(temp);
            for (std::size_t i = 0; i < W; ++i, r += step) {
                *r = temp[i];
            }
        }

        using binary_op_t = const Crtp(*)(const Crtp& l, const Crtp& r);
        SIMDIFY_INL const Crtp reduce(binary_op_t f) const {
            Crtp tmp = f(self(), _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(mm), _MM_SHUFFLE(2, 3, 0, 1))));
            return f(tmp, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp.mm), _MM_SHUFFLE(1, 0, 3, 2))));
        }
    };

    struct ssef : sse_base<ssef> {
        using sse_base::sse_base;

        SIMDIFY_TRIVIAL_TYPE(ssef);

        SIMDIFY_INL e_t first_element() const { return _mm_cvtss_f32(mm); }
    };

    struct sseu : sse_base<sseu> {
        using sse_base::sse_base;

        SIMDIFY_TRIVIAL_TYPE(sseu);

        SIMDIFY_INL sseu(const expr::bit_not<sseu>& r) { *this = r; }

        SIMDIFY_INL sseu& operator=(const expr::bit_not<sseu>& r) {
            mm = _mm_xor_ps(r.neg.mm, sseu(all_bits()).mm);
            return *this;
        }

        SIMDIFY_INL e_t first_element() const { return _mm_cvt_ss2si(mm); }

        SIMDIFY_INL bit_t front() const { return ls1b(bit_t(_mm_movemask_ps(mm)));; }
        SIMDIFY_INL bit_iterator begin() const { return bit_iterator(_mm_movemask_ps(mm)); }
        SIMDIFY_INL bit_iterator end() const { return bit_iterator(0); }
        SIMDIFY_INL bool any() const { return _mm_movemask_ps(mm) != 0; }
        SIMDIFY_INL bool all() const { return _mm_movemask_ps(mm) == 0xF; }
    };

    struct sses : sse_base<sses> {
        using sse_base::sse_base;

        SIMDIFY_TRIVIAL_TYPE(sses);

        SIMDIFY_INL e_t first_element() const { return _mm_cvt_ss2si(mm); }
    };

    SIMDIFY_INL const sseu operator&(const sseu& l, const sseu& r) { return _mm_and_ps(l.mm, r.mm); }
    SIMDIFY_INL const sseu operator|(const sseu& l, const sseu& r) { return _mm_or_ps(l.mm, r.mm); }
    SIMDIFY_INL const sseu operator^(const sseu& l, const sseu& r) { return _mm_xor_ps(l.mm, r.mm); }
    SIMDIFY_INL const expr::bit_not<sseu> operator~(const sseu& l) { return expr::bit_not<sseu>(l); }
    SIMDIFY_INL const sseu nand(const sseu& l, const sseu& r) { return _mm_andnot_ps(l.mm, r.mm); }

    SIMDIFY_INL const sseu operator<(const ssef& l, const ssef& r) { return _mm_cmplt_ps(l.mm, r.mm); }
    SIMDIFY_INL const sseu operator>(const ssef& l, const ssef& r) { return _mm_cmpgt_ps(l.mm, r.mm); }
    SIMDIFY_INL const sseu operator<=(const ssef& l, const ssef& r) { return _mm_cmple_ps(l.mm, r.mm); }
    SIMDIFY_INL const sseu operator>=(const ssef& l, const ssef& r) { return _mm_cmpge_ps(l.mm, r.mm); }
    SIMDIFY_INL const sseu operator==(const ssef& l, const ssef& r) { return _mm_cmpeq_ps(l.mm, r.mm); }
    SIMDIFY_INL const sseu operator!=(const ssef& l, const ssef& r) { return _mm_cmpneq_ps(l.mm, r.mm); }

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

    SIMDIFY_INL const ssef cond(const sseu& pred, const ssef& if_true, const ssef& if_false) {
        return tof((tou(if_true) & pred) | (tou(if_false) & ~pred));
    }

    SIMDIFY_INL const sseu cond(const sseu& pred, const sseu& if_true, const sseu& if_false) {
        return tou((tou(if_true) & pred) | (tou(if_false) & ~pred));
    }

    SIMDIFY_INL const sses cond(const sseu& pred, const sses& if_true, const sses& if_false) {
        return tos((tou(if_true) & pred) | (tou(if_false) & ~pred));
    }
}

#undef SIMDIFY_SSE_COMMON_DECLARATIONS

#endif // SIMDIFY_SIMD_TYPES_SSE_HPP
