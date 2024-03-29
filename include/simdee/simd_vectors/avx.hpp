// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_AVX_HPP
#define SIMDEE_SIMD_TYPES_AVX_HPP

#include "common.hpp"

#if !SIMDEE_AVX
#error "AVX intrinsics are required to use the AVX SIMD type. Please check your build options."
#endif
#if SIMDEE_NEED_INT && !SIMDEE_AVX2
#error "AVX2 intrinsics are required to use the AVX SIMD type with integer operations."
#endif

#include <immintrin.h>

namespace sd {
    struct avxb;
    struct avxf;
    struct avxu;
    struct avxs;
    using not_avxb = expr::deferred_lognot<avxb>;
    using not_avxu = expr::deferred_bitnot<avxu>;
    using not_avxs = expr::deferred_bitnot<avxs>;

    template <>
    struct is_simd_vector<avxb> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<avxf> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<avxu> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<avxs> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Scalar_t>
    struct avx_traits {
        using simd_t = Simd_t;
        using vector_t = __m256;
        using scalar_t = Scalar_t;
        using vec_b = avxb;
        using vec_f = avxf;
        using vec_u = avxu;
        using vec_s = avxs;
        using mask_t = impl::mask<0xffU>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <>
    struct simd_vector_traits<avxb> : avx_traits<avxb, bool32_t> {};
    template <>
    struct simd_vector_traits<avxf> : avx_traits<avxf, float> {};
    template <>
    struct simd_vector_traits<avxu> : avx_traits<avxu, uint32_t> {};
    template <>
    struct simd_vector_traits<avxs> : avx_traits<avxs, int32_t> {};

    template <typename Crtp>
    struct avx_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;
        SIMDEE_INL __m256i mmi() const { return _mm256_castps_si256(mm); }

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(avx_base)

        SIMDEE_BASE_CTOR(avx_base, vector_t, mm = r)
        SIMDEE_BASE_CTOR(avx_base, scalar_t,
                         mm = _mm256_broadcast_ss(reinterpret_cast<const float*>(&r)))
        SIMDEE_BASE_CTOR_FLAG(avx_base, expr::zero, mm = _mm256_setzero_ps())
        SIMDEE_BASE_CTOR_TPL(avx_base, expr::aligned<T>, aligned_load(r.ptr))
        SIMDEE_BASE_CTOR_TPL(avx_base, expr::unaligned<T>, unaligned_load(r.ptr))
        SIMDEE_BASE_CTOR_TPL(avx_base, expr::init<T>, *this = r.template to<scalar_t>())
        SIMDEE_BASE_CTOR(avx_base, storage_t, aligned_load(r.data()))

        SIMDEE_INL void aligned_load(const scalar_t* r) {
            mm = _mm256_load_ps(reinterpret_cast<const float*>(r));
        }
        SIMDEE_INL void aligned_store(scalar_t* r) const {
            _mm256_store_ps(reinterpret_cast<float*>(r), mm);
        }
        SIMDEE_INL void unaligned_load(const scalar_t* r) {
            mm = _mm256_loadu_ps(reinterpret_cast<const float*>(r));
        }
        SIMDEE_INL void unaligned_store(scalar_t* r) const {
            _mm256_storeu_ps(reinterpret_cast<float*>(r), mm);
        }

        template <unsigned int Lane>
        const Crtp broadcast() {
            static_assert(Lane < 8, "");
            constexpr int lane0 = Lane & 3;
            constexpr int lane1 = Lane >> 2;
            auto tmp = _mm256_shuffle_ps(mm, mm, _MM_SHUFFLE(lane0, lane0, lane0, lane0));
            return _mm256_permute2f128_ps(tmp, tmp, _MM_SHUFFLE(lane1, lane1, lane1, lane1));
        }

