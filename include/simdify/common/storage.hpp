#ifndef SIMDIFY_COMMON_STORAGE_HPP
#define SIMDIFY_COMMON_STORAGE_HPP

#include "expr.hpp"
#include "../simd_types/common.hpp"
#include "../util/inline.hpp"
#include <array>

namespace simd {

    //
    // storage for SIMD or floating-point types
    //
    template <typename T, typename Enable = void>
    struct storage;

    //
    // specialized storage for SIMD types
    //
    template <typename Simd_t>
    struct alignas(Simd_t)storage<Simd_t, typename std::enable_if<is_simd_type<Simd_t>::value>::type> : std::array<typename Simd_t::scalar_t, Simd_t::W> {
        using stored_t = Simd_t;
        using scalar_t = typename Simd_t::scalar_t;
        using array_t = std::array<scalar_t, Simd_t::W>;

        SIMDIFY_INL constexpr storage() = default;
        SIMDIFY_INL constexpr storage(const storage&) = default;
        SIMDIFY_INL constexpr storage(const array_t& rhs) : array_t(rhs) {}
        SIMDIFY_INL explicit storage(const Simd_t& rhs) { rhs.aligned_store(data()); }

        SIMDIFY_INL storage& operator=(const storage&) = default;
        SIMDIFY_INL storage& operator=(const array_t& rhs) {
            static_cast<array_t&>(*this) = rhs;
            return *this;
        }
        SIMDIFY_INL storage& operator=(const Simd_t& rhs) {
            rhs.aligned_store(data());
            return *this;
        }

        // implicit conversion to Simd_t
        SIMDIFY_INL operator Simd_t() const { Simd_t s; s.aligned_load(data()); return s; }
    };

    //
    // specialized storage for arithmetic types
    //
    template <typename Scalar_t>
    struct storage<Scalar_t, typename std::enable_if<std::is_arithmetic<Scalar_t>::value>::type> : std::array<Scalar_t, 1> {
        using stored_t = Scalar_t;
        using scalar_t = Scalar_t;
        using array_t = std::array<Scalar_t, 1>;

        SIMDIFY_INL constexpr storage() = default;
        SIMDIFY_INL constexpr storage(const storage&) = default;
        SIMDIFY_INL constexpr storage(const array_t& rhs) : array_t{ rhs } {}
        SIMDIFY_INL constexpr explicit storage(const Scalar_t& rhs) { *data() = rhs; }

        SIMDIFY_INL storage& operator=(const storage&) = default;
        SIMDIFY_INL storage& operator=(const array_t& rhs) {
            static_cast<array_t&>(*this) = rhs;
            return *this;
        }
        SIMDIFY_INL storage& operator=(const Scalar_t& rhs) {
            *data() = rhs;
            return *this;
        }

        // implicit conversion to Scalar_t
        SIMDIFY_INL operator Scalar_t() const { return m_data; }
    };

    template <typename T>
    SIMDIFY_INL constexpr auto tof(storage<T> r) -> decltype(tof(T(r))) { return tof(T(r)); }

    template <typename T>
    SIMDIFY_INL constexpr auto tou(storage<T> r) -> decltype(tou(T(r))) { return tou(T(r)); }

    template <typename T>
    SIMDIFY_INL constexpr auto tos(storage<T> r) -> decltype(tos(T(r))) { return tos(T(r)); }
}

#endif // SIMDIFY_COMMON_STORAGE_HPP
