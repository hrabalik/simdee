// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_NEON_HPP
#define SIMDEE_SIMD_TYPES_NEON_HPP

#include "common.hpp"

#if !SIMDEE_NEON
#error "NEON intrinsics are required to use the NEON SIMD type. Please check your build options."
#endif

#include <arm_neon.h>
#include <cmath>

namespace sd {

    namespace impl {
        SIMDEE_INL uint32x4_t neon_load(const bool32_t* ptr) {
            return vld1q_u32(reinterpret_cast<const uint32_t*>(ptr));
        }
        SIMDEE_INL float32x4_t neon_load(const float* ptr) { return vld1q_f32(ptr); }
        SIMDEE_INL uint32x4_t neon_load(const uint32_t* ptr) { return vld1q_u32(ptr); }
        SIMDEE_INL int32x4_t neon_load(const int32_t* ptr) { return vld1q_s32(ptr); }
        SIMDEE_INL void neon_store(const uint32x4_t& vec, bool32_t* ptr) {
            vst1q_u32(reinterpret_cast<uint32_t*>(ptr), vec);
        }
        SIMDEE_INL void neon_store(const float32x4_t& vec, float* ptr) { vst1q_f32(ptr, vec); }
        SIMDEE_INL void neon_store(const uint32x4_t& vec, uint32_t* ptr) { vst1q_u32(ptr, vec); }
        SIMDEE_INL void neon_store(const int32x4_t& vec, int32_t* ptr) { vst1q_s32(ptr, vec); }
    } // namespace impl

    struct neonb;
    struct neonf;
    struct neonu;
    struct neons;
    using not_neonb = expr::deferred_lognot<neonb>;
    using not_neonu = expr::deferred_bitnot<neonu>;
    using not_neons = expr::deferred_bitnot<neons>;

    template <>
    struct is_simd_vector<neonb> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<neonf> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<neonu> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<neons> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Scalar_t>
    struct neon_traits {
        using simd_t = Simd_t;
        using scalar_t = Scalar_t;
        using vec_b = neonb;
        using vec_f = neonf;
        using vec_u = neonu;
        using vec_s = neons;
        using mask_t = impl::mask<0xfU>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(int32x4_t)>;
    };

    template <>
    struct simd_vector_traits<neonb> : neon_traits<neonb, bool32_t> {
        using vector_t = uint32x4_t;
    };

    template <>
    struct simd_vector_traits<neonf> : neon_traits<neonf, float> {
        using vector_t = float32x4_t;
    };

    template <>
    struct simd_vector_traits<neonu> : neon_traits<neonu, uint32_t> {
        using vector_t = uint32x4_t;
    };

    template <>
    struct simd_vector_traits<neons> : neon_traits<neons, int32_t> {
        using vector_t = int32x4_t;
    };

    template <typename Crtp>
    struct neon_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(neon_base)
        SIMDEE_BASE_CTOR(neon_base, vector_t, mm = r)
        SIMDEE_BASE_CTOR_TPL(neon_base, expr::aligned<T>, aligned_load(r.ptr))
        SIMDEE_BASE_CTOR_TPL(neon_base, expr::unaligned<T>, unaligned_load(r.ptr))
        SIMDEE_BASE_CTOR(neon_base, storage_t, aligned_load(r.data()))

        SIMDEE_INL void aligned_load(const scalar_t* r) { mm = impl::neon_load(r); }
        SIMDEE_INL void aligned_store(scalar_t* r) const { impl::neon_store(mm, r); }
        SIMDEE_INL void unaligned_load(const scalar_t* r) { mm = impl::neon_load(r); }
        SIMDEE_INL void unaligned_store(scalar_t* r) const { impl::neon_store(mm, r); }
    };

