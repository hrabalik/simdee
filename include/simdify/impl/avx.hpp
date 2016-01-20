#ifndef SIMDIFY_AVX
#define SIMDIFY_AVX

// ensure AVX is on
#ifndef __AVX__
#error "The macro '__AVX__' is not set. AVX intrinsics are required to use the AVX SIMD type. Please check your build options."
#endif

#include "common.hpp"
#include <immintrin.h>

namespace simd {

    struct avxf;
    struct avxu;
    struct avxs;

    template <>
    struct is_simd_type<avxf> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_type<avxu> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_type<avxs> : std::integral_constant<bool, true> {};

    template <typename Mm_t, typename E_t>
    struct avx_traits {
        using mm_t = Mm_t;
        using e_t = E_t;
        using f_t = float;
        using u_t = uint32_t;
        using s_t = int32_t;
        using vec_f = avxf;
        using vec_u = avxu;
        using vec_s = avxs;
    };

    template <>
    struct simd_type_traits<avxf> : avx_traits<__m256, float> {};

    template <>
    struct simd_type_traits<avxu> : avx_traits<__m256, uint32_t> {};

    template <>
    struct simd_type_traits<avxs> : avx_traits<__m256, int32_t> {};

    template <typename Crtp>
    struct avx_base : simd_base<Crtp> {
        SIMDIFY_TRIVIAL_TYPE(avx_base);

        using mm_t = typename simd_base<Crtp>::mm_t;
        using e_t = typename simd_base<Crtp>::e_t;
        using simd_base<Crtp>::mm;
        using simd_base<Crtp>::W;
        using simd_base<Crtp>::self;

        SIMDIFY_INL avx_base(const mm_t& r) {
            mm = r;
        }

        SIMDIFY_INL Crtp& operator=(const mm_t& r) {
            mm = r;
            return self();
        }

        SIMDIFY_INL avx_base(e_t r) {
            mm = _mm256_broadcast_ss((float*)&r);
        }

        SIMDIFY_INL Crtp& operator=(e_t r) {
            mm = _mm256_broadcast_ss((float*)&r);
            return self();
        }

        SIMDIFY_INL avx_base(const expr::zero& r) {
            mm = _mm256_setzero_ps();
        }

        SIMDIFY_INL Crtp& operator=(const expr::zero& r) {
            mm = _mm256_setzero_ps();
            return self();
        }

        template <typename T>
        SIMDIFY_INL avx_base(const expr::aligned<T>& r) {
            aligned_load(r.ptr);
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::aligned<T>& r) {
            aligned_load(r.ptr);
            return self();
        }

        template <typename T>
        SIMDIFY_INL avx_base(const expr::unaligned<T>& r) {
            unaligned_load(r.ptr);
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::unaligned<T>& r) {
            unaligned_load(r.ptr);
            return self();
        }

        template <typename T>
        SIMDIFY_INL avx_base(const expr::init<T>& r) {
            *this = r.template to<e_t>();
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::init<T>& r) {
            *this = r.template to<e_t>();
            return self();
        }

        SIMDIFY_INL void aligned_load(const e_t* r) {
            mm = _mm256_load_ps((const float*)r);
        }

        SIMDIFY_INL void aligned_store(e_t* r) const {
            _mm256_store_ps((float*)r, mm);
        }

        SIMDIFY_INL void unaligned_load(const e_t* r) {
            mm = _mm256_loadu_ps((const float*)r);
        }

        SIMDIFY_INL void unaligned_store(e_t* r) {
            _mm256_storeu_ps((float*)r, mm);
        }

        void interleaved_load(const e_t* r, std::size_t step) {
            alignas(avx_base)e_t temp[W];
            for (std::size_t i = 0; i < W; ++i, r += step) {
                temp[i] = *r;
            }
            aligned_load(temp);
        }

        void interleaved_store(e_t* r, std::size_t step) const {
            alignas(avx_base)e_t temp[W];
            aligned_store(temp);
            for (std::size_t i = 0; i < W; ++i, r += step) {
                *r = temp[i];
            }
        }
    };

    struct avxf : avx_base<avxf> {
        using avx_base::avx_base;

        SIMDIFY_TRIVIAL_TYPE(avxf);

        SIMDIFY_INL e_t first_element() const { return  _mm_cvtss_f32(_mm256_castps256_ps128(mm)); }
    };

    struct avxu : avx_base<avxu> {
        using avx_base::avx_base;

        SIMDIFY_TRIVIAL_TYPE(avxu);

        SIMDIFY_INL avxu(const expr::bit_not<avxu>& r) { *this = r; }

        SIMDIFY_INL avxu& operator=(const expr::bit_not<avxu>& r) {
            mm = _mm256_xor_ps(r.neg.mm, avxu(all_bits()).mm);
            return *this;
        }

        SIMDIFY_INL e_t first_element() const { return _mm_cvt_ss2si(_mm256_castps256_ps128(mm)); }

