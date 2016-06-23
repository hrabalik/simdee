#ifndef SIMDIFY_SIMD_TYPES_COMMON_HPP
#define SIMDIFY_SIMD_TYPES_COMMON_HPP

#include "../util/inline.hpp"
#include "../util/bitfiddle.hpp"
#include "../util/integral.hpp"
#include "../common/expr.hpp"
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
        using mm_t = typename traits_t::mm_t;
        using e_t = typename traits_t::e_t;

        enum : std::size_t {
            W = sizeof(mm_t) / sizeof(e_t)
        };

        using array_t = std::array<e_t, W>;

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_TRIVIAL_TYPE( CLASS )                                                                    \
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

#endif // SIMDIFY_SIMD_TYPES_COMMON_HPP
