// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_CASTS_HPP
#define SIMDEE_COMMON_CASTS_HPP

#include "../util/inline.hpp"
#include "../util/select.hpp"
#include <cstring>
#include <type_traits>

namespace sd {

    namespace impl {

        template <typename T>
        struct is_extended_arithmetic_type
            : std::integral_constant<bool, false || std::is_arithmetic<T>::value ||
                                               is_bool_type<T>::value> {};

        template <typename T>
        struct util_b {
            using source_t = typename std::decay<T>::type;
            using target_t = select_bool_t<sizeof(source_t)>;
            static_assert(is_extended_arithmetic_type<source_t>::value,
                          "as_f(): source isn't arithmetic");
        };

        template <typename T>
        struct util_f {
            using source_t = typename std::decay<T>::type;
            using target_t = select_float_t<sizeof(source_t)>;
            static_assert(is_extended_arithmetic_type<source_t>::value,
                          "as_f(): source isn't arithmetic");
        };

        template <typename T>
        struct util_u {
            using source_t = typename std::decay<T>::type;
            using target_t = select_uint_t<sizeof(source_t)>;
            static_assert(is_extended_arithmetic_type<source_t>::value,
                          "as_u(): source isn't arithmetic");
        };

        template <typename T>
        struct util_s {
            using source_t = typename std::decay<T>::type;
            using target_t = select_sint_t<sizeof(source_t)>;
            static_assert(is_extended_arithmetic_type<source_t>::value,
                          "as_s(): source isn't arithmetic");
        };

    } // namespace impl

    template <typename T>
    SIMDEE_INL constexpr typename impl::util_b<T>::target_t cast_b(T r) {
        return static_cast<typename impl::util_b<T>::target_t>(r);
    }
    template <typename T>
    SIMDEE_INL constexpr typename impl::util_f<T>::target_t cast_f(T r) {
        return static_cast<typename impl::util_f<T>::target_t>(r);
    }
    template <typename T>
    SIMDEE_INL constexpr typename impl::util_u<T>::target_t cast_u(T r) {
        return static_cast<typename impl::util_u<T>::target_t>(r);
    }
    template <typename T>
    SIMDEE_INL constexpr typename impl::util_s<T>::target_t cast_s(T r) {
        return static_cast<typename impl::util_s<T>::target_t>(r);
    }

    namespace dirty {
        template <typename From, typename To>
        SIMDEE_INL To cast(const From& source) {
            static_assert(std::is_trivial<From>::value,
                          "dirty::cast(): the casted-from type isn't trivial");
            static_assert(std::is_trivial<To>::value,
                          "dirty::cast(): the casted-to type isn't trivial");
            static_assert(sizeof(From) == sizeof(To),
                          "dirty::cast(): the types aren't the same size");

            To result;
            std::memcpy(&result, &source, sizeof(From));
            return result;
        }

        template <typename T>
        SIMDEE_INL constexpr typename impl::util_b<T>::target_t as_b(T r) {
            return dirty::cast<typename impl::util_b<T>::source_t,
                               typename impl::util_b<T>::target_t>(r);
        }
        template <typename T>
        SIMDEE_INL constexpr typename impl::util_f<T>::target_t as_f(T r) {
            return dirty::cast<typename impl::util_f<T>::source_t,
                               typename impl::util_f<T>::target_t>(r);
        }
        template <typename T>
        SIMDEE_INL constexpr typename impl::util_u<T>::target_t as_u(T r) {
            return dirty::cast<typename impl::util_u<T>::source_t,
                               typename impl::util_u<T>::target_t>(r);
        }
        template <typename T>
        SIMDEE_INL constexpr typename impl::util_s<T>::target_t as_s(T r) {
            return dirty::cast<typename impl::util_s<T>::source_t,
                               typename impl::util_s<T>::target_t>(r);
        }

    } // namespace dirty

} // namespace sd

#endif // SIMDEE_COMMON_CASTS_HPP
