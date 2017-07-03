// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_VEC8_HPP
#define SIMDEE_VEC8_HPP

#include "common/init.hpp"

//
// AVX-accelerated vec8 implementation
//
#if (!SIMDEE_NEED_INT && SIMDEE_AVX) || SIMDEE_AVX2
#include "simd_vectors/avx.hpp"

namespace sd {
    using vec8b = avxb;
    using vec8f = avxf;
    using vec8u = avxu;
    using vec8s = avxs;
}

//
// Emulated vec8 implementation
//
#else
#include "vec4.hpp"
#include "simd_vectors/dual.hpp"

namespace sd {
    using vec8b = dual<vec4b>;
    using vec8f = dual<vec4f>;
    using vec8u = dual<vec4u>;
    using vec8s = dual<vec4s>;
}

#endif

#endif // SIMDEE_VEC8_HPP
