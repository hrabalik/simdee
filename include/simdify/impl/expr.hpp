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

        template <typename T>
        struct uint {
            SIMDIFY_FORCE_INLINE explicit uint(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_FORCE_INLINE explicit operator float() {
                using u_t = simd::select_uint_t<sizeof(float)>;
                return reinterpret_cast<const float&>(static_cast<u_t>(ref));
            }

            SIMDIFY_FORCE_INLINE explicit operator double() {
                using u_t = simd::select_uint_t<sizeof(double)>;
                return reinterpret_cast<const double&>(static_cast<u_t>(ref));
            }

            // data
            T&& ref;
        };

        template <typename T>
        struct sint {
            SIMDIFY_FORCE_INLINE explicit sint(T&& r) : ref(std::forward<T>(r)) {}

            SIMDIFY_FORCE_INLINE explicit operator float() {
                using u_t = simd::select_sint_t<sizeof(float)>;
                return reinterpret_cast<const float&>(static_cast<u_t>(ref));
            }

            SIMDIFY_FORCE_INLINE explicit operator double() {
                using u_t = simd::select_sint_t<sizeof(double)>;
                return reinterpret_cast<const double&>(static_cast<u_t>(ref));
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
    SIMDIFY_FORCE_INLINE expr::uint<T&&> uint(T&& r) { return expr::uint<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE float uintf(T&& r) { return float(expr::uint<T&&>(std::forward<T>(r))); }
    template <typename T>
    SIMDIFY_FORCE_INLINE double uintd(T&& r) { return double(expr::uint<T&&>(std::forward<T>(r))); }
    template <typename T>
    SIMDIFY_FORCE_INLINE expr::sint<T&&> sint(T&& r) { return expr::sint<T&&>(std::forward<T>(r)); }
    template <typename T>
    SIMDIFY_FORCE_INLINE float sintf(T&& r) { return float(expr::sint<T&&>(std::forward<T>(r))); }
    template <typename T>
    SIMDIFY_FORCE_INLINE double sintd(T&& r) { return double(expr::sint<T&&>(std::forward<T>(r))); }
}

#endif // SIMDIFY_EXPR
