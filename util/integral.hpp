#ifndef SIMDIFY_UTIL_INTEGRAL
#define SIMDIFY_UTIL_INTEGRAL

#include "type.hpp"
#include "inline.hpp"
#include <type_traits>

namespace simd {

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