#ifndef SIMDIFY_EXPR
#define SIMDIFY_EXPR

#include "../util/inline.hpp"
#include <type_traits>

namespace simd {
    namespace expr {
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
            SIMDIFY_FORCE_INLINE constexpr explicit aligned(T* const& r) : ptr(r) {}

            template <typename Simd_t>
            SIMDIFY_FORCE_INLINE void operator=(const Simd_t& r) {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via aligned()");
                using f_t = typename Simd_t::f_t;
                r.store(reinterpret_cast<f_t*>(ptr));
            }

            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr const F_t* get_load() const {
                return reinterpret_cast<const F_t*>(ptr);
            }

            // data
            T* const& ptr;
        };

        template <typename T>
        struct unaligned {
            SIMDIFY_FORCE_INLINE constexpr explicit unaligned(T* const& r) : ptr(r) {}

            template <typename Simd_t>
            SIMDIFY_FORCE_INLINE void operator=(const Simd_t& r) {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via unaligned()");
                using f_t = typename Simd_t::f_t;
                r.store(reinterpret_cast<f_t*>(ptr));
            }

            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr const F_t* get_load() const {
                return reinterpret_cast<const F_t*>(ptr);
            }

            // data
            T* const& ptr;
        };

        template <typename Crtp>
        struct tof {
            SIMDIFY_FORCE_INLINE constexpr const Crtp& self() const { return static_cast<const Crtp&>(*this); }

            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr F_t to() const {
                return self().template to<F_t>();
            }
        };

        template <typename T>
        struct utof : tof<utof<T>> {
            SIMDIFY_FORCE_INLINE constexpr explicit utof(T&& r) : ref(std::forward<T>(r)) {}

            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "utof() must only be used to produce floating-point values");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return dirty_cast<u_t, F_t>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct stof : tof<stof<T>> {
            SIMDIFY_FORCE_INLINE constexpr explicit stof(T&& r) : ref(std::forward<T>(r)) {}

            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "stof() must only be used to produce floating-point values");
                using s_t = simd::select_sint_t<sizeof(F_t)>;
                return dirty_cast<s_t, F_t>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        struct zero : tof<zero> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "zero() must only be used to produce floating-point values");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return dirty_cast<u_t, F_t>(0);
            }
        };

        struct all_bits : tof<all_bits> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "all_bits() must only be used to produce floating-point values");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return dirty_cast<u_t, F_t>(~u_t(0));
            }
        };


        struct sign_bit : tof<sign_bit> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "sign_bit() must only be used to produce floating-point values");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return dirty_cast<u_t, F_t>(~(~u_t(0) >> 1));
            }
        };

        struct abs_mask : tof<abs_mask> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE constexpr F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "abs_mask() must only be used to produce floating-point values");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return dirty_cast<u_t, F_t>(~u_t(0) >> 1);
            }
        };

        template <typename T>
        struct tou {
            using f_t = typename std::decay<T>::type;
            using u_t = simd::select_uint_t<sizeof(f_t)>;
            static_assert(std::is_floating_point<f_t>::value, "tou() must be used to convert a floating-point value");

            SIMDIFY_FORCE_INLINE constexpr explicit tou(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_FORCE_INLINE constexpr operator u_t() const {
                return dirty_cast<f_t, u_t>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct tos {
            using f_t = typename std::decay<T>::type;
            using s_t = simd::select_sint_t<sizeof(f_t)>;
            static_assert(std::is_floating_point<f_t>::value, "tos() must be used to convert a floating-point value");

            SIMDIFY_FORCE_INLINE constexpr explicit tos(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_FORCE_INLINE constexpr operator s_t() const {
                return dirty_cast<f_t, s_t>(std::forward<T>(ref));
            }

            // data
            T&& ref;
        };
    }

    template <typename T>
    SIMDIFY_FORCE_INLINE constexpr expr::aligned<T> aligned(T* const& r) { return expr::aligned<T>(r); }
    template <typename T>
    SIMDIFY_FORCE_INLINE constexpr expr::unaligned<T> unaligned(T* const& r) { return expr::unaligned<T>(r); }
    template <typename T>
    SIMDIFY_FORCE_INLINE constexpr expr::utof<T&&> utof(T&& r) { return expr::utof<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE constexpr expr::stof<T&&> stof(T&& r) { return expr::stof<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE constexpr expr::tou<T&&> tou(T&& r) { return expr::tou<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE constexpr expr::tos<T&&> tos(T&& r) { return expr::tos<T&&>(std::forward<T>(r)); }

    SIMDIFY_FORCE_INLINE constexpr expr::zero zero() { return expr::zero{}; }
    SIMDIFY_FORCE_INLINE constexpr expr::all_bits all_bits() { return expr::all_bits{}; }
    SIMDIFY_FORCE_INLINE constexpr expr::sign_bit sign_bit() { return expr::sign_bit{}; }
    SIMDIFY_FORCE_INLINE constexpr expr::abs_mask abs_mask() { return expr::abs_mask{}; }
}

#endif // SIMDIFY_EXPR
