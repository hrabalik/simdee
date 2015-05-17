#ifndef SIMDIFY_VEC3
#define SIMDIFY_VEC3

#include "simdify.hpp"

namespace simd {
    template <typename T>
    struct basic_vec3;
}

#include "impl/vec3_general.hpp"
#include "impl/vec3_flt.hpp"

namespace simd {
    using vec3 = basic_vec3<simd_t>;
}

#endif // SIMDIFY_VEC3