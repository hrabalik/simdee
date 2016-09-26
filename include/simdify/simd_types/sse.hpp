#ifndef SIMDIFY_SIMD_TYPES_SSE_HPP
#define SIMDIFY_SIMD_TYPES_SSE_HPP

#include "common.hpp"

// set feature flag
#define SIMDIFY_HAVE_SSE

// ensure that intrinsics are available
#ifndef __SSE2__
#error "The macro '__SSE2__' is not set. SSE2 intrinsics are required to use the SSE SIMD type. Please check your build options."
#endif

#include <emmintrin.h>

// additional includes for extra instruction sets
#if defined(__SSE3__)
#include <pmmintrin.h>
#endif
#if defined(__SSSE3__)
#include <tmmintrin.h>
#endif
#if defined (__SSE4_1__)
#include <smmintrin.h>
#endif
#if defined (__SSE4_2__)
#include <nmmintrin.h>
#endif

namespace simd {

    struct ssef;
    struct sseu;
    struct sses;
    using not_sseu = expr::deferred_not<sseu>;

    template<>
    struct is_simd_type<ssef> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<sseu> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<sses> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Vector_t, typename Scalar_t>
    struct sse_traits {
        using simd_t = Simd_t;
        using vector_t = Vector_t;
        using scalar_t = Scalar_t;
        using vec_f = ssef;
        using vec_u = sseu;
        using vec_s = sses;
        using mask_t = impl::mask<0xfU>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <>
    struct simd_type_traits<ssef> : sse_traits<ssef, __m128, float> {};
    template <>
    struct simd_type_traits<sseu> : sse_traits<sseu, __m128, uint32_t> {};
    template <>
    struct simd_type_traits<sses> : sse_traits<sses, __m128, int32_t> {};

    template <typename Crtp>
    struct sse_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDIFY_TRIVIAL_TYPE(sse_base);

        SIMDIFY_INL sse_base(const vector_t& r) {
            mm = r;
        }

        SIMDIFY_INL Crtp& operator=(const vector_t& r) {
            mm = r;
            return self();
        }

        SIMDIFY_INL sse_base(scalar_t r) {
            mm = _mm_set_ps1(reinterpret_cast<float&>(r));
        }

        SIMDIFY_INL Crtp& operator=(scalar_t r) {
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

        SIMDIFY_INL sse_base(const expr::all_bits& r) {
            operator=(r);
        }

        SIMDIFY_INL Crtp& operator=(const expr::all_bits& r) {
            mm = _mm_setzero_ps();
            mm = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_castps_si128(mm), _mm_castps_si128(mm)));
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
            *this = r.template to<scalar_t>();
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::init<T>& r) {
            *this = r.template to<scalar_t>();
            return self();
        }

        SIMDIFY_INL sse_base(const storage_t& r) {
            aligned_load(r.data());
        }

        SIMDIFY_INL Crtp& operator=(const storage_t& r) {
            aligned_load(r.data());
            return self();
        }

        SIMDIFY_INL void aligned_load(const scalar_t* r) {
            mm = _mm_load_ps((const float*)r);
        }

        SIMDIFY_INL void aligned_store(scalar_t* r) const {
            _mm_store_ps((float*)r, mm);
        }

        SIMDIFY_INL void unaligned_load(const scalar_t* r) {
            mm = _mm_loadu_ps((const float*)r);
        }

        SIMDIFY_INL void unaligned_store(scalar_t* r) const {
            _mm_storeu_ps((float*)r, mm);
        }

        void interleaved_load(const scalar_t* r, std::size_t step) {
            alignas(Crtp)scalar_t temp[width];
            for (std::size_t i = 0; i < width; ++i, r += step) {
                temp[i] = *r;
            }
            aligned_load(temp);
        }

