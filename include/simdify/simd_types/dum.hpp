#ifndef SIMDIFY_SIMD_TYPES_DUM_HPP
#define SIMDIFY_SIMD_TYPES_DUM_HPP

#include "common.hpp"

// set feature flag
#define SIMDIFY_HAVE_DUM

#include <cmath>

namespace simd {

    struct dumf;
    struct dumu;
    struct dums;

    template<>
    struct is_simd_type<dumf> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dumu> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dums> : std::integral_constant<bool, true> {};

    template <typename Vector_t>
    struct dum_traits {
        using vector_t = Vector_t;
        using scalar_t = Vector_t;
        using vec_f = dumf;
        using vec_u = dumu;
        using vec_s = dums;
    };

    template <>
    struct simd_type_traits<dumf> : dum_traits<float> {};

    template <>
    struct simd_type_traits<dumu> : dum_traits<uint32_t> {};

    template <>
    struct simd_type_traits<dums> : dum_traits<int32_t> {};

    template <typename Crtp>
    struct dum_base : simd_base<Crtp> {
        SIMDIFY_TRIVIAL_TYPE(dum_base);

        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using simd_base<Crtp>::mm;
        using simd_base<Crtp>::W;
        using simd_base<Crtp>::self;

        SIMDIFY_INL dum_base(const scalar_t& r) {
            mm = r;
        }

        SIMDIFY_INL Crtp& operator=(const scalar_t& r) {
            mm = r;
            return self();
        }

        template <typename T>
        SIMDIFY_INL dum_base(const expr::aligned<T>& r) {
            aligned_load(r.ptr);
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::aligned<T>& r) {
            aligned_load(r.ptr);
            return self();
        }

        template <typename T>
        SIMDIFY_INL dum_base(const expr::unaligned<T>& r) {
            unaligned_load(r.ptr);
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::unaligned<T>& r) {
            unaligned_load(r.ptr);
            return self();
        }

        template <typename T>
        SIMDIFY_INL dum_base(const expr::init<T>& r) {
            *this = r.template to<scalar_t>();
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::init<T>& r) {
            *this = r.template to<scalar_t>();
            return self();
        }

        SIMDIFY_INL void aligned_load(const scalar_t* r) {
            mm = *r;
        }

        SIMDIFY_INL void aligned_store(scalar_t* r) const {
            *r = mm;
        }

        SIMDIFY_INL void unaligned_load(const scalar_t* r) {
            mm = *r;
        }

        SIMDIFY_INL void unaligned_store(scalar_t* r) {
            *r = mm;
        }

        void interleaved_load(const scalar_t* r, std::size_t step) {
            mm = *r;
        }

        void interleaved_store(scalar_t* r, std::size_t step) const {
            *r = mm;
        }

        using binary_op_t = const Crtp(*)(const Crtp& l, const Crtp& r);
        SIMDIFY_INL const Crtp reduce(binary_op_t f) const { return self(); }
    };

    struct dumf : dum_base<dumf> {
        SIMDIFY_TRIVIAL_TYPE(dumf);

        using dum_base::dum_base;
        SIMDIFY_INL explicit dumf(const dums&);

        SIMDIFY_INL scalar_t first_element() const { return mm; }
    };

    struct dumu : dum_base<dumu> {
        SIMDIFY_TRIVIAL_TYPE(dumu);

        using dum_base::dum_base;
        SIMDIFY_INL explicit dumu(const dums&);

        SIMDIFY_INL mask_t mask() const { return nth_bit(simd::tou(mm), 31); }
        SIMDIFY_INL bool any() const { return mask() != 0; }
        SIMDIFY_INL bool all() const { return mask() != 0; }
        SIMDIFY_INL scalar_t first_element() const { return mm; }
        
    private:
        friend struct expr::bit_not<dumu>;
        SIMDIFY_INL mask_t not_mask() const { return mask() ^ 1U; }
    };

    struct dums : dum_base<dums> {
        SIMDIFY_TRIVIAL_TYPE(dums);

        using dum_base::dum_base;

        SIMDIFY_INL explicit dums(const dumf&);
        SIMDIFY_INL explicit dums(const dumu&);

        SIMDIFY_INL scalar_t first_element() const { return mm; }
    };

    SIMDIFY_INL dumf::dumf(const dums& r) { mm = static_cast<scalar_t>(r.mm); }
    SIMDIFY_INL dums::dums(const dumf& r) { mm = round_to_int32(r.mm); }
    SIMDIFY_INL dumu::dumu(const dums& r) { mm = static_cast<scalar_t>(r.mm); }
    SIMDIFY_INL dums::dums(const dumu& r) { mm = static_cast<scalar_t>(r.mm); }

