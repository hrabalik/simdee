// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_EXPR_HPP
#define SIMDEE_COMMON_EXPR_HPP

#include "../common/casts.hpp"
#include <limits>

namespace sd {
    namespace expr {
        template <typename T>
        struct aligned {
            SIMDEE_INL constexpr explicit aligned(T* r) : ptr(r) {}

            template <typename Simd_t>
            SIMDEE_INL void operator=(const Simd_t& r) const {
                r.aligned_store(ptr);
            }

            // data
            T* ptr;
        };

        template <typename T>
        struct unaligned {
            SIMDEE_INL constexpr explicit unaligned(T* r) : ptr(r) {}

            template <typename Simd_t>
            SIMDEE_INL void operator=(const Simd_t& r) const {
                r.unaligned_store(ptr);
            }

            // data
            T* ptr;
        };

        template <typename Crtp>
        struct init {
            SIMDEE_INL constexpr const Crtp& self() const {
                return static_cast<const Crtp&>(*this);
            }

            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                static_assert(impl::is_extended_arithmetic_type<Target>::value,
                              "init::to<Target>():: Target must be an arithmetic type");
                return self().template to<Target>();
            }
        };

        struct zero : init<zero> {
            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty::cast<u_t, Target>(0);
            }
        };

        struct all_bits : init<all_bits> {
            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty::cast<u_t, Target>(~u_t(0));
            }
        };

        struct sign_bit : init<sign_bit> {
            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty::cast<u_t, Target>(~(~u_t(0) >> 1));
            }
        };

        struct abs_mask : init<abs_mask> {
            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty::cast<u_t, Target>(~u_t(0) >> 1);
            }
        };

        struct inf : init<inf> {
            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                using f_t = select_float_t<sizeof(Target)>;
                return dirty::cast<f_t, Target>(std::numeric_limits<f_t>::infinity());
            }
        };

        struct ninf : init<ninf> {
            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                using f_t = select_float_t<sizeof(Target)>;
                return dirty::cast<f_t, Target>(-std::numeric_limits<f_t>::infinity());
            }
        };

        struct nan : init<nan> {
            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                using f_t = select_float_t<sizeof(Target)>;
                return dirty::cast<f_t, Target>(std::numeric_limits<f_t>::quiet_NaN());
            }
        };
    }

    template <typename T>
    SIMDEE_INL constexpr expr::aligned<T> aligned(T* const& r) {
        return expr::aligned<T>(r);
    }
    template <typename T>
    SIMDEE_INL constexpr expr::unaligned<T> unaligned(T* const& r) {
        return expr::unaligned<T>(r);
    }

    SIMDEE_INL constexpr expr::zero zero() { return expr::zero{}; }
    SIMDEE_INL constexpr expr::all_bits all_bits() { return expr::all_bits{}; }
    SIMDEE_INL constexpr expr::sign_bit sign_bit() { return expr::sign_bit{}; }
    SIMDEE_INL constexpr expr::abs_mask abs_mask() { return expr::abs_mask{}; }
    SIMDEE_INL constexpr expr::inf inf() { return expr::inf{}; }
    SIMDEE_INL constexpr expr::ninf ninf() { return expr::ninf{}; }
    SIMDEE_INL constexpr expr::nan nan() { return expr::nan{}; }
}

#endif // SIMDEE_COMMON_EXPR_HPP
