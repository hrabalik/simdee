// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_SELECT_HPP
#define SIMDEE_UTIL_SELECT_HPP

#include "bool_t.hpp"
#include <cstdint>

namespace sd {

    template <std::size_t Size>
    struct select_bool;
    template <>
    struct select_bool<1> {
        using type = bool8_t;
    };
    template <>
    struct select_bool<2> {
        using type = bool16_t;
    };
    template <>
    struct select_bool<4> {
        using type = bool32_t;
    };
    template <>
    struct select_bool<8> {
        using type = bool64_t;
    };

    template <std::size_t Size>
    struct select_sint;
    template <>
    struct select_sint<1> {
        using type = int8_t;
    };
    template <>
    struct select_sint<2> {
        using type = int16_t;
    };
    template <>
    struct select_sint<4> {
        using type = int32_t;
    };
    template <>
    struct select_sint<8> {
        using type = int64_t;
    };

    template <std::size_t Size>
    struct select_uint;
    template <>
    struct select_uint<1> {
        using type = uint8_t;
    };
    template <>
    struct select_uint<2> {
        using type = uint16_t;
    };
    template <>
    struct select_uint<4> {
        using type = uint32_t;
    };
    template <>
    struct select_uint<8> {
        using type = uint64_t;
    };

    template <std::size_t Size>
    struct select_float;
    template <>
    struct select_float<sizeof(float)> {
        using type = float;
    };
    template <>
    struct select_float<sizeof(double)> {
        using type = double;
    };

    template <std::size_t Size>
    using select_bool_t = typename select_bool<Size>::type;
    template <std::size_t Size>
    using select_sint_t = typename select_sint<Size>::type;
    template <std::size_t Size>
    using select_uint_t = typename select_uint<Size>::type;
    template <std::size_t Size>
    using select_float_t = typename select_float<Size>::type;

} // namespace sd

#endif // SIMDEE_UTIL_SELECT_HPP
