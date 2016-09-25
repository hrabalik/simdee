#ifndef SIMDIFY_SIMD_TYPES_COMMON_HPP
#define SIMDIFY_SIMD_TYPES_COMMON_HPP

#include "../util/inline.hpp"
#include "../util/bitfiddle.hpp"
#include "../util/integral.hpp"
#include "../util/trivial_type.hpp"
#include "../common/expr.hpp"
#include "../common/mask.hpp"
#include <array>
#include <type_traits>

namespace simd {

    //
    // forward declarations
    //
    template <typename T>
    struct is_simd_type : std::integral_constant<bool, false> {};
    template <typename T>
    struct simd_type_traits;

    //
    // SIMD type base class (with derived class as Crtp, CRTP-style)
    //
    template <typename Crtp>
    struct simd_base {
        using traits_t = simd_type_traits<Crtp>;
        using vector_t = typename traits_t::vector_t;
        using scalar_t = typename traits_t::scalar_t;
        using mask_t = typename traits_t::mask_t;

        enum : std::size_t {
            W = sizeof(vector_t) / sizeof(scalar_t)
        };

        SIMDIFY_INL Crtp& self() {
            return static_cast<Crtp&>(*this);
        }

        SIMDIFY_INL constexpr const Crtp& self() const {
            return static_cast<const Crtp&>(*this);
        }

        // data
        vector_t mm;
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
    template <typename T> SIMDIFY_INL const T max_mask(const simd_base<T>& in, const typename simd_type_traits<T>::vec_u& mask) {
        return cond(mask, in.self(), simd::ninf());
    }
    template <typename T> SIMDIFY_INL const T min_mask(const simd_base<T>& in, const typename simd_type_traits<T>::vec_u& mask) {
        return cond(mask, in.self(), simd::inf());
    }
    template <typename T> SIMDIFY_INL const T sum_mask(const simd_base<T>& in, const typename simd_type_traits<T>::vec_u& mask) {
        return cond(mask, in.self(), simd::zero());
    }
    template <typename T> SIMDIFY_INL const T product_mask(const simd_base<T>& in, const typename simd_type_traits<T>::vec_u& mask) {
        return cond(mask, in.self(), 1);
    }

}

#endif // SIMDIFY_SIMD_TYPES_COMMON_HPP