        template <typename Op_t>
        friend const Crtp reduce(const Crtp& l, Op_t f) {
            Crtp tmp = f(l, _mm256_permute_ps(l.mm, _MM_SHUFFLE(2, 3, 0, 1)));
            tmp = f(tmp, _mm256_permute_ps(tmp.mm, _MM_SHUFFLE(1, 0, 3, 2)));
            return f(tmp, _mm256_permute2f128_ps(tmp.mm, tmp.mm, _MM_SHUFFLE(0, 0, 0, 1)));
        }

#if SIMDEE_AVX2
        SIMDEE_INL avx_base(const expr::all_bits& r) { operator=(r); }
        SIMDEE_INL avx_base& operator=(const expr::all_bits&) {
            mm = _mm256_castsi256_ps(
                _mm256_cmpeq_epi32(_mm256_castps_si256(mm), _mm256_castps_si256(mm)));
            return self();
        }
#else
        SIMDEE_INL avx_base(const expr::all_bits& r) { operator=(r); }
        SIMDEE_INL avx_base& operator=(const expr::all_bits&) {
            mm = _mm256_castsi256_ps(_mm256_set1_epi32(-1));
            return self();
        }
#endif
    };

    struct avxb : avx_base<avxb> {
        SIMDEE_TRIVIAL_TYPE(avxb)
        SIMDEE_INL avxb(bool32_t v0, bool32_t v1, bool32_t v2, bool32_t v3, bool32_t v4,
                        bool32_t v5, bool32_t v6, bool32_t v7) {
            mm = _mm256_castsi256_ps(_mm256_setr_epi32(int32_t(v0), int32_t(v1), int32_t(v2),
                                                       int32_t(v3), int32_t(v4), int32_t(v5),
                                                       int32_t(v6), int32_t(v7)));
        }

        using avx_base::avx_base;
        SIMDEE_CTOR(avxb, __m256i, mm = _mm256_castsi256_ps(r))

#if SIMDEE_AVX2
        SIMDEE_CTOR(avxb, not_avxb,
                    mm = _mm256_xor_ps(r.neg.mm, _mm256_castsi256_ps(_mm256_cmpeq_epi32(
                                                     _mm256_castps_si256(r.neg.mm),
                                                     _mm256_castps_si256(r.neg.mm)))))
#else
        SIMDEE_CTOR(avxb, not_avxb, mm = _mm256_cmp_ps(r.neg.mm, r.neg.mm, _CMP_ORD_Q))
#endif

        SIMDEE_UNOP(avxb, mask_t, mask, mask_t(cast_u(_mm256_movemask_ps(l.mm))))
        SIMDEE_UNOP(avxb, scalar_t, first_scalar,
                    dirty::as_b(_mm_cvtss_f32(_mm256_castps256_ps128(l.mm))))

#if SIMDEE_AVX2
        SIMDEE_BINOP(avxb, avxb, operator==, _mm256_cmpeq_epi32(l.mmi(), r.mmi()))
#else
        SIMDEE_BINOP(avxb, not_avxb, operator==, not_avxb(_mm256_xor_ps(l.mm, r.mm)))
#endif
        SIMDEE_BINOP(avxb, avxb, operator!=, _mm256_xor_ps(l.mm, r.mm))

        SIMDEE_BINOP(avxb, avxb, operator&&, _mm256_and_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxb, avxb, operator||, _mm256_or_ps(l.mm, r.mm))
        SIMDEE_UNOP(avxb, not_avxb, operator!, not_avxb(l))
    };

    struct avxf : avx_base<avxf> {
        SIMDEE_TRIVIAL_TYPE(avxf)

        using avx_base::avx_base;
        SIMDEE_INL explicit avxf(const avxs&);
        SIMDEE_INL avxf(float v0, float v1, float v2, float v3, float v4, float v5, float v6,
                        float v7) {
            mm = _mm256_setr_ps(v0, v1, v2, v3, v4, v5, v6, v7);
        }

        SIMDEE_UNOP(avxf, scalar_t, first_scalar, _mm_cvtss_f32(_mm256_castps256_ps128(l.mm)))

        SIMDEE_BINOP(avxf, avxb, operator<, _mm256_cmp_ps(l.mm, r.mm, _CMP_LT_OQ))
        SIMDEE_BINOP(avxf, avxb, operator>, _mm256_cmp_ps(l.mm, r.mm, _CMP_GT_OQ))
        SIMDEE_BINOP(avxf, avxb, operator<=, _mm256_cmp_ps(l.mm, r.mm, _CMP_LE_OQ))
        SIMDEE_BINOP(avxf, avxb, operator>=, _mm256_cmp_ps(l.mm, r.mm, _CMP_GE_OQ))
        SIMDEE_BINOP(avxf, avxb, operator==, _mm256_cmp_ps(l.mm, r.mm, _CMP_EQ_OQ))
        SIMDEE_BINOP(avxf, avxb, operator!=, _mm256_cmp_ps(l.mm, r.mm, _CMP_NEQ_OQ))

