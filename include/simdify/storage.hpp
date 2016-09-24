#ifndef SIMDIFY_STORAGE_HPP
#define SIMDIFY_STORAGE_HPP

#include "simd_types/common.hpp"
#include "util/inline.hpp"
#include <array>
#include <algorithm>
#include <initializer_list>

namespace simd {

    //
    // storage for SIMD types
    //
    template <typename Simd_t>
    struct alignas(Simd_t)storage : std::array<typename Simd_t::scalar_t, Simd_t::W> {
        using stored_t = Simd_t;
        using scalar_t = typename Simd_t::scalar_t;
        using array_t = std::array<scalar_t, Simd_t::W>;
        using array_t::data;
        using array_t::begin;

        SIMDIFY_INL constexpr storage() = default;
        SIMDIFY_INL constexpr storage(const storage&) = default;
        SIMDIFY_INL explicit storage(const Simd_t& rhs) { rhs.aligned_store(data()); }
        storage(std::initializer_list<scalar_t> list) {
            std::copy(list.begin(), list.end(), begin());
        }

        SIMDIFY_INL storage& operator=(const storage&) = default;
        SIMDIFY_INL storage& operator=(const Simd_t& rhs) {
            rhs.aligned_store(data());
            return *this;
        }
        storage& operator=(std::initializer_list<scalar_t> list) {
            std::copy(list.begin(), list.end(), begin());
            return *this;
        }

        // implicit conversion to Simd_t
        SIMDIFY_INL operator Simd_t() const { Simd_t s; s.aligned_load(data()); return s; }
    };

}

#endif // SIMDIFY_STORAGE_HPP
