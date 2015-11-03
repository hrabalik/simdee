#ifndef SIMDIFY_EXPR
#define SIMDIFY_EXPR

#include "../util/inline.hpp"
#include <type_traits>

namespace simd {
    namespace expr {
        template <typename T>
        struct aligned {
            SIMDIFY_FORCE_INLINE explicit aligned(T* const& r) : ptr(r) {}

            template <typename Simd_t>
            SIMDIFY_FORCE_INLINE void operator=(const Simd_t& r) {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via aligned()");
                using f_t = typename Simd_t::f_t;
                r.store(reinterpret_cast<f_t*>(ptr));
            }

            template <typename F_t>
            SIMDIFY_FORCE_INLINE const F_t* get_load() const {
                return reinterpret_cast<const F_t*>(ptr);
            }

            // data
            T* const& ptr;
        };

        template <typename T>
        struct unaligned {
            SIMDIFY_FORCE_INLINE explicit unaligned(T* const& r) : ptr(r) {}

            template <typename Simd_t>
            SIMDIFY_FORCE_INLINE void operator=(const Simd_t& r) {
                static_assert(!std::is_const<T>::value, "Storing into a const pointer via unaligned()");
                using f_t = typename Simd_t::f_t;
                r.store(reinterpret_cast<f_t*>(ptr));
            }

            template <typename F_t>
            SIMDIFY_FORCE_INLINE const F_t* get_load() const {
                return reinterpret_cast<const F_t*>(ptr);
            }

            // data
            T* const& ptr;
        };

        template <typename Crtp>
        struct tof {
            SIMDIFY_FORCE_INLINE const Crtp& self() const { return static_cast<const Crtp&>(*this); }

            template <typename F_t>
            SIMDIFY_FORCE_INLINE F_t to() const {
                return self().to<F_t>();
            }
        };

        template <typename T>
        struct utof : tof<utof<T>> {
            SIMDIFY_FORCE_INLINE explicit utof(T&& r) : ref(std::forward<T>(r)) {}

            template <typename F_t>
            SIMDIFY_FORCE_INLINE F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "utof() must only be used to produce floating-point types");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return reinterpret_cast<const F_t&>(static_cast<const u_t&>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct stof : tof<stof<T>> {
            SIMDIFY_FORCE_INLINE explicit stof(T&& r) : ref(std::forward<T>(r)) {}

            template <typename F_t>
            SIMDIFY_FORCE_INLINE F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "stof() must only be used to produce floating-point types");
                using s_t = simd::select_sint_t<sizeof(F_t)>;
                return reinterpret_cast<const F_t&>(static_cast<const s_t&>(ref));
            }

            // data
            T&& ref;
        };

        struct zero : tof<zero> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "zero() must only be used to produce floating-point types");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return reinterpret_cast<const F_t&>(static_cast<const u_t&>(0));
            }
        };

        struct all_bits : tof<all_bits> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "all_bits() must only be used to produce floating-point types");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return reinterpret_cast<const F_t&>(static_cast<const u_t&>(~u_t(0)));
            }
        };


        struct sign_bit : tof<sign_bit> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "sign_bit() must only be used to produce floating-point types");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return reinterpret_cast<const F_t&>(static_cast<const u_t&>(~(~u_t(0) >> 1)));
            }
        };

        struct abs_mask : tof<abs_mask> {
            template <typename F_t>
            SIMDIFY_FORCE_INLINE F_t to() const {
                static_assert(std::is_floating_point<F_t>::value, "abs_mask() must only be used to produce floating-point types");
                using u_t = simd::select_uint_t<sizeof(F_t)>;
                return reinterpret_cast<const F_t&>(static_cast<const u_t&>(~u_t(0) >> 1));
            }
        };

        template <typename T>
        struct tou {
            using f_t = typename std::decay<T>::type;
            using u_t = simd::select_uint_t<sizeof(f_t)>;
            static_assert(std::is_floating_point<f_t>::value, "tou() must be used to convert a floating-point value");

            SIMDIFY_FORCE_INLINE explicit tou(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_FORCE_INLINE operator u_t() {
                return reinterpret_cast<const u_t&>(ref);
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct tos {
            using f_t = typename std::decay<T>::type;
            using s_t = simd::select_sint_t<sizeof(f_t)>;
            static_assert(std::is_floating_point<f_t>::value, "tos() must be used to convert a floating-point value");

            SIMDIFY_FORCE_INLINE explicit tos(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_FORCE_INLINE operator s_t() {
                return reinterpret_cast<const s_t&>(ref);
            }

            // data
            T&& ref;
        };
    }

    template <typename T>
    SIMDIFY_FORCE_INLINE expr::aligned<T> aligned(T* const& r) { return expr::aligned<T>(r); }
    template <typename T>
    SIMDIFY_FORCE_INLINE expr::unaligned<T> unaligned(T* const& r) { return expr::unaligned<T>(r); }
    template <typename T>
    SIMDIFY_FORCE_INLINE expr::utof<T&&> utof(T&& r) { return expr::utof<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE expr::stof<T&&> stof(T&& r) { return expr::stof<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE expr::tou<T&&> tou(T&& r) { return expr::tou<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE expr::tos<T&&> tos(T&& r) { return expr::tos<T&&>(std::forward<T>(r)); }

    SIMDIFY_FORCE_INLINE expr::zero zero() { return expr::zero{}; }
    SIMDIFY_FORCE_INLINE expr::all_bits all_bits() { return expr::all_bits{}; }
    SIMDIFY_FORCE_INLINE expr::sign_bit sign_bit() { return expr::sign_bit{}; }
    SIMDIFY_FORCE_INLINE expr::abs_mask abs_mask() { return expr::abs_mask{}; }
}

#endif // SIMDIFY_EXPR