        SIMDEE_UNOP(avxf, avxf, operator-, _mm256_xor_ps(l.mm, avxf(sign_bit()).mm))
        SIMDEE_BINOP(avxf, avxf, operator+, _mm256_add_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxf, avxf, operator-, _mm256_sub_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxf, avxf, operator*, _mm256_mul_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxf, avxf, operator/, _mm256_div_ps(l.mm, r.mm))

        SIMDEE_BINOP(avxf, avxf, min, _mm256_min_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxf, avxf, max, _mm256_max_ps(l.mm, r.mm))
        SIMDEE_UNOP(avxf, avxf, sqrt, _mm256_sqrt_ps(l.mm))
        SIMDEE_UNOP(avxf, avxf, rsqrt, _mm256_rsqrt_ps(l.mm))
        SIMDEE_UNOP(avxf, avxf, rcp, _mm256_rcp_ps(l.mm))
        SIMDEE_UNOP(avxf, avxf, abs, _mm256_and_ps(l.mm, avxf(abs_mask()).mm))
    };

    struct avxu : avx_base<avxu> {
        SIMDEE_TRIVIAL_TYPE(avxu)

        using avx_base::avx_base;
        SIMDEE_INL explicit avxu(const avxb&);
        SIMDEE_INL explicit avxu(const avxs&);
        SIMDEE_INL avxu(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4,
                        uint32_t v5, uint32_t v6, uint32_t v7) {
            mm = _mm256_castsi256_ps(_mm256_setr_epi32(int32_t(v0), int32_t(v1), int32_t(v2),
                                                       int32_t(v3), int32_t(v4), int32_t(v5),
                                                       int32_t(v6), int32_t(v7)));
        }
        SIMDEE_CTOR(avxu, __m256i, mm = _mm256_castsi256_ps(r))
        SIMDEE_CTOR(avxu, not_avxu, mm = _mm256_xor_ps(r.neg.mm, avxu(all_bits()).mm))

        SIMDEE_UNOP(avxu, scalar_t, first_scalar,
                    dirty::as_u(_mm_cvtss_f32(_mm256_castps256_ps128(l.mm))))

#if SIMDEE_NEED_INT
        SIMDEE_BINOP(avxu, not_avxb, operator<, max(l, r) != l)
        SIMDEE_BINOP(avxu, not_avxb, operator>, max(l, r) != r)
        SIMDEE_BINOP(avxu, avxb, operator<=, max(l, r) == r)
        SIMDEE_BINOP(avxu, avxb, operator>=, max(l, r) == l)
        SIMDEE_BINOP(avxu, avxb, operator==, _mm256_cmpeq_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxu, not_avxb, operator!=, not_avxb(_mm256_cmpeq_epi32(l.mmi(), r.mmi())))
        SIMDEE_BINOP(avxu, avxu, operator&, _mm256_and_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxu, avxu, operator|, _mm256_or_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxu, avxu, operator^, _mm256_xor_ps(l.mm, r.mm))
        SIMDEE_UNOP(avxu, not_avxu, operator~, not_avxu(l))
        SIMDEE_UNOP(avxu, avxu, operator-, _mm256_sub_epi32(_mm256_setzero_si256(), l.mmi()))
        SIMDEE_BINOP(avxu, avxu, operator+, _mm256_add_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxu, avxu, operator-, _mm256_sub_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxu, avxu, operator*, _mm256_mullo_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxu, avxu, min, _mm256_min_epu32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxu, avxu, max, _mm256_max_epu32(l.mmi(), r.mmi()))
#endif
    };

    struct avxs : avx_base<avxs> {
        SIMDEE_TRIVIAL_TYPE(avxs)

        using avx_base::avx_base;
        SIMDEE_INL explicit avxs(const avxf&);
        SIMDEE_INL explicit avxs(const avxu&);
        SIMDEE_INL avxs(int32_t v0, int32_t v1, int32_t v2, int32_t v3, int32_t v4, int32_t v5,
                        int32_t v6, int32_t v7) {
            mm = _mm256_castsi256_ps(_mm256_setr_epi32(v0, v1, v2, v3, v4, v5, v6, v7));
        }
        SIMDEE_CTOR(avxs, __m256i, mm = _mm256_castsi256_ps(r))
        SIMDEE_CTOR(avxs, not_avxs, mm = _mm256_xor_ps(r.neg.mm, avxs(all_bits()).mm))

