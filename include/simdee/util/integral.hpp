// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_INTEGRAL_HPP
#define SIMDEE_UTIL_INTEGRAL_HPP

#include "inline.hpp"
#include <cstdint>
#include <type_traits>

namespace sd {

    namespace impl {
        template <typename U, typename S>
        struct bool_t {
            bool_t() = default;
            SIMDEE_INL constexpr bool_t(const bool_t&) = default;
            SIMDEE_INL bool_t& operator=(const bool_t&) = default;

            SIMDEE_INL constexpr bool_t(bool value) : data(value ? U(-1) : U(0)) {}
            SIMDEE_INL constexpr explicit bool_t(U value) : data(value) {}
            SIMDEE_INL constexpr explicit bool_t(S value) : data(value) {}
            SIMDEE_INL constexpr operator bool() const { return data != 0; }
            SIMDEE_INL constexpr explicit operator U() const { return data; }
            SIMDEE_INL constexpr explicit operator S() const { return data; }

            SIMDEE_INL constexpr bool_t operator!() const { return bool_t(~data); }
            SIMDEE_INL constexpr bool_t operator&&(const bool_t& r) const { return bool_t(data & r.data); }
            SIMDEE_INL constexpr bool_t operator||(const bool_t& r) const { return bool_t(data | r.data); }

            // data
            U data;
        };
    }

    using bool8_t = impl::bool_t<uint8_t, int8_t>;
    using bool16_t = impl::bool_t<uint16_t, int16_t>;
    using bool32_t = impl::bool_t<uint32_t, int32_t>;
    using bool64_t = impl::bool_t<uint64_t, int64_t>;

    template <typename T>
    struct is_bool_type : std::integral_constant<bool, false
        || std::is_same<T, bool8_t>::value
        || std::is_same<T, bool16_t>::value
        || std::is_same<T, bool32_t>::value
        || std::is_same<T, bool64_t>::value
    > {};

    template <typename T>
    struct is_extended_arithmetic_type : std::integral_constant<bool, false
        || std::is_arithmetic<T>::value
        || is_bool_type<T>::value
    > {};

    template <std::size_t Size>
    struct select_bool;
    template <>
    struct select_bool<1> { using type = bool8_t; };
    template <>
    struct select_bool<2> { using type = bool16_t; };
    template <>
    struct select_bool<4> { using type = bool32_t; };
    template <>
    struct select_bool<8> { using type = bool64_t; };

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
    struct select_float;
    template <>
    struct select_float<sizeof(float)> { using type = float; };
    template <>
    struct select_float<sizeof(double)> { using type = double; };

    template <std::size_t Size>
    using select_bool_t = typename select_bool<Size>::type;
    template <std::size_t Size>
    using select_sint_t = typename select_sint<Size>::type;
    template <std::size_t Size>
    using select_uint_t = typename select_uint<Size>::type;
    template <std::size_t Size>
    using select_float_t = typename select_float<Size>::type;

    template <std::size_t x>
    struct is_power_of_2 : std::integral_constant < bool, x && (x & (x - 1)) == 0 > {};

    template <std::size_t b>
    SIMDEE_INL std::size_t div_floor(std::size_t a) { return a / b; }

    template <std::size_t b>
    SIMDEE_INL std::size_t div_ceil(std::size_t a) { return (a + (b - 1)) / b; }

    template <std::size_t b>
    SIMDEE_INL std::size_t div_floor_mult(std::size_t a) {
        if (is_power_of_2<b>::value)
            return a & ~(b - 1);
        else
            return (a / b) * b;
    }

    template <std::size_t b>
    SIMDEE_INL std::size_t div_ceil_mult(std::size_t a) {
        if (is_power_of_2<b>::value)
            return (a + (b - 1)) & ~(b - 1);
        else
            return ((a + (b - 1)) / b) * b;
    }

    template <std::size_t b>
    SIMDEE_INL std::size_t mod(std::size_t a) {
        if (is_power_of_2<b>::value)
            return a & (b - 1);
        else
            return a % b;
    }

    template <std::size_t... I>
    struct sequence {};

    template <std::size_t I1, std::size_t I2, std::size_t... I>
    struct make_sequence_impl : make_sequence_impl<I1, I2 - 1, I2, I...> {};

    template <std::size_t I1, std::size_t... I>
    struct make_sequence_impl<I1, I1, I...> { using type = sequence<I1, I...>; };

    template <std::size_t I1, std::size_t I2, typename Enable = void>
    struct make_sequence;

    template <std::size_t I1, std::size_t I2>
    struct make_sequence<I1, I2, typename std::enable_if<(I1 < I2)>::type> : make_sequence_impl<I1, I2 - 1> {};

    template <std::size_t I1, std::size_t I2>
    struct make_sequence<I1, I2, typename std::enable_if<(I1 == I2)>::type> { using type = sequence<>; };

    template <std::size_t I1, std::size_t I2>
    struct make_sequence<I1, I2, typename std::enable_if<(I1 > I2)>::type> {
        static_assert(I1 <= I2, "invalid make_sequence: negative size");
    };

    template <std::size_t I1, std::size_t I2>
    using make_sequence_t = typename make_sequence<I1, I2>::type;

}

#endif // SIMDEE_UTIL_INTEGRAL_HPP
