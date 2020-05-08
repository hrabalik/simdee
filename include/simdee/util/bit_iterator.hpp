// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_BIT_ITERATOR_HPP
#define SIMDEE_UTIL_BIT_ITERATOR_HPP

#include "inline.hpp"
#include <cstdint>
#include <cstring>
#include <iterator>

#if defined(__GNUC__) // GCC, Clang

namespace sd {

    namespace detail {

        SIMDEE_INL uint32_t lsb(uint32_t in) noexcept { return uint32_t(__builtin_ctz(in)); }

    } // namespace detail

} // namespace sd

#elif defined(_MSC_VER) // Visual Studio

#include <intrin.h>

namespace sd {

    namespace detail {

        SIMDEE_INL uint32_t lsb(uint32_t in) noexcept {
            unsigned long res;
            _BitScanForward(&res, in);
            return uint32_t(res);
        }

    } // namespace detail

} // namespace sd

#else

#error "incompatible compiler"

#endif

namespace sd {
    // provides indices of set (1) bits, ordered from least significant to most significant
    struct bit_iterator : std::iterator<std::input_iterator_tag, uint32_t> {
        uint32_t mask;

        SIMDEE_INL constexpr bit_iterator(uint32_t mask_) noexcept : mask(mask_) {}
        SIMDEE_INL uint32_t operator*() const noexcept { return detail::lsb(mask); }
        SIMDEE_INL bit_iterator& operator++() noexcept {
            mask = mask & (mask - 1);
            return *this;
        }
        SIMDEE_INL bit_iterator operator++(int) noexcept {
            bit_iterator r = mask;
            operator++();
            return r;
        }
        SIMDEE_INL constexpr bool operator==(const bit_iterator& rhs) const noexcept {
            return mask == rhs.mask;
        }
        SIMDEE_INL constexpr bool operator!=(const bit_iterator& rhs) const noexcept {
            return mask != rhs.mask;
        }
    };
} // namespace sd

#endif // SIMDEE_UTIL_BIT_ITERATOR_HPP