        void interleaved_store(scalar_t* r, std::size_t step) const {
            alignas(Crtp)scalar_t temp[width];
            aligned_store(temp);
            for (std::size_t i = 0; i < width; ++i, r += step) {
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
        SIMDIFY_TRIVIAL_TYPE(ssef);

        using sse_base::sse_base;
        SIMDIFY_INL explicit ssef(const sses&);

        SIMDIFY_INL scalar_t first_element() const { return _mm_cvtss_f32(mm); }
    };

    struct sseu : sse_base<sseu> {
        SIMDIFY_TRIVIAL_TYPE(sseu);

        using sse_base::sse_base;
        SIMDIFY_INL explicit sseu(const sses&);
        SIMDIFY_INL sseu(const __m128i r) { *this = r; }
        SIMDIFY_INL sseu(const not_sseu& r) { *this = r; }

        SIMDIFY_INL sseu& operator=(const __m128i r) {
            mm = _mm_castsi128_ps(r);
            return *this;
        }

        SIMDIFY_INL sseu& operator=(const not_sseu& r) {
            mm = _mm_xor_ps(r.neg.mm, sseu(all_bits()).mm);
            return *this;
        }

        SIMDIFY_INL __m128i mmi() const { return _mm_castps_si128(mm); }
        SIMDIFY_INL mask_t mask() const { return mask_t(uint32_t(_mm_movemask_ps(mm))); }
        SIMDIFY_INL scalar_t first_element() const { return tou(_mm_cvtss_f32(mm)); }
    };

    struct sses : sse_base<sses> {
        SIMDIFY_TRIVIAL_TYPE(sses);

        using sse_base::sse_base;
        SIMDIFY_INL explicit sses(const ssef&);
        SIMDIFY_INL explicit sses(const sseu&);
        SIMDIFY_INL sses(const __m128i& r) { *this = r; }

        SIMDIFY_INL sses& operator=(const __m128i& r) {
            mm = _mm_castsi128_ps(r);
            return *this;
        }

        SIMDIFY_INL __m128i mmi() const { return _mm_castps_si128(mm); }
        SIMDIFY_INL scalar_t first_element() const { return tos(_mm_cvtss_f32(mm)); }
    };

    SIMDIFY_INL ssef::ssef(const sses& r) { mm = _mm_cvtepi32_ps(_mm_castps_si128(r.data())); }
    SIMDIFY_INL sses::sses(const ssef& r) { mm = _mm_castsi128_ps(_mm_cvtps_epi32(r.data())); }
    SIMDIFY_INL sseu::sseu(const sses& r) { mm = r.data(); }
    SIMDIFY_INL sses::sses(const sseu& r) { mm = r.data(); }

    SIMDIFY_INL const sseu operator&(const sseu& l, const sseu& r) { return _mm_and_ps(l.data(), r.data()); }
    SIMDIFY_INL const sseu operator|(const sseu& l, const sseu& r) { return _mm_or_ps(l.data(), r.data()); }
    SIMDIFY_INL const sseu operator^(const sseu& l, const sseu& r) { return _mm_xor_ps(l.data(), r.data()); }
    SIMDIFY_INL const not_sseu operator~(const sseu& l) { return not_sseu(l); }
    SIMDIFY_INL const sseu andnot(const sseu& l, const sseu& r) { return _mm_andnot_ps(r.data(), l.data()); }

    SIMDIFY_INL const sseu operator<(const ssef& l, const ssef& r) { return _mm_cmplt_ps(l.data(), r.data()); }
    SIMDIFY_INL const sseu operator>(const ssef& l, const ssef& r) { return _mm_cmpgt_ps(l.data(), r.data()); }
    SIMDIFY_INL const sseu operator<=(const ssef& l, const ssef& r) { return _mm_cmple_ps(l.data(), r.data()); }
    SIMDIFY_INL const sseu operator>=(const ssef& l, const ssef& r) { return _mm_cmpge_ps(l.data(), r.data()); }
    SIMDIFY_INL const sseu operator==(const ssef& l, const ssef& r) { return _mm_cmpeq_ps(l.data(), r.data()); }
    SIMDIFY_INL const sseu operator!=(const ssef& l, const ssef& r) { return _mm_cmpneq_ps(l.data(), r.data()); }

    SIMDIFY_INL const ssef operator-(const ssef& l) { return _mm_xor_ps(l.data(), ssef(sign_bit()).data()); }
    SIMDIFY_INL const ssef operator+(const ssef& l, const ssef& r) { return _mm_add_ps(l.data(), r.data()); }
    SIMDIFY_INL const ssef operator-(const ssef& l, const ssef& r) { return _mm_sub_ps(l.data(), r.data()); }
    SIMDIFY_INL const ssef operator*(const ssef& l, const ssef& r) { return _mm_mul_ps(l.data(), r.data()); }
    SIMDIFY_INL const ssef operator/(const ssef& l, const ssef& r) { return _mm_div_ps(l.data(), r.data()); }

    SIMDIFY_INL const ssef min(const ssef& l, const ssef& r) { return _mm_min_ps(l.data(), r.data()); }
    SIMDIFY_INL const ssef max(const ssef& l, const ssef& r) { return _mm_max_ps(l.data(), r.data()); }
    SIMDIFY_INL const ssef sqrt(const ssef& l) { return _mm_sqrt_ps(l.data()); }
    SIMDIFY_INL const ssef rsqrt(const ssef& l) { return _mm_rsqrt_ps(l.data()); }
    SIMDIFY_INL const ssef rcp(const ssef& l) { return _mm_rcp_ps(l.data()); }
    SIMDIFY_INL const ssef abs(const ssef& l) { return _mm_and_ps(l.data(), ssef(abs_mask()).data()); }

    SIMDIFY_INL const ssef cond(const sseu& pred, const ssef& if_true, const ssef& if_false) {
        return tof((tou(if_true) & pred) | (tou(if_false) & ~pred));
    }

    SIMDIFY_INL const sseu cond(const sseu& pred, const sseu& if_true, const sseu& if_false) {
        return tou((tou(if_true) & pred) | (tou(if_false) & ~pred));
    }

    SIMDIFY_INL const sses cond(const sseu& pred, const sses& if_true, const sses& if_false) {
        return tos((tou(if_true) & pred) | (tou(if_false) & ~pred));
    }

#if defined(SIMDIFY_NEED_INT)
    SIMDIFY_INL const sseu operator==(const sseu& l, const sseu& r) { return _mm_cmpeq_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const not_sseu operator!=(const sseu& l, const sseu& r) { return not_sseu(_mm_cmpeq_epi32(l.mmi(), r.mmi())); }

    SIMDIFY_INL const sseu operator<(const sses& l, const sses& r) { return _mm_cmplt_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const sseu operator>(const sses& l, const sses& r) { return _mm_cmpgt_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const not_sseu operator<=(const sses& l, const sses& r) { return not_sseu(_mm_cmpgt_epi32(l.mmi(), r.mmi())); }
    SIMDIFY_INL const not_sseu operator>=(const sses& l, const sses& r) { return not_sseu(_mm_cmplt_epi32(l.mmi(), r.mmi())); }
    SIMDIFY_INL const sseu operator==(const sses& l, const sses& r) { return _mm_cmpeq_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const not_sseu operator!=(const sses& l, const sses& r) { return not_sseu(_mm_cmpeq_epi32(l.mmi(), r.mmi())); }

    SIMDIFY_INL const sses operator-(const sses& l) { return _mm_sub_epi32(_mm_setzero_si128(), l.mmi()); }
    SIMDIFY_INL const sses operator+(const sses& l, const sses& r) { return _mm_add_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const sses operator-(const sses& l, const sses& r) { return _mm_sub_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const sses operator*(const sses& l, const sses& r) { return _mm_mullo_epi32(l.mmi(), r.mmi()); }

    SIMDIFY_INL const sses min(const sses& l, const sses& r) { return _mm_min_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const sses max(const sses& l, const sses& r) { return _mm_max_epi32(l.mmi(), r.mmi()); }
    SIMDIFY_INL const sses abs(const sses& l) { return _mm_abs_epi32(l.mmi()); }
#endif

}

#undef SIMDIFY_SSE_COMMON_DECLARATIONS

#endif // SIMDIFY_SIMD_TYPES_SSE_HPP
