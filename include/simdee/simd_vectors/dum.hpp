// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_DUM_HPP
#define SIMDEE_SIMD_TYPES_DUM_HPP

#include "common.hpp"

#include <cmath>

namespace sd {
    struct dumb;
    struct dumf;
    struct dumu;
    struct dums;

    template <>
    struct is_simd_vector<dumb> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<dumf> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<dumu> : std::integral_constant<bool, true> {};
    template <>
    struct is_simd_vector<dums> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Vector_t>
    struct dum_traits {
        using simd_t = Simd_t;
        using vector_t = Vector_t;
        using scalar_t = Vector_t;
        using vec_b = dumb;
        using vec_f = dumf;
        using vec_u = dumu;
        using vec_s = dums;
        using mask_t = impl::mask<1U>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <>
    struct simd_vector_traits<dumb> : dum_traits<dumb, bool32_t> {};
    template <>
    struct simd_vector_traits<dumf> : dum_traits<dumf, float> {};
    template <>
    struct simd_vector_traits<dumu> : dum_traits<dumu, uint32_t> {};
    template <>
    struct simd_vector_traits<dums> : dum_traits<dums, int32_t> {};

    template <typename Crtp>
    struct dum_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(dum_base)
        SIMDEE_BASE_CTOR(dum_base, scalar_t, mm = r)
        SIMDEE_BASE_CTOR_TPL(dum_base, expr::aligned<T>, aligned_load(r.ptr))
        SIMDEE_BASE_CTOR_TPL(dum_base, expr::unaligned<T>, unaligned_load(r.ptr))
        SIMDEE_BASE_CTOR_TPL(dum_base, expr::interleaved<T>, interleaved_load(r.ptr, r.step))
        SIMDEE_BASE_CTOR_TPL(dum_base, expr::init<T>, *this = r.template to<scalar_t>())
        SIMDEE_BASE_CTOR(dum_base, storage_t, aligned_load(r.data()))

        SIMDEE_INL void aligned_load(const scalar_t* r) { mm = *r; }
        SIMDEE_INL void aligned_store(scalar_t* r) const { *r = mm; }
        SIMDEE_INL void unaligned_load(const scalar_t* r) { mm = *r; }
        SIMDEE_INL void unaligned_store(scalar_t* r) const { *r = mm; }
        SIMDEE_INL void interleaved_load(const scalar_t* r, int) { mm = *r; }
        SIMDEE_INL void interleaved_store(scalar_t* r, int) const { *r = mm; }

        template <unsigned int Lane>
        SIMDEE_INL const Crtp broadcast() {
            static_assert(Lane == 0, "");
            return mm;
        }

        template <typename Op_t>
        friend SIMDEE_INL const Crtp reduce(const Crtp& l, Op_t) {
            return l;
        }
    };

    struct dumb : dum_base<dumb> {
        SIMDEE_TRIVIAL_TYPE(dumb)

        using dum_base::dum_base;

        SIMDEE_UNOP(dumb, mask_t, mask, mask_t(l.mm))
        SIMDEE_UNOP(dumb, scalar_t, first_scalar, l.mm)
        SIMDEE_BINOP(dumb, dumb, operator==, dumb::scalar_t(l.mm == r.mm))
        SIMDEE_BINOP(dumb, dumb, operator!=, dumb::scalar_t(l.mm != r.mm))
        SIMDEE_BINOP(dumb, dumb, operator&&, first_scalar(l) && first_scalar(r))
        SIMDEE_BINOP(dumb, dumb, operator||, first_scalar(l) || first_scalar(r))
        SIMDEE_UNOP(dumb, dumb, operator!, !first_scalar(l))
    };

    struct dumf : dum_base<dumf> {
        SIMDEE_TRIVIAL_TYPE(dumf)

        using dum_base::dum_base;
        SIMDEE_INL explicit dumf(const dums&);

        SIMDEE_UNOP(dumf, scalar_t, first_scalar, l.mm)
        SIMDEE_BINOP(dumf, dumb, operator<, dumb::scalar_t(l.mm < r.mm))
        SIMDEE_BINOP(dumf, dumb, operator>, dumb::scalar_t(l.mm > r.mm))
        SIMDEE_BINOP(dumf, dumb, operator<=, dumb::scalar_t(l.mm <= r.mm))
        SIMDEE_BINOP(dumf, dumb, operator>=, dumb::scalar_t(l.mm >= r.mm))
        SIMDEE_BINOP(dumf, dumb, operator==, dumb::scalar_t(l.mm == r.mm))
        SIMDEE_BINOP(dumf, dumb, operator!=, dumb::scalar_t(l.mm != r.mm))

        SIMDEE_UNOP(dumf, dumf, operator-, - l.mm)
        SIMDEE_BINOP(dumf, dumf, operator+, l.mm + r.mm)
        SIMDEE_BINOP(dumf, dumf, operator-, l.mm - r.mm)
        SIMDEE_BINOP(dumf, dumf, operator*, l.mm * r.mm)
        SIMDEE_BINOP(dumf, dumf, operator/, l.mm / r.mm)

