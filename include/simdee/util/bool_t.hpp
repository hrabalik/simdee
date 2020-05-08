// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_BOOL_T_HPP
#define SIMDEE_UTIL_BOOL_T_HPP

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
            SIMDEE_INL constexpr explicit bool_t(S value) : data(U(value)) {}
            SIMDEE_INL constexpr operator bool() const { return data != 0; }
            SIMDEE_INL constexpr explicit operator U() const { return data; }
            SIMDEE_INL constexpr explicit operator S() const { return S(data); }

            SIMDEE_INL constexpr bool_t operator!() const { return bool_t(~data); }
            SIMDEE_INL constexpr bool_t operator&&(const bool_t& r) const {
                return bool_t(data & r.data);
            }
            SIMDEE_INL constexpr bool_t operator||(const bool_t& r) const {
                return bool_t(data | r.data);
            }

            // data
            U data;
        };
    } // namespace impl

    using bool8_t = impl::bool_t<uint8_t, int8_t>;
    using bool16_t = impl::bool_t<uint16_t, int16_t>;
    using bool32_t = impl::bool_t<uint32_t, int32_t>;
    using bool64_t = impl::bool_t<uint64_t, int64_t>;

    template <typename T>
    struct is_bool_type
        : std::integral_constant<
              bool, false || std::is_same<T, bool8_t>::value || std::is_same<T, bool16_t>::value ||
                        std::is_same<T, bool32_t>::value || std::is_same<T, bool64_t>::value> {};

} // namespace sd

#endif // SIMDEE_UTIL_BOOL_T_HPP
