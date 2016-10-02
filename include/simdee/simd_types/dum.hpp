// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_DUM_HPP
#define SIMDEE_SIMD_TYPES_DUM_HPP

#include "common.hpp"

#define SIMDEE_HAVE_DUM

#include <cmath>

namespace sd {

    struct dumb;
    struct dumf;
    struct dumu;
    struct dums;

    template<>
    struct is_simd_type<dumb> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dumf> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dumu> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dums> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Vector_t, typename Element_t>
    struct dum_traits {
        using simd_t = Simd_t;
        using vector_t = Vector_t;
        using scalar_t = Vector_t;
        using element_t = Element_t;
        using vec_f = dumf;
        using vec_u = dumu;
        using vec_s = dums;
        using mask_t = impl::mask<1U>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <>
    struct simd_type_traits<dumb> : dum_traits<dumb, bool32_t, bool> {};
    template <>
    struct simd_type_traits<dumf> : dum_traits<dumf, float, float> {};
    template <>
    struct simd_type_traits<dumu> : dum_traits<dumu, uint32_t, uint32_t> {};
    template <>
    struct simd_type_traits<dums> : dum_traits<dums, int32_t, int32_t> {};

    template <typename Crtp>
    struct dum_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using element_t = typename simd_base<Crtp>::element_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using binary_op_t = typename simd_base<Crtp>::binary_op_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(dum_base);
        SIMDEE_BASE_CTOR(dum_base, scalar_t, mm = r);
        SIMDEE_BASE_CTOR_TPL(dum_base, expr::aligned<T>, aligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(dum_base, expr::unaligned<T>, unaligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(dum_base, expr::init<T>, *this = r.template to<scalar_t>());
        SIMDEE_BASE_CTOR(dum_base, storage_t, aligned_load(r.data()));

        SIMDEE_INL void aligned_load(const scalar_t* r) { mm = *r; }
        SIMDEE_INL void aligned_store(scalar_t* r) const { *r = mm; }
        SIMDEE_INL void unaligned_load(const scalar_t* r) { mm = *r; }
        SIMDEE_INL void unaligned_store(scalar_t* r) const { *r = mm; }
        SIMDEE_INL void interleaved_load(const scalar_t* r, std::size_t step) { mm = *r; }
        SIMDEE_INL void interleaved_store(scalar_t* r, std::size_t step) const { *r = mm; }
        SIMDEE_INL const Crtp reduce(binary_op_t f) const { return self(); }
    };

    struct dumb : dum_base<dumb> {
        SIMDEE_TRIVIAL_TYPE(dumb);

        using dum_base::dum_base;
        SIMDEE_CTOR(dumb, element_t, mm = r ? scalar_t::T : scalar_t::F);

        SIMDEE_INL mask_t mask() const { return mask_t(tou(mm) & 1U); }
        SIMDEE_INL element_t first_element() const { return mm != scalar_t::F; }

        SIMDEE_BINOP(dumb, dumb, operator==, (l.mm == r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumb, dumb, operator!=, (l.mm != r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumb, dumb, operator&&, l.first_element() && r.first_element());
        SIMDEE_BINOP(dumb, dumb, operator||, l.first_element() || r.first_element());
        SIMDEE_UNOP(dumb, dumb, operator!, !l.first_element());
        SIMDEE_BINOP(dumb, dumb, andnot, l.first_element() && !r.first_element());
    };

    struct dumf : dum_base<dumf> {
        SIMDEE_TRIVIAL_TYPE(dumf);

        using dum_base::dum_base;
        SIMDEE_INL explicit dumf(const dums&);

        SIMDEE_INL element_t first_element() const { return mm; }

        SIMDEE_BINOP(dumf, dumb, operator<, (l.mm < r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumf, dumb, operator>, (l.mm > r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumf, dumb, operator<=, (l.mm <= r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumf, dumb, operator>=, (l.mm >= r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumf, dumb, operator==, (l.mm == r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumf, dumb, operator!=, (l.mm != r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);

        SIMDEE_UNOP(dumf, dumf, operator-, -l.mm);
        SIMDEE_BINOP(dumf, dumf, operator+, l.mm + r.mm);
        SIMDEE_BINOP(dumf, dumf, operator-, l.mm - r.mm);
        SIMDEE_BINOP(dumf, dumf, operator*, l.mm * r.mm);
        SIMDEE_BINOP(dumf, dumf, operator/, l.mm / r.mm);

        SIMDEE_BINOP(dumf, dumf, min, std::min(l.mm, r.mm));
        SIMDEE_BINOP(dumf, dumf, max, std::max(l.mm, r.mm));
        SIMDEE_UNOP(dumf, dumf, sqrt, std::sqrt(l.mm));
        SIMDEE_UNOP(dumf, dumf, rsqrt, 1 / std::sqrt(l.mm));
        SIMDEE_UNOP(dumf, dumf, rcp, 1 / l.mm);
        SIMDEE_UNOP(dumf, dumf, abs, std::abs(l.mm));
    };

    struct dumu : dum_base<dumu> {
        SIMDEE_TRIVIAL_TYPE(dumu);

        using dum_base::dum_base;
        SIMDEE_INL explicit dumu(const dumb&);
        SIMDEE_INL explicit dumu(const dums&);

        SIMDEE_INL element_t first_element() const { return mm; }

#   if defined(SIMDEE_NEED_INT)
        SIMDEE_BINOP(dumu, dumb, operator==, (l.mm == r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dumu, dumb, operator!=, (l.mm != r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
#   endif
        SIMDEE_BINOP(dumu, dumu, operator&, uval(tou(l.mm) & tou(r.mm)));
        SIMDEE_BINOP(dumu, dumu, operator|, uval(tou(l.mm) | tou(r.mm)));
        SIMDEE_BINOP(dumu, dumu, operator^, uval(tou(l.mm) ^ tou(r.mm)));
        SIMDEE_UNOP(dumu, dumu, operator~, uval(~tou(l.mm)));
        SIMDEE_BINOP(dumu, dumu, andnot, uval(tou(l.mm) & ~tou(r.mm)));
    };

    struct dums : dum_base<dums> {
        SIMDEE_TRIVIAL_TYPE(dums);

        using dum_base::dum_base;

        SIMDEE_INL explicit dums(const dumf&);
        SIMDEE_INL explicit dums(const dumu&);

        SIMDEE_INL element_t first_element() const { return mm; }

#   if defined(SIMDEE_NEED_INT)
        SIMDEE_BINOP(dums, dumb, operator<, (l.mm < r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dums, dumb, operator>, (l.mm > r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dums, dumb, operator<=, (l.mm <= r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dums, dumb, operator>=, (l.mm >= r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dums, dumb, operator==, (l.mm == r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);
        SIMDEE_BINOP(dums, dumb, operator!=, (l.mm != r.mm) ? dumb::scalar_t::T : dumb::scalar_t::F);

        SIMDEE_UNOP(dums, dums, operator-, -l.mm);
        SIMDEE_BINOP(dums, dums, operator+, l.mm + r.mm);
        SIMDEE_BINOP(dums, dums, operator-, l.mm - r.mm);
        SIMDEE_BINOP(dums, dums, operator*, l.mm * r.mm);

        SIMDEE_BINOP(dums, dums, min, std::min(l.mm, r.mm));
        SIMDEE_BINOP(dums, dums, max, std::max(l.mm, r.mm));
        SIMDEE_UNOP(dums, dums, abs, std::abs(l.mm));
#   endif
    };

    SIMDEE_INL dumf::dumf(const dums& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dums::dums(const dumf& r) { mm = round_to_int32(r.data()); }
    SIMDEE_INL dumu::dumu(const dumb& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dumu::dumu(const dums& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dums::dums(const dumu& r) { mm = static_cast<scalar_t>(r.data()); }

    SIMDEE_INL const dumb cond(const dumb& pred, const dumb& if_true, const dumb& if_false) {
        return pred.first_element() ? if_true : if_false;
    }
    SIMDEE_INL const dumf cond(const dumb& pred, const dumf& if_true, const dumf& if_false) {
        return pred.first_element() ? if_true : if_false;
    }
    SIMDEE_INL const dumu cond(const dumb& pred, const dumu& if_true, const dumu& if_false) {
        return pred.first_element() ? if_true : if_false;
    }
    SIMDEE_INL const dums cond(const dumb& pred, const dums& if_true, const dums& if_false) {
        return pred.first_element() ? if_true : if_false;
    }
}

#undef SIMDEE_DUM_COMMON_DECLARATIONS

#endif // SIMDEE_SIMD_TYPES_DUM_HPP