        SIMDIFY_INL bit_t front() const { return ls1b(bit_t(_mm256_movemask_ps(mm)));; }
        SIMDIFY_INL bit_iterator begin() const { return bit_iterator(_mm256_movemask_ps(mm)); }
        SIMDIFY_INL bit_iterator end() const { return bit_iterator(0); }
    };

    struct avxs : avx_base<avxs> {
        using avx_base::avx_base;

        SIMDIFY_TRIVIAL_TYPE(avxs);

        SIMDIFY_INL e_t first_element() const { return _mm_cvt_ss2si(_mm256_castps256_ps128(mm)); }
    };

    SIMDIFY_INL const avxu operator&(const avxu& l, const avxu& r) { return _mm256_and_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxu operator|(const avxu& l, const avxu& r) { return _mm256_or_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxu operator^(const avxu& l, const avxu& r) { return _mm256_xor_ps(l.mm, r.mm); }
    SIMDIFY_INL const expr::bit_not<avxu> operator~(const avxu& l) { return expr::bit_not<avxu>(l); }
    SIMDIFY_INL const avxu nand(const avxu& l, const avxu& r) { return _mm256_andnot_ps(l.mm, r.mm); }
    SIMDIFY_INL const bool any(const avxu& l) { return _mm256_movemask_ps(l.mm) != 0; }
    SIMDIFY_INL const bool all(const avxu& l) { return _mm256_movemask_ps(l.mm) == 0xFF; }

    SIMDIFY_INL const avxu operator<(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LT_OQ); }
    SIMDIFY_INL const avxu operator>(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GT_OQ); }
    SIMDIFY_INL const avxu operator<=(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LE_OQ); }
    SIMDIFY_INL const avxu operator>=(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GE_OQ); }
    SIMDIFY_INL const avxu operator==(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_EQ_OQ); }
    SIMDIFY_INL const avxu operator!=(const avxf& l, const avxf& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_NEQ_OQ); }

    SIMDIFY_INL const avxf operator-(const avxf& l) { return _mm256_xor_ps(l.mm, avxf(sign_bit()).mm); }
    SIMDIFY_INL const avxf operator+(const avxf& l, const avxf& r) { return _mm256_add_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxf operator-(const avxf& l, const avxf& r) { return _mm256_sub_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxf operator*(const avxf& l, const avxf& r) { return _mm256_mul_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxf operator/(const avxf& l, const avxf& r) { return _mm256_div_ps(l.mm, r.mm); }

    SIMDIFY_INL const avxf min(const avxf& l, const avxf& r) { return _mm256_min_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxf max(const avxf& l, const avxf& r) { return _mm256_max_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxf sqrt(const avxf& l) { return _mm256_sqrt_ps(l.mm); }
    SIMDIFY_INL const avxf rsqrt(const avxf& l) { return _mm256_rsqrt_ps(l.mm); }
    SIMDIFY_INL const avxf rcp(const avxf& l) { return _mm256_rcp_ps(l.mm); }
    SIMDIFY_INL const avxf abs(const avxf& l) { return _mm256_and_ps(l.mm, avxf(abs_mask()).mm); }

    SIMDIFY_INL const avxf cond(const avxu& pred, const avxf& if_true, const avxf& if_false) {
        return _mm256_blendv_ps(if_false.mm, if_true.mm, pred.mm);
    }
    SIMDIFY_INL const avxu cond(const avxu& pred, const avxu& if_true, const avxu& if_false) {
        return _mm256_blendv_ps(if_false.mm, if_true.mm, pred.mm);
    }
    SIMDIFY_INL const avxs cond(const avxu& pred, const avxs& if_true, const avxs& if_false) {
        return _mm256_blendv_ps(if_false.mm, if_true.mm, pred.mm);
    }

    template <typename Crtp>
    struct avx_horizontal_impl : horizontal_impl_base<Crtp> {
        using binary_op_t = typename horizontal_impl_base<Crtp>::binary_op_t;

        template <binary_op_t F>
        static SIMDIFY_INL const Crtp reduce(const Crtp& in) {
            Crtp tmp = F(in, _mm256_permute_ps(in.mm, _MM_SHUFFLE(2, 3, 0, 1)));
            tmp = F(tmp, _mm256_permute_ps(tmp.mm, _MM_SHUFFLE(1, 0, 3, 2)));
            return F(tmp, _mm256_permute2f128_ps(tmp.mm, tmp.mm, _MM_SHUFFLE(0, 0, 0, 1)));
        }
    };

    template <>
    struct horizontal_impl<avxf> : avx_horizontal_impl<avxf> {};
    template <>
    struct horizontal_impl<avxu> : avx_horizontal_impl<avxu> {};
    template <>
    struct horizontal_impl<avxs> : avx_horizontal_impl<avxs> {};
}

#undef SIMDIFY_AVX_COMMON_DECLARATIONS

#endif // SIMDIFY_AVX