        SIMDEE_UNOP(avxs, scalar_t, first_scalar,
                    dirty::as_s(_mm_cvtss_f32(_mm256_castps256_ps128(l.mm))))

#if SIMDEE_NEED_INT
        SIMDEE_BINOP(avxs, avxb, operator<, _mm256_cmpgt_epi32(r.mmi(), l.mmi()))
        SIMDEE_BINOP(avxs, avxb, operator>, _mm256_cmpgt_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxs, not_avxb, operator<=, not_avxb(_mm256_cmpgt_epi32(l.mmi(), r.mmi())))
        SIMDEE_BINOP(avxs, not_avxb, operator>=, not_avxb(_mm256_cmpgt_epi32(r.mmi(), l.mmi())))
        SIMDEE_BINOP(avxs, avxb, operator==, _mm256_cmpeq_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxs, not_avxb, operator!=, not_avxb(_mm256_cmpeq_epi32(l.mmi(), r.mmi())))

        SIMDEE_BINOP(avxs, avxs, operator&, _mm256_and_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxs, avxs, operator|, _mm256_or_ps(l.mm, r.mm))
        SIMDEE_BINOP(avxs, avxs, operator^, _mm256_xor_ps(l.mm, r.mm))
        SIMDEE_UNOP(avxs, not_avxs, operator~, not_avxs(l))

        SIMDEE_UNOP(avxs, avxs, operator-, _mm256_sub_epi32(_mm256_setzero_si256(), l.mmi()))
        SIMDEE_BINOP(avxs, avxs, operator+, _mm256_add_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxs, avxs, operator-, _mm256_sub_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxs, avxs, operator*, _mm256_mullo_epi32(l.mmi(), r.mmi()))

        SIMDEE_BINOP(avxs, avxs, min, _mm256_min_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(avxs, avxs, max, _mm256_max_epi32(l.mmi(), r.mmi()))
        SIMDEE_UNOP(avxs, avxs, abs, _mm256_abs_epi32(l.mmi()))
#endif
    };

    SIMDEE_INL avxf::avxf(const avxs& r) { mm = _mm256_cvtepi32_ps(_mm256_castps_si256(r.data())); }
    SIMDEE_INL avxs::avxs(const avxf& r) {
        mm = _mm256_castsi256_ps(_mm256_cvttps_epi32(r.data()));
    }
    SIMDEE_INL avxu::avxu(const avxb& r) { mm = r.data(); }
    SIMDEE_INL avxu::avxu(const avxs& r) { mm = r.data(); }
    SIMDEE_INL avxs::avxs(const avxu& r) { mm = r.data(); }

    SIMDEE_INL const avxb cond(const avxb& pred, const avxb& if_true, const avxb& if_false) {
        return _mm256_blendv_ps(if_false.data(), if_true.data(), pred.data());
    }
    SIMDEE_INL const avxf cond(const avxb& pred, const avxf& if_true, const avxf& if_false) {
        return _mm256_blendv_ps(if_false.data(), if_true.data(), pred.data());
    }
    SIMDEE_INL const avxu cond(const avxb& pred, const avxu& if_true, const avxu& if_false) {
        return _mm256_blendv_ps(if_false.data(), if_true.data(), pred.data());
    }
    SIMDEE_INL const avxs cond(const avxb& pred, const avxs& if_true, const avxs& if_false) {
        return _mm256_blendv_ps(if_false.data(), if_true.data(), pred.data());
    }

    namespace impl {

        template <typename T, typename NotT>
        struct avx_special_ops {
            SIMDEE_INL static T andnot(const T& l, const T& r) {
                return _mm256_andnot_ps(r.data(), l.data());
            }
            SIMDEE_INL static NotT ornot(const T& l, const T& r) { return NotT(andnot(r, l)); }
        };

        template <typename T>
        struct special_ops;

        template <>
        struct special_ops<avxb> : avx_special_ops<avxb, not_avxb> {};

        template <>
        struct special_ops<avxu> : avx_special_ops<avxu, not_avxu> {};

        template <>
        struct special_ops<avxs> : avx_special_ops<avxs, not_avxs> {};

    } // namespace impl

} // namespace sd

#endif // SIMDEE_SIMD_TYPES_AVX_HPP
