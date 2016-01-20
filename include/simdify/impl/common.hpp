#ifndef SIMDIFY_COMMON
#define SIMDIFY_COMMON

#include "../util/inline.hpp"
#include "../util/1b.hpp"
#include "../util/integral.hpp"
#include "expr.hpp"
#include <array>
#include <type_traits>

namespace simd {

    //
    // forward declarations
    //
    template <typename T>
    struct is_simd_type : std::integral_constant<bool, false> {};
    template <typename T>
    struct horizontal_impl;
    template <typename T>
    struct simd_type_traits;

    //
    // SIMD type base class (with derived class as Crtp, CRTP-style)
    //
    template <typename Crtp>
    struct simd_base {
        using traits_t = simd_type_traits<Crtp>;
        using mm_t = typename traits_t::mm_t;
        using e_t = typename traits_t::e_t;

        enum : std::size_t {
            W = sizeof(mm_t) / sizeof(e_t)
        };

        using array_e = std::array<e_t, W>;
        using horizontal = horizontal_impl<Crtp>;

        SIMDIFY_INL Crtp& self() {
            return static_cast<Crtp&>(*this);
        }

        SIMDIFY_INL constexpr const Crtp& self() const {
            return static_cast<const Crtp&>(*this);
        }

        // data
        mm_t mm;
    };

    //
    // meta operations - apply to all SIMD types
    //
    template <typename T> SIMDIFY_INL T& operator&=(simd_base<T>& l, const simd_base<T>& r) {
        l.self() = l.self() & r.self();
        return l.self();
    }
    template <typename T> SIMDIFY_INL T& operator|=(simd_base<T>& l, const simd_base<T>& r) {
        l.self() = l.self() | r.self();
        return l.self();
    }
    template <typename T> SIMDIFY_INL T& operator^=(simd_base<T>& l, const simd_base<T>& r) {
        l.self() = l.self() ^ r.self();
        return l.self();
    }
    template <typename T> SIMDIFY_INL T& operator+=(simd_base<T>& l, const simd_base<T>& r) {
        l.self() = l.self() + r.self();
        return l.self();
    }
    template <typename T> SIMDIFY_INL T& operator-=(simd_base<T>& l, const simd_base<T>& r) {
        l.self() = l.self() - r.self();
        return l.self();
    }
    template <typename T> SIMDIFY_INL T& operator*=(simd_base<T>& l, const simd_base<T>& r) {
        l.self() = l.self() * r.self();
        return l.self();
    }
    template <typename T> SIMDIFY_INL T& operator/=(simd_base<T>& l, const simd_base<T>& r) {
        l.self() = l.self() / r.self();
        return l.self();
    }
    template <typename T> SIMDIFY_INL const T operator+(const simd_base<T>& l) {
        return l.self();
    }
    template <typename T> SIMDIFY_INL const T signum(const simd_base<T>& l) {
        return cond(l.self() > zero(), T(1), T(-1));
    }

    //
    // provides access to min, max, operator+, operator* before they have been declared
    //
    template <typename T>
    struct operators {
        static SIMDIFY_INL const T min_(const T& l, const T& r) { return min(l, r); }
        static SIMDIFY_INL const T max_(const T& l, const T& r) { return max(l, r); }
        static SIMDIFY_INL const T add_(const T& l, const T& r) { return l + r; }
        static SIMDIFY_INL const T mul_(const T& l, const T& r) { return l * r; }
    };

    //
    // base class for horizontal<T>
    //
    template <typename T>
    struct horizontal_impl_base {
        using unary_op_t = const T(*)(const T& in);
        using binary_op_t = const T(*)(const T& l, const T& r);
        using e_t = typename T::e_t;
        using ops = operators<T>;
        using mask_t = typename simd_type_traits<T>::vec_u;

        template <binary_op_t F>
        static SIMDIFY_INL const T reduce(const T& in) { return horizontal_impl<T>::template reduce<F>(in); }

        template <binary_op_t F, typename N>
        static SIMDIFY_INL const T reduce_mask(const T& in, const mask_t& mask, const N& neutral_value) {
            return reduce<F>(cond(mask, in, neutral_value));
        }

        static SIMDIFY_INL const T min(const T& in) { return reduce<ops::min_>(in); }
        static SIMDIFY_INL const T max(const T& in) { return reduce<ops::max_>(in); }
        static SIMDIFY_INL const T sum(const T& in) { return reduce<ops::add_>(in); }
        static SIMDIFY_INL const T product(const T& in) { return reduce<ops::mul_>(in); }

        static SIMDIFY_INL const T min_mask(const T& in, const mask_t& mask) {
            return reduce_mask<ops::min_>(in, mask, std::numeric_limits<e_t>::max());
        }
        static SIMDIFY_INL const T max_mask(const T& in, const mask_t& mask) {
            return reduce_mask<ops::max_>(in, mask, std::numeric_limits<e_t>::lowest());
        }
        static SIMDIFY_INL const T sum_mask(const T& in, const mask_t& mask) {
            return reduce_mask<ops::add_>(in, mask, zero());
        }
        static SIMDIFY_INL const T product_mask(const T& in, const mask_t& mask) {
            return reduce_mask<ops::mul_>(in, mask, e_t(1));
        }
    };

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_TRIVIAL_TYPE( CLASS )                                                                   \
                                                                                                         \
SIMDIFY_INL ~CLASS() = default;                                                                          \
                                                                                                         \
SIMDIFY_INL CLASS() = default;                                                                           \
                                                                                                         \
SIMDIFY_INL CLASS(const CLASS&) = default;                                                               \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(const CLASS&) = default;                                                    \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // SIMDIFY_COMMON
