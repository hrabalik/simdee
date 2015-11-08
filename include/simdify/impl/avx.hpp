#ifndef SIMDIFY_AVX
#define SIMDIFY_AVX

// ensure AVX is on
#ifndef __AVX__
#error "The macro '__AVX__' is not set. AVX intrinsics are required to use the AVX SIMD type. Please check your build options."
#endif

#include "common.hpp"
#include <immintrin.h>

namespace simd {
    
    // SIMD with AVX
    struct avx : simd_base<__m256, float, avx> {
        SIMDIFY_FORCE_INLINE ~avx() = default;
        SIMDIFY_FORCE_INLINE avx() = default;
        SIMDIFY_FORCE_INLINE avx(const avx&) = default;
        SIMDIFY_FORCE_INLINE avx(avx&&) = default;
        SIMDIFY_FORCE_INLINE avx& operator=(const avx&) = default;
        SIMDIFY_FORCE_INLINE avx& operator=(avx&&) = default;

        SIMDIFY_FORCE_INLINE avx(mm_t r) : simd_base(r) {}
        SIMDIFY_FORCE_INLINE avx(f_t r) : simd_base(_mm256_broadcast_ss(&r)) {}
        SIMDIFY_FORCE_INLINE avx(const expr::zero&) : simd_base(_mm256_setzero_ps()) {}
        SIMDIFY_FORCE_INLINE avx(const bitwise_not<avx>& r) : simd_base(_mm256_andnot_ps(r.neg.mm, avx(all_bits()).mm)) {}
        SIMDIFY_FORCE_INLINE void load(const f_t* r) { mm = _mm256_load_ps(r); }
        SIMDIFY_FORCE_INLINE void store(f_t* r) const { _mm256_store_ps(r, mm); }
        SIMDIFY_FORCE_INLINE f_t front() const { return _mm_cvtss_f32(_mm256_castps256_ps128(mm)); }
        SIMDIFY_FORCE_INLINE f_t back() const { return f.back(); }

        template <typename T>
        SIMDIFY_FORCE_INLINE avx(const expr::aligned<T>& r) : simd_base(_mm256_load_ps(r.template get_load<f_t>())) {}
        template <typename T>
        SIMDIFY_FORCE_INLINE avx(const expr::unaligned<T>& r) : simd_base(_mm256_loadu_ps(r.template get_load<f_t>())) {}
        template <typename T>
        SIMDIFY_FORCE_INLINE avx(const expr::tof<T>& r) : avx(r.template to<f_t>()) {}
    };

    SIMDIFY_FORCE_INLINE const avx operator&(const avx& l, const avx& r) { return _mm256_and_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator|(const avx& l, const avx& r) { return _mm256_or_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator^(const avx& l, const avx& r) { return _mm256_xor_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const bitwise_not<avx> operator~(const avx& l) { return bitwise_not<avx>(l); }
    SIMDIFY_FORCE_INLINE const avx operator<(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LT_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator>(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GT_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator<=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_LE_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator>=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_GE_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator==(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_EQ_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator!=(const avx& l, const avx& r) { return _mm256_cmp_ps(l.mm, r.mm, _CMP_NEQ_OQ); }
    SIMDIFY_FORCE_INLINE const avx operator+(const avx& l, const avx& r) { return _mm256_add_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator-(const avx& l, const avx& r) { return _mm256_sub_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator*(const avx& l, const avx& r) { return _mm256_mul_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator/(const avx& l, const avx& r) { return _mm256_div_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx operator-(const avx& in) { return in ^ sign_bit(); }
    SIMDIFY_FORCE_INLINE const avx andnot(const avx& l, const avx& r) { return _mm256_andnot_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx min(const avx& l, const avx& r) { return _mm256_min_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx max(const avx& l, const avx& r) { return _mm256_max_ps(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const avx cond(const avx& pred, const avx& if_true, const avx& if_false) { return _mm256_blendv_ps(if_false.mm, if_true.mm, pred.mm); }

    // horizontal operations
    template <>
    struct horizontal_impl<avx> : horizontal_impl_base<avx> {
        static SIMDIFY_FORCE_INLINE bit_field find(const avx& in) { return bit_field(bit_t(_mm256_movemask_ps(in.mm))); }
        static SIMDIFY_FORCE_INLINE bool any(const avx& in) { return _mm256_movemask_ps(in.mm) != 0; }
        static SIMDIFY_FORCE_INLINE bool all(const avx& in) { return _mm256_movemask_ps(in.mm) == 0xFF; }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE const avx reduce_vector(const avx& in) {
            avx tmp = F(in, _mm256_permute_ps(in.mm, _MM_SHUFFLE(2, 3, 0, 1)));
            tmp = F(tmp, _mm256_permute_ps(tmp.mm, _MM_SHUFFLE(1, 0, 3, 2)));
            return F(tmp, _mm256_permute2f128_ps(tmp.mm, tmp.mm, _MM_SHUFFLE(0, 0, 0, 1)));
        }
    };

}

#endif // SIMDIFY_AVX
