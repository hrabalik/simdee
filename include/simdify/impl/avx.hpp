#ifndef SIMDIFY_AVX
#define SIMDIFY_AVX

// ensure AVX is on
#ifndef __AVX__
#error "The macro '__AVX__' is not set. AVX intrinsics are required to use the AVX SIMD type. Please check your build options."
#endif

#include "common.hpp"
#include <immintrin.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_AVX_COMMON_DECLARATIONS( CLASS, OPNAME, PTR )                                            \
                                                                                                         \
SIMDIFY_INL ~CLASS() = default;                                                                          \
                                                                                                         \
SIMDIFY_INL CLASS() = default;                                                                           \
                                                                                                         \
SIMDIFY_INL CLASS(const CLASS&) = default;                                                               \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(const CLASS&) = default;                                                    \
                                                                                                         \
SIMDIFY_INL CLASS(const mm_t& r) {                                                                       \
    mm = r;                                                                                              \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(const mm_t& r) {                                                            \
    mm = r;                                                                                              \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL CLASS(e_t r) {                                                                               \
    mm = _mm256_broadcast_ss((PTR*)&r);                                                                  \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(e_t r) {                                                                    \
    mm = _mm256_broadcast_ss((PTR*)&r);                                                                  \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL CLASS(const expr::zero& r) {                                                                 \
    mm = _mm256_setzero_##OPNAME();                                                                      \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(const expr::zero& r) {                                                      \
    mm = _mm256_setzero_##OPNAME();                                                                      \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS(const expr::aligned<T>& r) {                                                           \
    aligned_load(r.ptr);                                                                                 \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS& operator=(const expr::aligned<T>& r) {                                                \
    aligned_load(r.ptr);                                                                                 \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS(const expr::unaligned<T>& r) {                                                         \
    unaligned_load(r.ptr);                                                                               \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS& operator=(const expr::unaligned<T>& r) {                                              \
    unaligned_load(r.ptr);                                                                               \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS(const expr::init<T>& r) {                                                              \
    *this = r.template to<e_t>();                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS& operator=(const expr::init<T>& r) {                                                   \
    *this = r.template to<e_t>();                                                                        \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void aligned_load(const e_t* r) {                                                            \
    mm = _mm256_load_##OPNAME((const PTR*)r);                                                            \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void aligned_store(e_t* r) const {                                                           \
    _mm256_store_##OPNAME((PTR*)r, mm);                                                                  \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void unaligned_load(const e_t* r) {                                                          \
    mm = _mm256_loadu_##OPNAME((const PTR*)r);                                                           \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void unaligned_store(e_t* r) {                                                               \
    _mm256_storeu_##OPNAME((PTR*)r, mm);                                                                 \
}                                                                                                        \
                                                                                                         \
void interleaved_load(const e_t* r, std::size_t step) {                                                  \
    alignas(CLASS)e_t temp[W];                                                                           \
    for (std::size_t i = 0; i < W; ++i, r += step) {                                                     \
        temp[i] = *r;                                                                                    \
    }                                                                                                    \
    aligned_load(temp);                                                                                  \
}                                                                                                        \
                                                                                                         \
void interleaved_store(e_t* r, std::size_t step) {                                                       \
    alignas(CLASS)e_t temp[W];                                                                           \
    aligned_store(temp);                                                                                 \
    for (std::size_t i = 0; i < W; ++i, r += step) {                                                     \
        *r = temp[i];                                                                                    \
    }                                                                                                    \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    struct avxf : simd_base<avxf> {
        SIMDIFY_AVX_COMMON_DECLARATIONS(avxf, ps, float);

        e_t front() const { return  _mm_cvtss_f32(_mm256_castps256_ps128(mm)); }
    };

    struct avxu : simd_base<avxu> {
        SIMDIFY_AVX_COMMON_DECLARATIONS(avxu, ps, float);

        SIMDIFY_INL avxu(const expr::bit_not<avxu>& r) { *this = r; }

        SIMDIFY_INL avxu& operator=(const expr::bit_not<avxu>& r) {
            mm = _mm256_xor_ps(r.neg.mm, avxu(all_bits()).mm);
            return *this;
        }

        e_t front() const { return _mm_cvt_ss2si(_mm256_castps256_ps128(mm)); }
    };

    struct avxs : simd_base<avxs> {
        SIMDIFY_AVX_COMMON_DECLARATIONS(avxs, ps, float);

        e_t front() const { return _mm_cvt_ss2si(_mm256_castps256_ps128(mm)); }
    };

    SIMDIFY_INL const avxu operator&(const avxu& l, const avxu& r) { return _mm256_and_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxu operator|(const avxu& l, const avxu& r) { return _mm256_or_ps(l.mm, r.mm); }
    SIMDIFY_INL const avxu operator^(const avxu& l, const avxu& r) { return _mm256_xor_ps(l.mm, r.mm); }
    SIMDIFY_INL const expr::bit_not<avxu> operator~(const avxu& l) { return expr::bit_not<avxu>(l); }
    SIMDIFY_INL const avxu nand(const avxu& l, const avxu& r) { return _mm256_andnot_ps(l.mm, r.mm); }

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
        static SIMDIFY_INL bit_field find(const avxu& in) { return bit_field(bit_t(_mm256_movemask_ps(in.mm))); }
        static SIMDIFY_INL bool any(const avxu& in) { return _mm256_movemask_ps(in.mm) != 0; }
        static SIMDIFY_INL bool all(const avxu& in) { return _mm256_movemask_ps(in.mm) == 0xFF; }

        template <binary_op_t F>
        static SIMDIFY_INL const Crtp reduce_vector(const Crtp& in) {
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
