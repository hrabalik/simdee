// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_EXPR_HPP
#define SIMDEE_COMMON_EXPR_HPP

#include "../util/inline.hpp"
#include "../util/integral.hpp"
#include <type_traits>
#include <limits>
#include <utility>

namespace sd {
    template <typename T>
    struct is_simd_type;
    template <typename T>
    struct simd_type_traits;

    namespace impl {
        template <typename T>
        struct util_f {
            using source_t = typename std::decay<T>::type;
            using target_t = select_float_t<sizeof(source_t)>;
            static_assert(is_extended_arithmetic_type<source_t>::value, "as_f(): source isn't arithmetic");
        };

        template <typename T>
        struct util_u {
            using source_t = typename std::decay<T>::type;
            using target_t = select_uint_t<sizeof(source_t)>;
            static_assert(is_extended_arithmetic_type<source_t>::value, "as_u(): source isn't arithmetic");
        };

        template <typename T>
        struct util_s {
            using source_t = typename std::decay<T>::type;
            using target_t = select_sint_t<sizeof(source_t)>;
            static_assert(is_extended_arithmetic_type<source_t>::value, "as_s(): source isn't arithmetic");
        };
    }

    namespace dirty {
        template <typename From, typename To>
        constexpr To&& cast(From&& from) {
            static_assert(std::is_trivial<From>::value, "dirty::cast(): the casted-from type isn't trivial");
            static_assert(std::is_trivial<To>::value, "dirty::cast(): the casted-to type isn't trivial");
            static_assert(sizeof(From) == sizeof(To), "dirty::cast(): the types aren't the same size");
            return std::move(*reinterpret_cast<To*>(&from));
        }

        template <typename From, typename To>
        constexpr To& cast(From& from) {
            static_assert(std::is_trivial<From>::value, "dirty::cast(): the casted-from type isn't trivial");
            static_assert(std::is_trivial<To>::value, "dirty::cast(): the casted-to type isn't trivial");
            static_assert(sizeof(From) == sizeof(To), "dirty::cast(): the types aren't the same size");
            return *reinterpret_cast<To*>(&from);
        }

        template <typename From, typename To>
        constexpr const To& cast(const From& from) {
            static_assert(std::is_trivial<From>::value, "dirty::cast(): the casted-from type isn't trivial");
            static_assert(std::is_trivial<To>::value, "dirty::cast(): the casted-to type isn't trivial");
            static_assert(sizeof(From) == sizeof(To), "dirty::cast(): the types aren't the same size");
            return *reinterpret_cast<const To*>(&from);
        }

        template <typename T>
        SIMDEE_INL constexpr typename impl::util_f<T>::target_t as_f(T r) {
            return dirty::cast<typename impl::util_f<T>::source_t, typename impl::util_f<T>::target_t>(r);
        }
        template <typename T>
        SIMDEE_INL constexpr typename impl::util_u<T>::target_t as_u(T r) {
            return dirty::cast<typename impl::util_u<T>::source_t, typename impl::util_u<T>::target_t>(r);
        }
        template <typename T>
        SIMDEE_INL constexpr typename impl::util_s<T>::target_t as_s(T r) {
            return dirty::cast<typename impl::util_s<T>::source_t, typename impl::util_s<T>::target_t>(r);
        }
    }

    namespace expr {
        template <typename T>
        struct aligned {
            SIMDEE_INL constexpr explicit aligned(T* r) : ptr(r) {}

            template <typename Simd_t>
            SIMDEE_INL void operator=(const Simd_t& r) const {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via aligned()");
                using scalar_t = typename Simd_t::scalar_t;
                r.aligned_store(reinterpret_cast<scalar_t*>(ptr));
            }

            // data
            T* ptr;
        };

        template <typename T>
        struct unaligned {
            SIMDEE_INL constexpr explicit unaligned(T* r) : ptr(r) {}

            template <typename Simd_t>
            SIMDEE_INL void operator=(const Simd_t& r) const {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via unaligned()");
                using scalar_t = typename Simd_t::scalar_t;
                r.unaligned_store(reinterpret_cast<scalar_t*>(ptr));
            }

            // data
            T* ptr;
        };

        template <typename Crtp>
        struct init {
            SIMDEE_INL constexpr const Crtp& self() const { return static_cast<const Crtp&>(*this); }

            template <typename Target>
            SIMDEE_INL constexpr Target to() const {
                static_assert(is_extended_arithmetic_type<Target>::value, "init::to<Target>():: Target must be an arithmetic type");
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
    SIMDEE_INL constexpr expr::aligned<T> aligned(T* const& r) { return expr::aligned<T>(r); }
    template <typename T>
    SIMDEE_INL constexpr expr::unaligned<T> unaligned(T* const& r) { return expr::unaligned<T>(r); }

    SIMDEE_INL constexpr expr::zero zero() { return expr::zero{}; }
    SIMDEE_INL constexpr expr::all_bits all_bits() { return expr::all_bits{}; }
    SIMDEE_INL constexpr expr::sign_bit sign_bit() { return expr::sign_bit{}; }
    SIMDEE_INL constexpr expr::abs_mask abs_mask() { return expr::abs_mask{}; }
    SIMDEE_INL constexpr expr::inf inf() { return expr::inf{}; }
    SIMDEE_INL constexpr expr::ninf ninf() { return expr::ninf{}; }
    SIMDEE_INL constexpr expr::nan nan() { return expr::nan{}; }
}

#endif // SIMDEE_COMMON_EXPR_HPP
