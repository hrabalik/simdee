// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_DUM_HPP
#define SIMDEE_SIMD_TYPES_DUM_HPP

#include "common.hpp"

#define SIMDEE_HAVE_DUM

#include <cmath>

namespace sd {

    struct dumf;
    struct dumu;
    struct dums;

    template<>
    struct is_simd_type<dumf> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dumu> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dums> : std::integral_constant<bool, true> {};

    template <typename Simd_t, typename Vector_t>
    struct dum_traits {
        using simd_t = Simd_t;
        using vector_t = Vector_t;
        using scalar_t = Vector_t;
        using vec_f = dumf;
        using vec_u = dumu;
        using vec_s = dums;
        using mask_t = impl::mask<1U>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <>
    struct simd_type_traits<dumf> : dum_traits<dumf, float> {};

    template <>
    struct simd_type_traits<dumu> : dum_traits<dumu, uint32_t> {};

    template <>
    struct simd_type_traits<dums> : dum_traits<dums, int32_t> {};

    template <typename Crtp>
    struct dum_base : simd_base<Crtp> {
    protected:
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
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

        SIMDEE_INL void aligned_load(const scalar_t* r) {
            mm = *r;
        }

        SIMDEE_INL void aligned_store(scalar_t* r) const {
            *r = mm;
        }

        SIMDEE_INL void unaligned_load(const scalar_t* r) {
            mm = *r;
        }

        SIMDEE_INL void unaligned_store(scalar_t* r) const {
            *r = mm;
        }

        void interleaved_load(const scalar_t* r, std::size_t step) {
            mm = *r;
        }

        void interleaved_store(scalar_t* r, std::size_t step) const {
            *r = mm;
        }

        SIMDEE_INL const Crtp reduce(binary_op_t f) const { return self(); }
    };

    struct dumf : dum_base<dumf> {
        SIMDEE_TRIVIAL_TYPE(dumf);

        using dum_base::dum_base;
        SIMDEE_INL explicit dumf(const dums&);

        SIMDEE_INL scalar_t first_element() const { return mm; }
    };

    struct dumu : dum_base<dumu> {
        SIMDEE_TRIVIAL_TYPE(dumu);

        using dum_base::dum_base;
        SIMDEE_INL explicit dumu(const dums&);

        SIMDEE_INL mask_t mask() const { return mask_t(sd::tou(mm) >> 31); }
        SIMDEE_INL scalar_t first_element() const { return mm; }
    };

    struct dums : dum_base<dums> {
        SIMDEE_TRIVIAL_TYPE(dums);

        using dum_base::dum_base;

        SIMDEE_INL explicit dums(const dumf&);
        SIMDEE_INL explicit dums(const dumu&);

        SIMDEE_INL scalar_t first_element() const { return mm; }
    };

    SIMDEE_INL dumf::dumf(const dums& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dums::dums(const dumf& r) { mm = round_to_int32(r.data()); }
    SIMDEE_INL dumu::dumu(const dums& r) { mm = static_cast<scalar_t>(r.data()); }
    SIMDEE_INL dums::dums(const dumu& r) { mm = static_cast<scalar_t>(r.data()); }

    SIMDEE_INL const dumu operator&(const dumu& l, const dumu& r) { return uval(tou(l.data()) & tou(r.data())); }
    SIMDEE_INL const dumu operator|(const dumu& l, const dumu& r) { return uval(tou(l.data()) | tou(r.data())); }
    SIMDEE_INL const dumu operator^(const dumu& l, const dumu& r) { return uval(tou(l.data()) ^ tou(r.data())); }
    SIMDEE_INL const dumu operator~(const dumu& l) { return uval(~tou(l.data())); }
    SIMDEE_INL const dumu andnot(const dumu& l, const dumu& r) { return uval(tou(l.data()) & ~tou(r.data())); }

    SIMDEE_INL const dumu operator<(const dumf& l, const dumf& r) { return (l.data() < r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator>(const dumf& l, const dumf& r) { return (l.data() > r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator<=(const dumf& l, const dumf& r) { return (l.data() <= r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator>=(const dumf& l, const dumf& r) { return (l.data() >= r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator==(const dumf& l, const dumf& r) { return (l.data() == r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator!=(const dumf& l, const dumf& r) { return (l.data() != r.data()) ? ~0U : 0U; }

    SIMDEE_INL const dumf operator-(const dumf& in) { return -in.data(); }
    SIMDEE_INL const dumf operator+(const dumf& l, const dumf& r) { return l.data() + r.data(); }
    SIMDEE_INL const dumf operator-(const dumf& l, const dumf& r) { return l.data() - r.data(); }
    SIMDEE_INL const dumf operator*(const dumf& l, const dumf& r) { return l.data() * r.data(); }
    SIMDEE_INL const dumf operator/(const dumf& l, const dumf& r) { return l.data() / r.data(); }

    SIMDEE_INL const dumf min(const dumf& l, const dumf& r) { return std::min(l.data(), r.data()); }
    SIMDEE_INL const dumf max(const dumf& l, const dumf& r) { return std::max(l.data(), r.data()); }
    SIMDEE_INL const dumf sqrt(const dumf& l) { return std::sqrt(l.data()); }
    SIMDEE_INL const dumf rsqrt(const dumf& l) { return 1 / std::sqrt(l.data()); }
    SIMDEE_INL const dumf rcp(const dumf& l) { return 1 / l.data(); }
    SIMDEE_INL const dumf abs(const dumf& l) { return std::abs(l.data()); }

    SIMDEE_INL const dumf cond(const dumu& pred, const dumf& if_true, const dumf& if_false) {
        return pred.data() ? if_true : if_false;
    }
    SIMDEE_INL const dumu cond(const dumu& pred, const dumu& if_true, const dumu& if_false) {
        return pred.data() ? if_true : if_false;
    }
    SIMDEE_INL const dums cond(const dumu& pred, const dums& if_true, const dums& if_false) {
        return pred.data() ? if_true : if_false;
    }

#if defined(SIMDEE_NEED_INT)
    SIMDEE_INL const dumu operator==(const dumu& l, const dumu& r) { return (l.data() == r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator!=(const dumu& l, const dumu& r) { return (l.data() != r.data()) ? ~0U : 0U; }

    SIMDEE_INL const dumu operator<(const dums& l, const dums& r) { return (l.data() < r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator>(const dums& l, const dums& r) { return (l.data() > r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator<=(const dums& l, const dums& r) { return (l.data() <= r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator>=(const dums& l, const dums& r) { return (l.data() >= r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator==(const dums& l, const dums& r) { return (l.data() == r.data()) ? ~0U : 0U; }
    SIMDEE_INL const dumu operator!=(const dums& l, const dums& r) { return (l.data() != r.data()) ? ~0U : 0U; }

    SIMDEE_INL const dums operator-(const dums& in) { return -in.data(); }
    SIMDEE_INL const dums operator+(const dums& l, const dums& r) { return l.data() + r.data(); }
    SIMDEE_INL const dums operator-(const dums& l, const dums& r) { return l.data() - r.data(); }
    SIMDEE_INL const dums operator*(const dums& l, const dums& r) { return l.data() * r.data(); }

    SIMDEE_INL const dums min(const dums& l, const dums& r) { return std::min(l.data(), r.data()); }
    SIMDEE_INL const dums max(const dums& l, const dums& r) { return std::max(l.data(), r.data()); }
    SIMDEE_INL const dums abs(const dums& l) { return std::abs(l.data()); }
#endif

}

#undef SIMDEE_DUM_COMMON_DECLARATIONS

#endif // SIMDEE_SIMD_TYPES_DUM_HPP
