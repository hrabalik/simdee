// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_SSE_HPP
#define SIMDEE_SIMD_TYPES_SSE_HPP

#include "common.hpp"

#if !SIMDEE_SSE2
#error "SSE2 intrinsics are required to use the SSE SIMD type. Please check your build options."
#endif

#include <emmintrin.h>

#if SIMDEE_SSSE3
#include <tmmintrin.h>
#endif

#if SIMDEE_SSE41
#include <smmintrin.h>
#endif

namespace sd {
    namespace impl {
#if SIMDEE_SSE41
        SIMDEE_INL __m128 sse_cond(__m128 pred, __m128 if_true, __m128 if_false) {
            return _mm_blendv_ps(if_false, if_true, pred);
        }
        SIMDEE_INL __m128i sse_imul(__m128i l, __m128i r) { return _mm_mullo_epi32(l, r); }
#else
        SIMDEE_INL __m128 sse_cond(__m128 pred, __m128 if_true, __m128 if_false) {
            return _mm_or_ps(_mm_and_ps(pred, if_true), _mm_andnot_ps(pred, if_false));
        }
        SIMDEE_INL __m128i sse_imul(__m128i l, __m128i r) {
            __m128i prod_a = _mm_mul_epu32(l, r);
            __m128i prod_b = _mm_mul_epu32(_mm_srli_epi64(l, 32), _mm_srli_epi64(r, 32));
            prod_a = _mm_and_si128(prod_a, _mm_setr_epi32(-1, 0, -1, 0));
            prod_b = _mm_slli_epi64(prod_b, 32);
            return _mm_or_si128(prod_a, prod_b);
        }
#endif
    } // namespace impl

    struct sseb;
    struct ssef;
    struct sseu;
    struct sses;
    using not_sseb = expr::deferred_lognot<sseb>;
    using not_sseu = expr::deferred_bitnot<sseu>;
    using not_sses = expr::deferred_bitnot<sses>;

    template <>
    struct is_simd_vector<sseb> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<ssef> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<sseu> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<sses> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Scalar_t>
    struct sse_traits {
        using simd_t = Simd_t;
        using vector_t = __m128;
        using scalar_t = Scalar_t;
        using vec_b = sseb;
        using vec_f = ssef;
        using vec_u = sseu;
        using vec_s = sses;
        using mask_t = impl::mask<0xfU>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <>
    struct simd_vector_traits<sseb> : sse_traits<sseb, bool32_t> {};
    template <>
    struct simd_vector_traits<ssef> : sse_traits<ssef, float> {};
    template <>
    struct simd_vector_traits<sseu> : sse_traits<sseu, uint32_t> {};
    template <>
    struct simd_vector_traits<sses> : sse_traits<sses, int32_t> {};

    template <typename Crtp>
    struct sse_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;
        SIMDEE_INL __m128i mmi() const { return _mm_castps_si128(mm); }

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(sse_base)

        SIMDEE_BASE_CTOR(sse_base, vector_t, mm = r)
        SIMDEE_BASE_CTOR(sse_base, scalar_t, mm = _mm_set_ps1(reinterpret_cast<const float&>(r)))
        SIMDEE_BASE_CTOR_FLAG(sse_base, expr::zero, mm = _mm_setzero_ps())
        SIMDEE_BASE_CTOR_TPL(sse_base, expr::aligned<T>, aligned_load(r.ptr))
        SIMDEE_BASE_CTOR_TPL(sse_base, expr::unaligned<T>, unaligned_load(r.ptr))
        SIMDEE_BASE_CTOR_TPL(sse_base, expr::interleaved<T>, interleaved_load(r.ptr, r.step))
        SIMDEE_BASE_CTOR_TPL(sse_base, expr::init<T>, *this = r.template to<scalar_t>())
        SIMDEE_BASE_CTOR(sse_base, storage_t, aligned_load(r.data()))

