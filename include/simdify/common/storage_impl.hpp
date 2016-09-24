#ifndef SIMDIFY_COMMON_STORAGE_HPP
#define SIMDIFY_COMMON_STORAGE_HPP

#include "expr.hpp"
#include "../simd_types/common.hpp"
#include "../util/inline.hpp"
#include <array>

namespace simd {

    //
    // storage for SIMD types
    //
    template <typename T, typename Enable = void>
    struct storage;

    //
    // storage specialization for SIMD types with width > 1
    //
    template <typename Simd_t>
    struct alignas(Simd_t)storage<Simd_t, typename std::enable_if<(Simd_t::W > 1)>::type> : std::array<typename Simd_t::scalar_t, Simd_t::W> {
        using stored_t = Simd_t;
        using scalar_t = typename Simd_t::scalar_t;
        using array_t = std::array<scalar_t, Simd_t::W>;
        using array_t::data;

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
    // storage specialization for SIMD types with width == 0
    //
    template <typename Simd_t>
    struct storage<Simd_t, typename std::enable_if<(Simd_t::W == 1)>::type> : std::array<typename Simd_t::scalar_t, Simd_t::W> {
        using stored_t = Simd_t;
        using scalar_t = typename Simd_t::scalar_t;
        using array_t = std::array<scalar_t, Simd_t::W>;
        using array_t::data;

        SIMDIFY_INL constexpr storage() = default;
        SIMDIFY_INL constexpr storage(const storage&) = default;
        SIMDIFY_INL storage(const Simd_t& rhs) { rhs.aligned_store(data()); }

        SIMDIFY_INL storage& operator=(const storage&) = default;
        SIMDIFY_INL storage& operator=(const Simd_t& rhs) {
            rhs.aligned_store(data());
            return *this;
        }

        // implicit conversion to Simd_t
        SIMDIFY_INL operator Simd_t() const { Simd_t s; s.aligned_load(data()); return s; }
    };

    template <typename T>
    SIMDIFY_INL constexpr auto tof(storage<T> r) -> decltype(tof(T(r))) { return tof(T(r)); }

    template <typename T>
    SIMDIFY_INL constexpr auto tou(storage<T> r) -> decltype(tou(T(r))) { return tou(T(r)); }

    template <typename T>
    SIMDIFY_INL constexpr auto tos(storage<T> r) -> decltype(tos(T(r))) { return tos(T(r)); }
}

#endif // SIMDIFY_COMMON_STORAGE_HPP
