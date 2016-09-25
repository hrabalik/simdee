#ifndef SIMDIFY_COMMON_MASK_HPP
#define SIMDIFY_COMMON_MASK_HPP

#include "../util/inline.hpp"
#include "../util/trivial_type.hpp"
#include <cstdint>

namespace simd {
    namespace impl {
        // result of the mask operation
        template <uint32_t AllBitsMask>
        struct mask {
            SIMDIFY_TRIVIAL_TYPE(mask);

            SIMDIFY_INL constexpr mask(uint32_t i) : value(i) {}

            SIMDIFY_INL constexpr mask operator&(mask r) const { return mask(value & r.value); }
            SIMDIFY_INL constexpr mask operator|(mask r) const { return mask(value | r.value); }
            SIMDIFY_INL constexpr mask operator^(mask r) const { return mask(value ^ r.value); }
            SIMDIFY_INL mask& operator&=(mask r) { value &= r.value; return *this; }
            SIMDIFY_INL mask& operator|=(mask r) { value |= r.value; return *this; }
            SIMDIFY_INL mask& operator^=(mask r) { value ^= r.value; return *this; }
            SIMDIFY_INL constexpr mask operator~() const { return mask(value ^ AllBitsMask); }
            SIMDIFY_INL constexpr bool any() const { return value != 0; }
            SIMDIFY_INL constexpr bool all() const { return value == AllBitsMask; }
            SIMDIFY_INL constexpr bit_iterator begin() const { return bit_iterator(value); }
            SIMDIFY_INL constexpr bit_iterator end() const { return bit_iterator(value); }

            //data
            uint32_t value;
        };
    }

    template <typename T>
    using mask = typename T::mask_t;
}

#endif // SIMDIFY_COMMON_MASK_HPP
