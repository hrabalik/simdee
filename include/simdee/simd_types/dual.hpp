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

    template <typename Crtp>
    struct dual_base : simd_base<Crtp> {
    protected:
        using T = typename simd_base<Crtp>::vector_t::first_type;
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using element_t = typename simd_base<Crtp>::element_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using binary_op_t = typename simd_base<Crtp>::binary_op_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(dual_base);

        SIMDEE_BASE_CTOR(dual_base, vector_t, mm = r);
        SIMDEE_BASE_CTOR(dual_base, scalar_t, mm.first = r; mm.second = r);
        SIMDEE_BASE_CTOR_TPL(dual_base, expr::aligned<T>, aligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(dual_base, expr::unaligned<T>, unaligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(dual_base, expr::init<T>, mm.first = r; mm.second = r);
        SIMDEE_BASE_CTOR(dual_base, storage_t, aligned_load(r.data()));

        SIMDEE_INL void aligned_load(const scalar_t* r) {
            first.aligned_load(r);
            second.aligned_load(r + T::width);
        }

        SIMDEE_INL void aligned_store(scalar_t* r) const {
            first.aligned_store(r);
            second.aligned_store(r + T::width);
        }

        SIMDEE_INL void unaligned_load(const scalar_t* r) {
            first.unaligned_load(r);
            second.unaligned_load(r + T::width);
        }

        SIMDEE_INL void unaligned_store(scalar_t* r) const {
            first.unaligned_store(r);
            second.unaligned_store(r + T::width);
        }

        SIMDEE_INL void interleaved_load(const scalar_t* r, std::size_t step) {
            first.interleaved_load(r);
            second.interleaved_load(r + (T::width * step));
        }

        SIMDEE_INL void interleaved_store(scalar_t* r, std::size_t step) const {
            first.interleaved_store(r);
            second.interleaved_store(r + (T::width * step));
        }

        // missing: reduce
    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_b>::value>::type> : dual_base<dual<T>> {

    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_f>::value>::type> : dual_base<dual<T>> {

    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_u>::value>::type> : dual_base<dual<T>> {

    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_s>::value>::type> : dual_base<dual<T>> {

    };
}

#endif // SIMDEE_SIMD_TYPES_DUAL_HPP
