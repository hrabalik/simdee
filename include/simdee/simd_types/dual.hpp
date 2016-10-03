// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_DUAL_HPP
#define SIMDEE_SIMD_TYPES_DUAL_HPP

#include "common.hpp"
#include <utility>

namespace sd {
    namespace impl {
        template <typename Mask_t>
        using dual_mask_t = impl::mask<((Mask_t::all_bits + 1) * (Mask_t::all_bits + 1)) - 1>;
    }

    template <typename T, typename Enable = void>
    struct dual;

    template <typename T>
    struct is_simd_type<dual<T>> : std::integral_constant<bool, true> {};

    template <typename T>
    struct simd_type_traits<dual<T>> {
        using simd_t = dual<T>;
        using vector_t = std::pair<T, T>;
        using scalar_t = typename simd_type_traits<T>::scalar_t;
        using element_t = typename simd_type_traits<T>::element_t;
        using vec_b = dual<typename simd_type_traits<T>::vec_b>;
        using vec_f = dual<typename simd_type_traits<T>::vec_f>;
        using vec_u = dual<typename simd_type_traits<T>::vec_u>;
        using vec_s = dual<typename simd_type_traits<T>::vec_s>;
        using mask_t = impl::dual_mask_t<typename simd_type_traits<T>::mask_t>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <typename T>
    struct dual_base : simd_base<dual<T>> {

    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_b>::value>::type> {

    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_f>::value>::type> {

    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_u>::value>::type> {

    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_s>::value>::type> {

    };
}

#endif // SIMDEE_SIMD_TYPES_DUAL_HPP
