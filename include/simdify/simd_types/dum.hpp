#ifndef SIMDIFY_SIMD_TYPES_DUM_HPP
#define SIMDIFY_SIMD_TYPES_DUM_HPP
#define SIMDIFY_HAVE_DUM

#include "common.hpp"
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

    template <typename Mm_t>
    struct dum_traits {
        using mm_t = Mm_t;
        using e_t = Mm_t;
        using f_t = float;
        using u_t = uint32_t;
        using s_t = int32_t;
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

        using mm_t = typename simd_base<Crtp>::mm_t;
        using e_t = typename simd_base<Crtp>::e_t;
        using simd_base<Crtp>::mm;
        using simd_base<Crtp>::W;
        using simd_base<Crtp>::self;

        SIMDIFY_INL dum_base(const e_t& r) {
            mm = r;
        }

        SIMDIFY_INL Crtp& operator=(const e_t& r) {
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
            *this = r.template to<e_t>();
        }

        template <typename T>
        SIMDIFY_INL Crtp& operator=(const expr::init<T>& r) {
            *this = r.template to<e_t>();
            return self();
        }

        SIMDIFY_INL void aligned_load(const e_t* r) {
            mm = *r;
        }

        SIMDIFY_INL void aligned_store(e_t* r) const {
            *r = mm;
        }

        SIMDIFY_INL void unaligned_load(const e_t* r) {
            mm = *r;
        }

        SIMDIFY_INL void unaligned_store(e_t* r) {
            *r = mm;
        }

        void interleaved_load(const e_t* r, std::size_t step) {
            mm = *r;
        }

        void interleaved_store(e_t* r, std::size_t step) const {
            *r = mm;
        }

        using binary_op_t = const Crtp(*)(const Crtp& l, const Crtp& r);
        SIMDIFY_INL const Crtp reduce(binary_op_t f) const { return self(); }
    };

    struct dumf : dum_base<dumf> {
        using dum_base::dum_base;

        SIMDIFY_TRIVIAL_TYPE(dumf);

        SIMDIFY_INL e_t first_element() const { return mm; }
    };

    struct dumu : dum_base<dumu> {
        using dum_base::dum_base;

        SIMDIFY_TRIVIAL_TYPE(dumu);

        SIMDIFY_INL e_t first_element() const { return mm; }

        struct iterator : std::iterator<std::input_iterator_tag, bit_t> {
            bit_t mask;

            SIMDIFY_INL explicit iterator(bit_t mask_) : mask(mask_) {}
            SIMDIFY_INL bit_t operator*() const { return 0; }
            SIMDIFY_INL bit_t operator->() const { return 0; }
            SIMDIFY_INL iterator& operator++() { mask = 0; return *this; }
            SIMDIFY_INL iterator operator++(int) { iterator r{ mask }; mask = 0; return r; }
            SIMDIFY_INL bool operator!=(const iterator& rhs) const { return mask != rhs.mask; }
        };

        SIMDIFY_INL bit_t front() const { return 0; }
        SIMDIFY_INL iterator begin() const { return iterator((simd::tou(mm) & 0x80000000U) >> 31); }
        SIMDIFY_INL iterator end() const { return iterator(0); }
        SIMDIFY_INL bool any() const { return (simd::tou(mm) & 0x80000000U) != 0; }
        SIMDIFY_INL bool all() const { return (simd::tou(mm) & 0x80000000U) != 0; }
    };

    struct dums : dum_base<dums> {
        using dum_base::dum_base;

        SIMDIFY_TRIVIAL_TYPE(dums);

        SIMDIFY_INL e_t first_element() const { return mm; }
    };

    SIMDIFY_INL const dumu operator&(const dumu& l, const dumu& r) { return uval(tou(l.mm) & tou(r.mm)); }
    SIMDIFY_INL const dumu operator|(const dumu& l, const dumu& r) { return uval(tou(l.mm) | tou(r.mm)); }
    SIMDIFY_INL const dumu operator^(const dumu& l, const dumu& r) { return uval(tou(l.mm) ^ tou(r.mm)); }
    SIMDIFY_INL const dumu operator~(const dumu& l) { return uval(~tou(l.mm)); }
    SIMDIFY_INL const dumu nand(const dumu& l, const dumu& r) { return uval(tou(l.mm) & ~tou(r.mm)); }

    SIMDIFY_INL const dumu operator<(const dumf& l, const dumf& r) { return (l.mm < r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator>(const dumf& l, const dumf& r) { return (l.mm > r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator<=(const dumf& l, const dumf& r) { return (l.mm <= r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator>=(const dumf& l, const dumf& r) { return (l.mm >= r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator==(const dumf& l, const dumf& r) { return (l.mm == r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator!=(const dumf& l, const dumf& r) { return (l.mm != r.mm) ? 0xffffffffU : 0; }

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
}

#undef SIMDIFY_DUM_COMMON_DECLARATIONS

#endif // SIMDIFY_SIMD_TYPES_DUM_HPP
