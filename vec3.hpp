#ifndef SIMDIFY_VEC3
#define SIMDIFY_VEC3

#include "simdify.hpp"
#include "util/inline.hpp"

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    template <typename T>
    struct basic_vec3 {
        using mm_t = typename T::mm_t;
        using fp_t = typename T::fp_t;
        using bitmask_t = typename T::bitmask_t;

        basic_vec3() = default;
        basic_vec3(const basic_vec3& other) = default;
        basic_vec3& operator=(const basic_vec3& other) = default;

        template <typename S>
        basic_vec3(const S& other) : x(other.x), y(other.y), z(other.z) {}

        template <typename S>
        basic_vec3& operator=(const S& other) {
            x = T(other.x); y = T(other.y); z = T(other.z);
            return *this;
        }

        T x, y, z;
    };

    using vec3 = basic_vec3<simd_t>;
}

#undef INL

#endif // SIMDIFY_VEC3