        SIMDEE_BINOP(dumf, dumf, min, std::min(l.mm, r.mm))
        SIMDEE_BINOP(dumf, dumf, max, std::max(l.mm, r.mm))
        SIMDEE_UNOP(dumf, dumf, sqrt, std::sqrt(l.mm))
        SIMDEE_UNOP(dumf, dumf, rsqrt, 1 / std::sqrt(l.mm))
        SIMDEE_UNOP(dumf, dumf, rcp, 1 / l.mm)
        SIMDEE_UNOP(dumf, dumf, abs, std::abs(l.mm))
    };

    struct dumu : dum_base<dumu> {
        SIMDEE_TRIVIAL_TYPE(dumu)

        using dum_base::dum_base;
        SIMDEE_INL explicit dumu(const dumb&);
        SIMDEE_INL explicit dumu(const dums&);

        SIMDEE_UNOP(dumu, scalar_t, first_scalar, l.mm)

#if SIMDEE_NEED_INT
        SIMDEE_BINOP(dumu, dumb, operator<, dumb::scalar_t(l.mm < r.mm))
        SIMDEE_BINOP(dumu, dumb, operator>, dumb::scalar_t(l.mm > r.mm))
        SIMDEE_BINOP(dumu, dumb, operator<=, dumb::scalar_t(l.mm <= r.mm))
        SIMDEE_BINOP(dumu, dumb, operator>=, dumb::scalar_t(l.mm >= r.mm))
        SIMDEE_BINOP(dumu, dumb, operator==, dumb::scalar_t(l.mm == r.mm))
        SIMDEE_BINOP(dumu, dumb, operator!=, dumb::scalar_t(l.mm != r.mm))
        SIMDEE_BINOP(dumu, dumu, operator&, l.mm & r.mm)
        SIMDEE_BINOP(dumu, dumu, operator|, l.mm | r.mm)
        SIMDEE_BINOP(dumu, dumu, operator^, l.mm ^ r.mm)
        SIMDEE_UNOP(dumu, dumu, operator~, ~l.mm)
        SIMDEE_UNOP(dumu, dumu, operator-, 0 - l.mm)
        SIMDEE_BINOP(dumu, dumu, operator+, l.mm + r.mm)
        SIMDEE_BINOP(dumu, dumu, operator-, l.mm - r.mm)
        SIMDEE_BINOP(dumu, dumu, operator*, l.mm * r.mm)
        SIMDEE_BINOP(dumu, dumu, min, std::min(l.mm, r.mm))
        SIMDEE_BINOP(dumu, dumu, max, std::max(l.mm, r.mm))
#endif
    };

    struct dums : dum_base<dums> {
        SIMDEE_TRIVIAL_TYPE(dums)

        using dum_base::dum_base;

        SIMDEE_INL explicit dums(const dumf&);
        SIMDEE_INL explicit dums(const dumu&);

        SIMDEE_UNOP(dums, scalar_t, first_scalar, l.mm)

#if SIMDEE_NEED_INT
        SIMDEE_BINOP(dums, dumb, operator<, dumb::scalar_t(l.mm < r.mm))
        SIMDEE_BINOP(dums, dumb, operator>, dumb::scalar_t(l.mm > r.mm))
        SIMDEE_BINOP(dums, dumb, operator<=, dumb::scalar_t(l.mm <= r.mm))
        SIMDEE_BINOP(dums, dumb, operator>=, dumb::scalar_t(l.mm >= r.mm))
        SIMDEE_BINOP(dums, dumb, operator==, dumb::scalar_t(l.mm == r.mm))
        SIMDEE_BINOP(dums, dumb, operator!=, dumb::scalar_t(l.mm != r.mm))

        SIMDEE_BINOP(dums, dums, operator&, l.mm & r.mm)
        SIMDEE_BINOP(dums, dums, operator|, l.mm | r.mm)
        SIMDEE_BINOP(dums, dums, operator^, l.mm ^ r.mm)
        SIMDEE_UNOP(dums, dums, operator~, ~l.mm)

        SIMDEE_UNOP(dums, dums, operator-, - l.mm)
        SIMDEE_BINOP(dums, dums, operator+, l.mm + r.mm)
        SIMDEE_BINOP(dums, dums, operator-, l.mm - r.mm)
        SIMDEE_BINOP(dums, dums, operator*, l.mm * r.mm)

        SIMDEE_BINOP(dums, dums, min, std::min(l.mm, r.mm))
        SIMDEE_BINOP(dums, dums, max, std::max(l.mm, r.mm))
        SIMDEE_UNOP(dums, dums, abs, std::abs(l.mm))
#endif
    };

    SIMDEE_INL dumf::dumf(const dums& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dums::dums(const dumf& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dumu::dumu(const dumb& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dumu::dumu(const dums& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dums::dums(const dumu& r) { mm = static_cast<scalar_t>(r.data()); }

    SIMDEE_INL const dumb cond(const dumb& pred, const dumb& if_true, const dumb& if_false) {
        return first_scalar(pred) ? if_true : if_false;
    }
    SIMDEE_INL const dumf cond(const dumb& pred, const dumf& if_true, const dumf& if_false) {
        return first_scalar(pred) ? if_true : if_false;
    }
    SIMDEE_INL const dumu cond(const dumb& pred, const dumu& if_true, const dumu& if_false) {
        return first_scalar(pred) ? if_true : if_false;
    }
    SIMDEE_INL const dums cond(const dumb& pred, const dums& if_true, const dums& if_false) {
        return first_scalar(pred) ? if_true : if_false;
    }
}

#endif // SIMDEE_SIMD_TYPES_DUM_HPP
