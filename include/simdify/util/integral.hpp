#ifndef SIMDIFY_UTIL_INTEGRAL
#define SIMDIFY_UTIL_INTEGRAL

#include "inline.hpp"
#include <cstdint>
#include <type_traits>

namespace simd {

    template <std::size_t Size>
    struct select_sint;
    template <>
    struct select_sint<1> { using type = int8_t; };
    template <>
    struct select_sint<2> { using type = int16_t; };
    template <>
    struct select_sint<4> { using type = int32_t; };
    template <>
    struct select_sint<8> { using type = int64_t; };

    template <std::size_t Size>
    struct select_uint;
    template <>
    struct select_uint<1> { using type = uint8_t; };
    template <>
    struct select_uint<2> { using type = uint16_t; };
    template <>
    struct select_uint<4> { using type = uint32_t; };
    template <>
    struct select_uint<8> { using type = uint64_t; };

    template <std::size_t Size>
    using select_sint_t = typename select_sint<Size>::type;
    template <std::size_t Size>
    using select_uint_t = typename select_uint<Size>::type;

    template <std::size_t x>
    struct is_power_of_2 : std::integral_constant < bool, x && (x & (x - 1)) == 0 > {};

    template <std::size_t b>
    SIMDIFY_FORCE_INLINE std::size_t div_floor(std::size_t a) { return a / b; }

    template <std::size_t b>
    SIMDIFY_FORCE_INLINE std::size_t div_ceil(std::size_t a) { return (a + (b - 1)) / b; }

    template <std::size_t b>
    SIMDIFY_FORCE_INLINE std::size_t div_floor_shift(std::size_t a) {
        if (is_power_of_2<b>::value)
            return a & ~(b - 1);
        else
            return (a / b) * b;
    }

    template <std::size_t b>
    SIMDIFY_FORCE_INLINE std::size_t div_ceil_shift(std::size_t a) {
        if (is_power_of_2<b>::value)
            return (a + (b - 1)) & ~(b - 1);
        else
            return ((a + (b - 1)) / b) * b;
    }

    template <std::size_t b>
    SIMDIFY_FORCE_INLINE std::size_t mod(std::size_t a) {
        if (is_power_of_2<b>::value)
            return a & (b - 1);
        else
            return a % b;
    }

}

#endif // SIMDIFY_UTIL_INTEGRAL
