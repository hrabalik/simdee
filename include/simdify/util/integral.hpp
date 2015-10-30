#ifndef SIMDIFY_UTIL_INTEGRAL
#define SIMDIFY_UTIL_INTEGRAL

#include "type.hpp"
#include "inline.hpp"
#include <cstdint>
#include <type_traits>

namespace simd {

    template <uint Size>
    struct select_int;
    template <>
    struct select_int<1> { using type = int8_t; };
    template <>
    struct select_int<2> { using type = int16_t; };
    template <>
    struct select_int<4> { using type = int32_t; };
    template <>
    struct select_int<8> { using type = int64_t; };

    template <uint Size>
    struct select_uint;
    template <>
    struct select_uint<1> { using type = uint8_t; };
    template <>
    struct select_uint<2> { using type = uint16_t; };
    template <>
    struct select_uint<4> { using type = uint32_t; };
    template <>
    struct select_uint<8> { using type = uint64_t; };

    template <uint Size>
    using select_int_t = typename select_int<Size>::type;
    template <uint Size>
    using select_uint_t = typename select_uint<Size>::type;

    template <uint x>
    struct is_power_of_2 : std::integral_constant < bool, x && (x & (x - 1)) == 0 > {};

    template <uint b>
    SIMDIFY_FORCE_INLINE uint div_floor(uint a) { return a / b; }

    template <uint b>
    SIMDIFY_FORCE_INLINE uint div_ceil(uint a) { return (a + (b - 1)) / b; }

    template <uint b>
    SIMDIFY_FORCE_INLINE uint div_floor_shift(uint a) {
        if (is_power_of_2<b>::value)
            return a & ~(b - 1);
        else
            return (a / b) * b;
    }

    template <uint b>
    SIMDIFY_FORCE_INLINE uint div_ceil_shift(uint a) {
        if (is_power_of_2<b>::value)
            return (a + (b - 1)) & ~(b - 1);
        else
            return ((a + (b - 1)) / b) * b;
    }

    template <uint b>
    SIMDIFY_FORCE_INLINE uint mod(uint a) {
        if (is_power_of_2<b>::value)
            return a & (b - 1);
        else
            return a % b;
    }

}

#endif // SIMDIFY_UTIL_INTEGRAL
