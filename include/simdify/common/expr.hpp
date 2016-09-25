#ifndef SIMDIFY_COMMON_EXPR_HPP
#define SIMDIFY_COMMON_EXPR_HPP

#include "../util/inline.hpp"
#include "../util/integral.hpp"
#include <type_traits>
#include <limits>
#include <utility>

namespace simd {
    template <typename T>
    struct is_simd_type;
    template <typename T>
    struct simd_type_traits;

    namespace expr {
        template <typename T>
        struct bit_not {
            SIMDIFY_INL constexpr explicit bit_not(const T& r) : neg(r) {}

            SIMDIFY_INL typename T::mask_t mask() const { return ~neg.mask(); }
            SIMDIFY_INL typename T::scalar_t first_element() const { return ~neg.first_element(); }

            SIMDIFY_INL const T reduce(typename T::binary_op_t f) {
                T pos = ~neg;
                return pos.reduce(f);
            }

            // data
            const T neg;
        };

        template<typename T>
        SIMDIFY_INL constexpr const T operator~(const bit_not<T>& l) {
            return l.neg;
        }
        template<typename T>
        SIMDIFY_INL const T operator&(const bit_not<T>& l, const T& r) {
            return andnot(r, l.neg);
        }
        template<typename T>
        SIMDIFY_INL const T operator&(const T& l, const bit_not<T>& r) {
            return andnot(l, r.neg);
        }
        template<typename T>
        SIMDIFY_INL const bit_not<T> operator&(const bit_not<T>& l, const bit_not<T>& r) {
            return bit_not<T>(l.neg | r.neg);
        }
        template<typename T>
        SIMDIFY_INL const bit_not<T> operator|(const bit_not<T>& l, const bit_not<T>& r) {
            return bit_not<T>(l.neg & r.neg);
        }
        template<typename T>
        SIMDIFY_INL const T operator^(const bit_not<T>& l, const bit_not<T>& r) {
            return l.neg ^ r.neg;
        }

        template <typename From, typename To>
        constexpr To&& dirty_cast(From&& from) {
            static_assert(std::is_trivial<From>::value, "dirty_cast(): the casted-from type isn't trivial");
            static_assert(std::is_trivial<To>::value, "dirty_cast(): the casted-to type isn't trivial");
            static_assert(sizeof(From) == sizeof(To), "dirty_cast(): the types aren't the same size");
            return std::move(*reinterpret_cast<To*>(&from));
        }

        template <typename From, typename To>
        constexpr To& dirty_cast(From& from) {
            static_assert(std::is_trivial<From>::value, "dirty_cast(): the casted-from type isn't trivial");
            static_assert(std::is_trivial<To>::value, "dirty_cast(): the casted-to type isn't trivial");
            static_assert(sizeof(From) == sizeof(To), "dirty_cast(): the types aren't the same size");
            return *reinterpret_cast<To*>(&from);
        }

        template <typename From, typename To>
        constexpr const To& dirty_cast(const From& from) {
            static_assert(std::is_trivial<From>::value, "dirty_cast(): the casted-from type isn't trivial");
            static_assert(std::is_trivial<To>::value, "dirty_cast(): the casted-to type isn't trivial");
            static_assert(sizeof(From) == sizeof(To), "dirty_cast(): the types aren't the same size");
            return *reinterpret_cast<const To*>(&from);
        }

        template <typename T>
        struct aligned {
            SIMDIFY_INL constexpr explicit aligned(T* r) : ptr(r) {}

            template <typename Simd_t>
            SIMDIFY_INL void operator=(const Simd_t& r) const {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via aligned()");
                using scalar_t = typename Simd_t::scalar_t;
                r.aligned_store(reinterpret_cast<scalar_t*>(ptr));
            }

            // data
            T* ptr;
        };

        template <typename T>
        struct unaligned {
            SIMDIFY_INL constexpr explicit unaligned(T* r) : ptr(r) {}

            template <typename Simd_t>
            SIMDIFY_INL void operator=(const Simd_t& r) const {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via unaligned()");
                using scalar_t = typename Simd_t::scalar_t;
                r.unaligned_store(reinterpret_cast<scalar_t*>(ptr));
            }

            // data
            T* ptr;
        };

        template <typename Crtp>
        struct init {
            SIMDIFY_INL constexpr const Crtp& self() const { return static_cast<const Crtp&>(*this); }

            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                static_assert(std::is_arithmetic<Target>::value, "init::to<Target>():: Target must be an arithmetic type");
                return self().template to<Target>();
            }
        };

        template <typename T>
        struct fval : init<fval<T>> {
            SIMDIFY_INL constexpr explicit fval(T&& r) : ref(std::forward<T>(r)) {}

            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using f_t = select_float_t<sizeof(Target)>;
                return dirty_cast<f_t, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct uval : init<uval<T>> {
            SIMDIFY_INL constexpr explicit uval(T&& r) : ref(std::forward<T>(r)) {}

            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty_cast<u_t, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct sval : init<sval<T>> {
            SIMDIFY_INL constexpr explicit sval(T&& r) : ref(std::forward<T>(r)) {}

            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using s_t = select_sint_t<sizeof(Target)>;
                return dirty_cast<s_t, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        struct zero : init<zero> {
            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty_cast<u_t, Target>(0);
            }
        };

        struct all_bits : init<all_bits> {
            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty_cast<u_t, Target>(~u_t(0));
            }
        };


