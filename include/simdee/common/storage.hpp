// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_STORAGE_HPP
#define SIMDEE_COMMON_STORAGE_HPP

#include "../util/inline.hpp"
#include <array>
#include <algorithm>
#include <initializer_list>

namespace sd {
    namespace impl {
        //
        // storage for SIMD types
        //
        template <typename Simd_t, typename Scalar_t, std::size_t Align>
        struct alignas(Align)storage : std::array<Scalar_t, Simd_t::width> {
            using stored_t = Simd_t;
            using array_t = std::array<Scalar_t, Simd_t::width>;
            using array_t::data;
            using array_t::begin;

            SIMDEE_INL constexpr storage() = default;
            SIMDEE_INL constexpr storage(const storage&) = default;
            SIMDEE_INL explicit storage(const Simd_t& rhs) {
                rhs.aligned_store(data());
            }
            storage(std::initializer_list<Scalar_t> list) {
                std::copy(list.begin(), list.end(), begin());
            }

            SIMDEE_INL storage& operator=(const storage&) = default;
            SIMDEE_INL storage& operator=(const Simd_t& rhs) {
                rhs.aligned_store(data());
                return *this;
            }
            storage& operator=(std::initializer_list<Scalar_t> list) {
                std::copy(list.begin(), list.end(), begin());
                return *this;
            }
        };
    }

    template <typename T>
    using storage = typename T::storage_t;
}

#endif // SIMDEE_COMMON_STORAGE_HPP
