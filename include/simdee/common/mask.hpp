// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_MASK_HPP
#define SIMDEE_COMMON_MASK_HPP

#include "../util/inline.hpp"
#include "../util/bitfiddle.hpp"
#include <cstdint>

namespace sd {
    namespace impl {
        template <uint32_t AllBitsMask>
        struct mask {
            enum : uint32_t { all_bits = AllBitsMask };

            SIMDEE_INL mask(const mask&) = default;                                                               \
            SIMDEE_INL mask& operator=(const mask&) = default;

            SIMDEE_INL constexpr explicit mask(uint32_t i) : value(i) {}

            SIMDEE_INL constexpr bool operator[](int i) const { return (value & (1U << i)) != 0; }
            SIMDEE_INL constexpr mask operator&(mask r) const { return mask(value & r.value); }
            SIMDEE_INL constexpr mask operator|(mask r) const { return mask(value | r.value); }
            SIMDEE_INL constexpr mask operator^(mask r) const { return mask(value ^ r.value); }
            SIMDEE_INL mask& operator&=(mask r) { value &= r.value; return *this; }
            SIMDEE_INL mask& operator|=(mask r) { value |= r.value; return *this; }
            SIMDEE_INL mask& operator^=(mask r) { value ^= r.value; return *this; }
            SIMDEE_INL constexpr mask operator~() const { return mask(value ^ all_bits); }
            SIMDEE_INL constexpr bool operator==(mask r) const { return value == r.value; }
            SIMDEE_INL constexpr bool operator!=(mask r) const { return value != r.value; }
            SIMDEE_INL constexpr bit_iterator begin() const { return bit_iterator(value); }
            SIMDEE_INL constexpr bit_iterator end() const { return bit_iterator(0); }

            SIMDEE_INL friend constexpr bool any(mask l) {
                return l.value != 0;
            }
            SIMDEE_INL friend constexpr bool all(mask l) {
                return l.value == all_bits;
            }

            //data
            uint32_t value;
        };
    }
}

#endif // SIMDEE_COMMON_MASK_HPP