        struct sign_bit : init<sign_bit> {
            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty_cast<u_t, Target>(~(~u_t(0) >> 1));
            }
        };

        struct abs_mask : init<abs_mask> {
            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using u_t = select_uint_t<sizeof(Target)>;
                return dirty_cast<u_t, Target>(~u_t(0) >> 1);
            }
        };

        struct inf : init<inf> {
            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using f_t = select_float_t<sizeof(Target)>;
                return dirty_cast<f_t, Target>(std::numeric_limits<f_t>::infinity());
            }
        };

        struct ninf : init<ninf> {
            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using f_t = select_float_t<sizeof(Target)>;
                return dirty_cast<f_t, Target>(-std::numeric_limits<f_t>::infinity());
            }
        };

        struct nan : init<nan> {
            template <typename Target>
            SIMDIFY_INL constexpr Target to() const {
                using f_t = select_float_t<sizeof(Target)>;
                return dirty_cast<f_t, Target>(std::numeric_limits<f_t>::quiet_NaN());
            }
        };

        template <typename T, typename Enable = void>
        struct tof;

        template <typename T>
        struct tof<T, typename std::enable_if<std::is_arithmetic<typename std::decay<T>::type>::value>::type> {
            using Source = typename std::decay<T>::type;
            using Target = select_float_t<sizeof(Source)>;

            SIMDIFY_INL constexpr explicit tof(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_INL constexpr operator Target() const {
                return dirty_cast<Source, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct tof<T, typename std::enable_if<is_simd_type<typename std::decay<T>::type>::value>::type> {
            using Source = typename std::decay<T>::type;
            using Target = typename simd_type_traits<Source>::vec_f;

            SIMDIFY_INL constexpr explicit tof(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_INL constexpr operator Target() const {
                return dirty_cast<Source, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T, typename Enable = void>
        struct tou;

        template <typename T>
        struct tou<T, typename std::enable_if<std::is_arithmetic<typename std::decay<T>::type>::value>::type> {
            using Source = typename std::decay<T>::type;
            using Target = select_uint_t<sizeof(Source)>;

            SIMDIFY_INL constexpr explicit tou(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_INL constexpr operator Target() const {
                return dirty_cast<Source, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct tou<T, typename std::enable_if<is_simd_type<typename std::decay<T>::type>::value>::type> {
            using Source = typename std::decay<T>::type;
            using Target = typename simd_type_traits<Source>::vec_u;

            SIMDIFY_INL constexpr explicit tou(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_INL constexpr operator Target() const {
                return dirty_cast<Source, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T, typename Enable = void>
        struct tos;

        template <typename T>
        struct tos<T, typename std::enable_if<std::is_arithmetic<typename std::decay<T>::type>::value>::type> {
            using Source = typename std::decay<T>::type;
            using Target = select_sint_t<sizeof(Source)>;
            static_assert(std::is_arithmetic<Source>::value, "tos() must be used to convert from an arithmetic type");

            SIMDIFY_INL constexpr explicit tos(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_INL constexpr operator Target() const {
                return dirty_cast<Source, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct tos<T, typename std::enable_if<is_simd_type<typename std::decay<T>::type>::value>::type> {
            using Source = typename std::decay<T>::type;
            using Target = typename simd_type_traits<Source>::vec_s;

            SIMDIFY_INL constexpr explicit tos(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_INL constexpr operator Target() const {
                return dirty_cast<Source, Target>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };
    }

    template <typename T>
    SIMDIFY_INL constexpr expr::aligned<T> aligned(T* const& r) { return expr::aligned<T>(r); }
    template <typename T>
    SIMDIFY_INL constexpr expr::unaligned<T> unaligned(T* const& r) { return expr::unaligned<T>(r); }
    template <typename T>
    SIMDIFY_INL constexpr expr::fval<T&&> fval(T&& r) { return expr::fval<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_INL constexpr expr::uval<T&&> uval(T&& r) { return expr::uval<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_INL constexpr expr::sval<T&&> sval(T&& r) { return expr::sval<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_INL constexpr expr::tof<T&&> tof(T&& r) { return expr::tof<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_INL constexpr expr::tou<T&&> tou(T&& r) { return expr::tou<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_INL constexpr expr::tos<T&&> tos(T&& r) { return expr::tos<T&&>(std::forward<T>(r)); }

    SIMDIFY_INL constexpr expr::zero zero() { return expr::zero{}; }
    SIMDIFY_INL constexpr expr::all_bits all_bits() { return expr::all_bits{}; }
    SIMDIFY_INL constexpr expr::sign_bit sign_bit() { return expr::sign_bit{}; }
    SIMDIFY_INL constexpr expr::abs_mask abs_mask() { return expr::abs_mask{}; }
    SIMDIFY_INL constexpr expr::inf inf() { return expr::inf{}; }
    SIMDIFY_INL constexpr expr::ninf ninf() { return expr::ninf{}; }
    SIMDIFY_INL constexpr expr::nan nan() { return expr::nan{}; }
}

#endif // SIMDIFY_COMMON_EXPR_HPP