// clang-format off
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_NEON_COMMON( CLASS, SUFFIX, SCALAR_TYPE )                                                 \
using neon_base::neon_base;                                                                              \
SIMDEE_TRIVIAL_TYPE( CLASS )                                                                             \
SIMDEE_CTOR( CLASS , scalar_t, mm = vmovq_n_ ## SUFFIX ( SCALAR_TYPE (r)))                               \
SIMDEE_CTOR_TPL( CLASS, expr::init<T>, mm = vmovq_n_ ## SUFFIX (r.template to< SCALAR_TYPE >()))         \
SIMDEE_UNOP( CLASS, scalar_t, first_scalar, vgetq_lane_ ## SUFFIX (l.mm, 0))                             \
                                                                                                         \
SIMDEE_INL CLASS (scalar_t v0, scalar_t v1, scalar_t v2, scalar_t v3) {                                  \
    vector_t v = { v0, v1, v2, v3 };                                                                     \
    mm = v;                                                                                              \
}                                                                                                        \
                                                                                                         \
template <typename Op_t>                                                                                 \
friend const CLASS reduce(const CLASS & l, Op_t f) {                                                     \
    CLASS tmp = f(l, vextq_ ## SUFFIX (l.mm, l.mm, 2));                                                  \
    return f(tmp, vrev64q_ ## SUFFIX (tmp.mm));                                                          \
}                                                                                                        \
                                                                                                         \
template <unsigned int Lane>                                                                             \
const CLASS broadcast();                                                                                 \
                                                                                                         \
SIMDEE_NEON_OPTIMIZED_REDUCE(CLASS, SUFFIX)                                                              \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#if SIMDEE_ARM64
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_NEON_OPTIMIZED_REDUCE( CLASS, SUFFIX )                                                    \
SIMDEE_INL friend const CLASS reduce(const CLASS & l, op_add) {                                          \
    CLASS tmp {vpaddq_ ## SUFFIX (l.mm, l.mm)};                                                          \
    return vpaddq_ ## SUFFIX (tmp.mm, tmp.mm);                                                           \
}                                                                                                        \
                                                                                                         \
SIMDEE_INL friend const CLASS reduce(const CLASS & l, op_min) {                                          \
    CLASS tmp {vpminq_ ## SUFFIX (l.mm, l.mm)};                                                          \
    return vpminq_ ## SUFFIX (tmp.mm, tmp.mm);                                                           \
}                                                                                                        \
                                                                                                         \
SIMDEE_INL friend const CLASS reduce(const CLASS & l, op_max) {                                          \
    CLASS tmp {vpmaxq_ ## SUFFIX (l.mm, l.mm)};                                                          \
    return vpmaxq_ ## SUFFIX (tmp.mm, tmp.mm);                                                           \
}                                                                                                        \
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
#define SIMDEE_NEON_OPTIMIZED_REDUCE( CLASS, SUFFIX )
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_NEON_COMMON_FREESTANDING( CLASS, SUFFIX )                                                 \
                                                                                                         \
template <>                                                                                              \
SIMDEE_INL const CLASS CLASS::broadcast<0>() {                                                           \
    return vdupq_lane_ ## SUFFIX (vget_low_ ## SUFFIX (mm), 0);                                          \
}                                                                                                        \
                                                                                                         \
template <>                                                                                              \
SIMDEE_INL const CLASS CLASS::broadcast<1>() {                                                           \
    return vdupq_lane_ ## SUFFIX (vget_low_ ## SUFFIX (mm), 1);                                          \
}                                                                                                        \
                                                                                                         \
template <>                                                                                              \
SIMDEE_INL const CLASS CLASS::broadcast<2>() {                                                           \
    return vdupq_lane_ ## SUFFIX (vget_high_ ## SUFFIX (mm), 0);                                         \
}                                                                                                        \
                                                                                                         \
template <>                                                                                              \
SIMDEE_INL const CLASS CLASS::broadcast<3>() {                                                           \
    return vdupq_lane_ ## SUFFIX (vget_high_ ## SUFFIX (mm), 1);                                         \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // clang-format on

    struct neonb final : neon_base<neonb> {
        SIMDEE_NEON_COMMON(neonb, u32, uint32_t)
        SIMDEE_CTOR(neonb, not_neonb, mm = vmvnq_u32(r.neg.mm))

        SIMDEE_BINOP(neonb, neonb, operator==, vceqq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonb, neonb, operator!=, vmvnq_u32(vceqq_u32(l.mm, r.mm)))
        SIMDEE_BINOP(neonb, neonb, operator&&, vandq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonb, neonb, operator||, vorrq_u32(l.mm, r.mm))
        SIMDEE_UNOP(neonb, not_neonb, operator!, not_neonb(l))

#if SIMDEE_ARM64
        friend const mask_t mask(const neonb& l) {
            uint32x4_t temp = {0x1, 0x2, 0x4, 0x8};
            temp = vandq_u32(temp, l.mm);
            temp = vpaddq_u32(temp, temp);
            temp = vpaddq_u32(temp, temp);
            return mask_t(vgetq_lane_u32(temp, 0));
        }
#else
        friend const mask_t mask(const neonb& l) {
            uint32x4_t temp = {0x1, 0x2, 0x4, 0x8};
            temp = vandq_u32(temp, l.mm);
            uint32x2_t temp2 = vadd_u32(vget_low_u32(temp), vget_high_u32(temp));
            temp2 = vadd_u32(temp2, vrev64_u32(temp2));
            return mask_t(vget_lane_u32(temp2, 0));
        }
#endif
    };

    struct neonf final : neon_base<neonf> {
        SIMDEE_NEON_COMMON(neonf, f32, float)
        SIMDEE_INL explicit neonf(const neons&);

        SIMDEE_BINOP(neonf, neonb, operator<, vcltq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonb, operator>, vcgtq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonb, operator<=, vcleq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonb, operator>=, vcgeq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonb, operator==, vceqq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonb, operator!=, vmvnq_u32(vceqq_f32(l.mm, r.mm)))

        SIMDEE_UNOP(neonf, neonf, operator-, vnegq_f32(l.mm))
        SIMDEE_BINOP(neonf, neonf, operator+, vaddq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonf, operator-, vsubq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonf, operator*, vmulq_f32(l.mm, r.mm))

        inline friend const neonf operator/(const neonf& l, const neonf& r) {
#if SIMDEE_ARM64
            return vdivq_f32(l.mm, r.mm);
#else
            float res0 = vgetq_lane_f32(l.mm, 0) / vgetq_lane_f32(r.mm, 0);
            float res1 = vgetq_lane_f32(l.mm, 1) / vgetq_lane_f32(r.mm, 1);
            float32x2_t res01 = {res0, res1};
            float res2 = vgetq_lane_f32(l.mm, 2) / vgetq_lane_f32(r.mm, 2);
            float res3 = vgetq_lane_f32(l.mm, 3) / vgetq_lane_f32(r.mm, 3);
            float32x2_t res23 = {res2, res3};
            return vcombine_f32(res01, res23);
#endif
        }

        SIMDEE_BINOP(neonf, neonf, min, vminq_f32(l.mm, r.mm))
        SIMDEE_BINOP(neonf, neonf, max, vmaxq_f32(l.mm, r.mm))

        inline friend const neonf sqrt(const neonf& l) {
#if SIMDEE_ARM64
            return vsqrtq_f32(l.mm);
#else
            float res0 = std::sqrt(vgetq_lane_f32(l.mm, 0));
            float res1 = std::sqrt(vgetq_lane_f32(l.mm, 1));
            float32x2_t res01 = {res0, res1};
            float res2 = std::sqrt(vgetq_lane_f32(l.mm, 2));
            float res3 = std::sqrt(vgetq_lane_f32(l.mm, 3));
            float32x2_t res23 = {res2, res3};
            return vcombine_f32(res01, res23);
#endif
        }

        SIMDEE_INL friend const neonf rsqrt(const neonf& l) {
            float32x4_t res = vrsqrteq_f32(l.mm);
            res = vmulq_f32(res, vrsqrtsq_f32(vmulq_f32(l.mm, res), res));
            return res;
        }

        SIMDEE_INL friend const neonf rcp(const neonf& l) {
            float32x4_t res = vrecpeq_f32(l.mm);
            res = vmulq_f32(res, vrecpsq_f32(l.mm, res));
            return res;
        }

        SIMDEE_UNOP(neonf, neonf, abs, vabsq_f32(l.mm))
    };

    struct neonu final : neon_base<neonu> {
        SIMDEE_NEON_COMMON(neonu, u32, uint32_t)
        SIMDEE_INL explicit neonu(const neonb&);
        SIMDEE_INL explicit neonu(const neons&);
        SIMDEE_CTOR(neonu, not_neonu, mm = vmvnq_u32(r.neg.mm))

#if SIMDEE_NEED_INT
        SIMDEE_BINOP(neonu, neonb, operator<, vcltq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonb, operator>, vcgtq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonb, operator<=, vcleq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonb, operator>=, vcgeq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonb, operator==, vceqq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonb, operator!=, vmvnq_u32(vceqq_u32(l.mm, r.mm)))
        SIMDEE_BINOP(neonu, neonu, operator&, vandq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonu, operator|, vorrq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonu, operator^, veorq_u32(l.mm, r.mm))
        SIMDEE_UNOP(neonu, not_neonu, operator~, not_neonu(l));
        SIMDEE_UNOP(neonu, neonu, operator-,
                    vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(l.mm))))
        SIMDEE_BINOP(neonu, neonu, operator+, vaddq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonu, operator-, vsubq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonu, operator*, vmulq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonu, min, vminq_u32(l.mm, r.mm))
        SIMDEE_BINOP(neonu, neonu, max, vmaxq_u32(l.mm, r.mm))
#endif
    };

    struct neons final : neon_base<neons> {
        SIMDEE_NEON_COMMON(neons, s32, int32_t)
        SIMDEE_INL explicit neons(const neonf&);
        SIMDEE_INL explicit neons(const neonu&);
        SIMDEE_CTOR(neons, not_neons, mm = vmvnq_s32(r.neg.mm))

#if SIMDEE_NEED_INT
        SIMDEE_BINOP(neons, neonb, operator<, vcltq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neonb, operator>, vcgtq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neonb, operator<=, vcleq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neonb, operator>=, vcgeq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neonb, operator==, vceqq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neonb, operator!=, vmvnq_u32(vceqq_s32(l.mm, r.mm)))
        SIMDEE_BINOP(neons, neons, operator&, vandq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neons, operator|, vorrq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neons, operator^, veorq_s32(l.mm, r.mm))
        SIMDEE_UNOP(neons, not_neons, operator~, not_neons(l))
        SIMDEE_UNOP(neons, neons, operator-, vnegq_s32(l.mm))
        SIMDEE_BINOP(neons, neons, operator+, vaddq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neons, operator-, vsubq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neons, operator*, vmulq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neons, min, vminq_s32(l.mm, r.mm))
        SIMDEE_BINOP(neons, neons, max, vmaxq_s32(l.mm, r.mm))
        SIMDEE_UNOP(neons, neons, abs, vabsq_s32(l.mm))
#endif
    };

    SIMDEE_INL neonf::neonf(const neons& r) { mm = vcvtq_f32_s32(r.data()); }
    SIMDEE_INL neons::neons(const neonf& r) { mm = vcvtq_s32_f32(r.data()); }
    SIMDEE_INL neonu::neonu(const neonb& r) { mm = r.data(); }
    SIMDEE_INL neonu::neonu(const neons& r) { mm = vreinterpretq_u32_s32(r.data()); }
    SIMDEE_INL neons::neons(const neonu& r) { mm = vreinterpretq_s32_u32(r.data()); }

    SIMDEE_INL const neonb cond(const neonb& pred, const neonb& if_true, const neonb& if_false) {
        return vbslq_u32(pred.data(), if_true.data(), if_false.data());
    }

    SIMDEE_INL const neonf cond(const neonb& pred, const neonf& if_true, const neonf& if_false) {
        return vbslq_f32(pred.data(), if_true.data(), if_false.data());
    }

    SIMDEE_INL const neonu cond(const neonb& pred, const neonu& if_true, const neonu& if_false) {
        return vbslq_u32(pred.data(), if_true.data(), if_false.data());
    }

    SIMDEE_INL const neons cond(const neonb& pred, const neons& if_true, const neons& if_false) {
        return vbslq_s32(pred.data(), if_true.data(), if_false.data());
    }

    SIMDEE_NEON_COMMON_FREESTANDING(neonb, u32)
    SIMDEE_NEON_COMMON_FREESTANDING(neonf, f32)
    SIMDEE_NEON_COMMON_FREESTANDING(neonu, u32)
    SIMDEE_NEON_COMMON_FREESTANDING(neons, s32)

    namespace impl {

        template <typename T>
        struct special_ops;

        template <typename T>
        struct neonbu_special_ops {
            SIMDEE_INL static T andnot(const T& l, const T& r) {
                return vbicq_u32(l.data(), r.data());
            }
            SIMDEE_INL static T ornot(const T& l, const T& r) {
                return vornq_u32(l.data(), r.data());
            }
        };

        template <>
        struct special_ops<neonb> : neonbu_special_ops<neonb> {};

        template <>
        struct special_ops<neonu> : neonbu_special_ops<neonu> {};

        template <>
        struct special_ops<neons> {
            SIMDEE_INL static neons andnot(const neons& l, const neons& r) {
                return vbicq_s32(l.data(), r.data());
            }
            SIMDEE_INL static neons ornot(const neons& l, const neons& r) {
                return vornq_s32(l.data(), r.data());
            }
        };

    } // namespace impl

} // namespace sd

#endif // SIMDEE_SIMD_TYPES_NEON_HPP