        SIMDEE_INL sse_base(const expr::all_bits& r) { operator=(r); }
        SIMDEE_INL sse_base& operator=(const expr::all_bits&) {
            mm = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_castps_si128(mm), _mm_castps_si128(mm)));
            return self();
        }

        SIMDEE_INL void aligned_load(const scalar_t* r) {
            mm = _mm_load_ps(reinterpret_cast<const float*>(r));
        }
        SIMDEE_INL void aligned_store(scalar_t* r) const {
            _mm_store_ps(reinterpret_cast<float*>(r), mm);
        }
        SIMDEE_INL void unaligned_load(const scalar_t* r) {
            mm = _mm_loadu_ps(reinterpret_cast<const float*>(r));
        }
        SIMDEE_INL void unaligned_store(scalar_t* r) const {
            _mm_storeu_ps(reinterpret_cast<float*>(r), mm);
        }

        void interleaved_load(const scalar_t* r, int step) {
            alignas(Crtp) scalar_t temp[width];
            for (std::size_t i = 0; i < width; ++i, r += step) { temp[i] = *r; }
            aligned_load(temp);
        }

        void interleaved_store(scalar_t* r, int step) const {
            alignas(Crtp) scalar_t temp[width];
            aligned_store(temp);
            for (std::size_t i = 0; i < width; ++i, r += step) { *r = temp[i]; }
        }

        template <typename Op_t>
        friend const Crtp reduce(const Crtp& l, Op_t f) {
            Crtp tmp = f(l, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(l.mm),
                                                               _MM_SHUFFLE(2, 3, 0, 1))));
            return f(tmp, _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp.mm),
                                                             _MM_SHUFFLE(1, 0, 3, 2))));
        }
    };

    struct sseb : sse_base<sseb> {
        SIMDEE_TRIVIAL_TYPE(sseb)

        using sse_base::sse_base;
        SIMDEE_INL sseb(bool32_t v0, bool32_t v1, bool32_t v2, bool32_t v3) {
            mm = _mm_castsi128_ps(
                _mm_setr_epi32(int32_t(v0), int32_t(v1), int32_t(v2), int32_t(v3)));
        }
        SIMDEE_CTOR(sseb, __m128i, mm = _mm_castsi128_ps(r))
        SIMDEE_CTOR(sseb, not_sseb,
                    mm = _mm_xor_ps(r.neg.mm,
                                    _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_castps_si128(r.neg.mm),
                                                                     _mm_castps_si128(r.neg.mm)))))

        SIMDEE_UNOP(sseb, mask_t, mask, mask_t(cast_u(_mm_movemask_ps(l.mm))))
        SIMDEE_UNOP(sseb, scalar_t, first_scalar, dirty::as_b(_mm_cvtss_f32(l.mm)))

        SIMDEE_BINOP(sseb, sseb, operator==, _mm_cmpeq_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sseb, sseb, operator!=, _mm_xor_ps(l.mm, r.mm))
        SIMDEE_BINOP(sseb, sseb, operator&&, _mm_and_ps(l.mm, r.mm))
        SIMDEE_BINOP(sseb, sseb, operator||, _mm_or_ps(l.mm, r.mm))
        SIMDEE_UNOP(sseb, not_sseb, operator!, not_sseb(l))
    };

    struct ssef : sse_base<ssef> {
        SIMDEE_TRIVIAL_TYPE(ssef)

        using sse_base::sse_base;
        SIMDEE_INL explicit ssef(const sses&);
        SIMDEE_INL ssef(float v0, float v1, float v2, float v3) {
            mm = _mm_setr_ps(v0, v1, v2, v3);
        }

        SIMDEE_UNOP(ssef, scalar_t, first_scalar, _mm_cvtss_f32(l.mm))

        SIMDEE_BINOP(ssef, sseb, operator<, _mm_cmplt_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, sseb, operator>, _mm_cmpgt_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, sseb, operator<=, _mm_cmple_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, sseb, operator>=, _mm_cmpge_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, sseb, operator==, _mm_cmpeq_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, sseb, operator!=, _mm_cmpneq_ps(l.mm, r.mm))

        SIMDEE_UNOP(ssef, ssef, operator-, _mm_xor_ps(l.mm, ssef(sign_bit()).mm))
        SIMDEE_BINOP(ssef, ssef, operator+, _mm_add_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, ssef, operator-, _mm_sub_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, ssef, operator*, _mm_mul_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, ssef, operator/, _mm_div_ps(l.mm, r.mm))

        SIMDEE_BINOP(ssef, ssef, min, _mm_min_ps(l.mm, r.mm))
        SIMDEE_BINOP(ssef, ssef, max, _mm_max_ps(l.mm, r.mm))
        SIMDEE_UNOP(ssef, ssef, sqrt, _mm_sqrt_ps(l.mm))
        SIMDEE_UNOP(ssef, ssef, rsqrt, _mm_rsqrt_ps(l.mm))
        SIMDEE_UNOP(ssef, ssef, rcp, _mm_rcp_ps(l.mm))
        SIMDEE_UNOP(ssef, ssef, abs, _mm_and_ps(l.mm, ssef(abs_mask()).mm))
    };

    struct sseu : sse_base<sseu> {
        SIMDEE_TRIVIAL_TYPE(sseu)

        using sse_base::sse_base;
        SIMDEE_INL explicit sseu(const sseb&);
        SIMDEE_INL explicit sseu(const sses&);
        SIMDEE_INL sseu(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
            mm = _mm_castsi128_ps(
                _mm_setr_epi32(int32_t(v0), int32_t(v1), int32_t(v2), int32_t(v3)));
        }
        SIMDEE_CTOR(sseu, __m128i, mm = _mm_castsi128_ps(r))
        SIMDEE_CTOR(sseu, not_sseu, mm = _mm_xor_ps(r.neg.mm, sseu(all_bits()).mm))

        SIMDEE_UNOP(sseu, scalar_t, first_scalar, dirty::as_u(_mm_cvtss_f32(l.mm)))

#if SIMDEE_NEED_INT

#if SIMDEE_SSE41
        SIMDEE_BINOP(sseu, not_sseb, operator<, max(l, r) != l)
        SIMDEE_BINOP(sseu, not_sseb, operator>, max(l, r) != r)
        SIMDEE_BINOP(sseu, sseb, operator<=, max(l, r) == r)
        SIMDEE_BINOP(sseu, sseb, operator>=, max(l, r) == l)
        SIMDEE_BINOP(sseu, sseu, min, _mm_min_epu32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sseu, sseu, max, _mm_max_epu32(l.mmi(), r.mmi()))
#else
        SIMDEE_BINOP(sseu, sseb, operator<, r> l)
        SIMDEE_INL friend const sseb operator>(const sseu& l, const sseu& r) {
            sseu low{0x80000000};
            return _mm_cmpgt_epi32(_mm_sub_epi32(l.mmi(), low.mmi()),
                                   _mm_sub_epi32(r.mmi(), low.mmi()));
        }
        SIMDEE_BINOP(sseu, not_sseb, operator<=, !(l > r))
        SIMDEE_BINOP(sseu, not_sseb, operator>=, !(r > l))
        SIMDEE_INL friend const sseu min(const sseu& l, const sseu& r) {
            return impl::sse_cond((r > l).data(), l.mm, r.mm);
        }

        SIMDEE_INL friend const sseu max(const sseu& l, const sseu& r) {
            return impl::sse_cond((l > r).data(), l.mm, r.mm);
        }
#endif

        SIMDEE_BINOP(sseu, sseb, operator==, _mm_cmpeq_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sseu, not_sseb, operator!=, not_sseb(_mm_cmpeq_epi32(l.mmi(), r.mmi())))
        SIMDEE_BINOP(sseu, sseu, operator&, _mm_and_ps(l.mm, r.mm))
        SIMDEE_BINOP(sseu, sseu, operator|, _mm_or_ps(l.mm, r.mm))
        SIMDEE_BINOP(sseu, sseu, operator^, _mm_xor_ps(l.mm, r.mm))
        SIMDEE_UNOP(sseu, not_sseu, operator~, not_sseu(l))
        SIMDEE_UNOP(sseu, sseu, operator-, _mm_sub_epi32(_mm_setzero_si128(), l.mmi()))
        SIMDEE_BINOP(sseu, sseu, operator+, _mm_add_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sseu, sseu, operator-, _mm_sub_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sseu, sseu, operator*, impl::sse_imul(l.mmi(), r.mmi()))
#endif
    };

    struct sses : sse_base<sses> {
        SIMDEE_TRIVIAL_TYPE(sses)

        using sse_base::sse_base;
        SIMDEE_INL explicit sses(const ssef&);
        SIMDEE_INL explicit sses(const sseu&);
        SIMDEE_INL sses(int32_t v0, int32_t v1, int32_t v2, int32_t v3) {
            mm = _mm_castsi128_ps(_mm_setr_epi32(v0, v1, v2, v3));
        }
        SIMDEE_CTOR(sses, __m128i, mm = _mm_castsi128_ps(r))
        SIMDEE_CTOR(sses, not_sses, mm = _mm_xor_ps(r.neg.mm, sses(all_bits()).mm))

        SIMDEE_UNOP(sses, scalar_t, first_scalar, dirty::as_s(_mm_cvtss_f32(l.mm)))

#if SIMDEE_NEED_INT
        SIMDEE_BINOP(sses, sseb, operator<, _mm_cmplt_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sses, sseb, operator>, _mm_cmpgt_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sses, not_sseb, operator<=, not_sseb(_mm_cmpgt_epi32(l.mmi(), r.mmi())))
        SIMDEE_BINOP(sses, not_sseb, operator>=, not_sseb(_mm_cmplt_epi32(l.mmi(), r.mmi())))
        SIMDEE_BINOP(sses, sseb, operator==, _mm_cmpeq_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sses, not_sseb, operator!=, not_sseb(_mm_cmpeq_epi32(l.mmi(), r.mmi())))

        SIMDEE_BINOP(sses, sses, operator&, _mm_and_ps(l.mm, r.mm))
        SIMDEE_BINOP(sses, sses, operator|, _mm_or_ps(l.mm, r.mm))
        SIMDEE_BINOP(sses, sses, operator^, _mm_xor_ps(l.mm, r.mm))
        SIMDEE_UNOP(sses, not_sses, operator~, not_sses(l))

        SIMDEE_UNOP(sses, sses, operator-, _mm_sub_epi32(_mm_setzero_si128(), l.mmi()))
        SIMDEE_BINOP(sses, sses, operator+, _mm_add_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sses, sses, operator-, _mm_sub_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sses, sses, operator*, impl::sse_imul(l.mmi(), r.mmi()))

#if SIMDEE_SSE41
        SIMDEE_BINOP(sses, sses, min, _mm_min_epi32(l.mmi(), r.mmi()))
        SIMDEE_BINOP(sses, sses, max, _mm_max_epi32(l.mmi(), r.mmi()))
#else
        SIMDEE_INL friend sses min(const sses& l, const sses& r) {
            return impl::sse_cond((r > l).data(), l.mm, r.mm);
        }
        SIMDEE_INL friend sses max(const sses& l, const sses& r) {
            return impl::sse_cond((l > r).data(), l.mm, r.mm);
        }
#endif

#if SIMDEE_SSSE3
        SIMDEE_UNOP(sses, sses, abs, _mm_abs_epi32(l.mmi()))
#else
        SIMDEE_UNOP(sses, sses, abs, max(l, -l))
#endif

#endif
    };

    SIMDEE_INL ssef::ssef(const sses& r) { mm = _mm_cvtepi32_ps(_mm_castps_si128(r.data())); }
    SIMDEE_INL sses::sses(const ssef& r) { mm = _mm_castsi128_ps(_mm_cvttps_epi32(r.data())); }
    SIMDEE_INL sseu::sseu(const sseb& r) { mm = r.data(); }
    SIMDEE_INL sseu::sseu(const sses& r) { mm = r.data(); }
    SIMDEE_INL sses::sses(const sseu& r) { mm = r.data(); }

    SIMDEE_INL const sseb cond(const sseb& pred, const sseb& if_true, const sseb& if_false) {
        return impl::sse_cond(pred.data(), if_true.data(), if_false.data());
    }

    SIMDEE_INL const ssef cond(const sseb& pred, const ssef& if_true, const ssef& if_false) {
        return impl::sse_cond(pred.data(), if_true.data(), if_false.data());
    }

    SIMDEE_INL const sseu cond(const sseb& pred, const sseu& if_true, const sseu& if_false) {
        return impl::sse_cond(pred.data(), if_true.data(), if_false.data());
    }

    SIMDEE_INL const sses cond(const sseb& pred, const sses& if_true, const sses& if_false) {
        return impl::sse_cond(pred.data(), if_true.data(), if_false.data());
    }

    namespace impl {
        template <typename T>
        struct sse_special_traits {
            SIMDEE_INL static T andnot(const T& l, const T& r) {
                return _mm_andnot_ps(r.data(), l.data());
            }
        };

        template <typename T>
        struct special_traits;

        template <>
        struct special_traits<sseb> : sse_special_traits<sseb> {};

        template <>
        struct special_traits<sseu> : sse_special_traits<sseu> {};

        template <>
        struct special_traits<sses> : sse_special_traits<sses> {};
    } // namespace impl

} // namespace sd

#endif // SIMDEE_SIMD_TYPES_SSE_HPP