    SIMDIFY_INL const dumu operator&(const dumu& l, const dumu& r) { return uval(tou(l.mm) & tou(r.mm)); }
    SIMDIFY_INL const dumu operator|(const dumu& l, const dumu& r) { return uval(tou(l.mm) | tou(r.mm)); }
    SIMDIFY_INL const dumu operator^(const dumu& l, const dumu& r) { return uval(tou(l.mm) ^ tou(r.mm)); }
    SIMDIFY_INL const dumu operator~(const dumu& l) { return uval(~tou(l.mm)); }
    SIMDIFY_INL const dumu andnot(const dumu& l, const dumu& r) { return uval(tou(l.mm) & ~tou(r.mm)); }

    SIMDIFY_INL const dumu operator<(const dumf& l, const dumf& r) { return (l.mm < r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator>(const dumf& l, const dumf& r) { return (l.mm > r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator<=(const dumf& l, const dumf& r) { return (l.mm <= r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator>=(const dumf& l, const dumf& r) { return (l.mm >= r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator==(const dumf& l, const dumf& r) { return (l.mm == r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator!=(const dumf& l, const dumf& r) { return (l.mm != r.mm) ? ~0U : 0U; }

    SIMDIFY_INL const dumf operator-(const dumf& in) { return -in.mm; }
    SIMDIFY_INL const dumf operator+(const dumf& l, const dumf& r) { return l.mm + r.mm; }
    SIMDIFY_INL const dumf operator-(const dumf& l, const dumf& r) { return l.mm - r.mm; }
    SIMDIFY_INL const dumf operator*(const dumf& l, const dumf& r) { return l.mm * r.mm; }
    SIMDIFY_INL const dumf operator/(const dumf& l, const dumf& r) { return l.mm / r.mm; }

    SIMDIFY_INL const dumf min(const dumf& l, const dumf& r) { return std::min(l.mm, r.mm); }
    SIMDIFY_INL const dumf max(const dumf& l, const dumf& r) { return std::max(l.mm, r.mm); }
    SIMDIFY_INL const dumf sqrt(const dumf& l) { return std::sqrt(l.mm); }
    SIMDIFY_INL const dumf rsqrt(const dumf& l) { return 1 / std::sqrt(l.mm); }
    SIMDIFY_INL const dumf rcp(const dumf& l) { return 1 / l.mm; }
    SIMDIFY_INL const dumf abs(const dumf& l) { return std::abs(l.mm); }

    SIMDIFY_INL const dumf cond(const dumu& pred, const dumf& if_true, const dumf& if_false) {
        return pred.any() ? if_true : if_false;
    }
    SIMDIFY_INL const dumu cond(const dumu& pred, const dumu& if_true, const dumu& if_false) {
        return pred.any() ? if_true : if_false;
    }
    SIMDIFY_INL const dums cond(const dumu& pred, const dums& if_true, const dums& if_false) {
        return pred.any() ? if_true : if_false;
    }

#if defined(SIMDIFY_NEED_INT)
    SIMDIFY_INL const dumu operator<(const dums& l, const dums& r) { return (l.mm < r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator>(const dums& l, const dums& r) { return (l.mm > r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator<=(const dums& l, const dums& r) { return (l.mm <= r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator>=(const dums& l, const dums& r) { return (l.mm >= r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator==(const dums& l, const dums& r) { return (l.mm == r.mm) ? ~0U : 0U; }
    SIMDIFY_INL const dumu operator!=(const dums& l, const dums& r) { return (l.mm != r.mm) ? ~0U : 0U; }

    SIMDIFY_INL const dums operator-(const dums& in) { return -in.mm; }
    SIMDIFY_INL const dums operator+(const dums& l, const dums& r) { return l.mm + r.mm; }
    SIMDIFY_INL const dums operator-(const dums& l, const dums& r) { return l.mm - r.mm; }
    SIMDIFY_INL const dums operator*(const dums& l, const dums& r) { return l.mm * r.mm; }

    SIMDIFY_INL const dums min(const dums& l, const dums& r) { return std::min(l.mm, r.mm); }
    SIMDIFY_INL const dums max(const dums& l, const dums& r) { return std::max(l.mm, r.mm); }
    SIMDIFY_INL const dums abs(const dums& l) { return std::abs(l.mm); }
#endif

}

#undef SIMDIFY_DUM_COMMON_DECLARATIONS

#endif // SIMDIFY_SIMD_TYPES_DUM_HPP
