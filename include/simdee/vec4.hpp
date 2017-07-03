// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_VEC4_HPP
#define SIMDEE_VEC4_HPP

#include "common/init.hpp"

//
// SSE-accelerated vec4 implementation
//
#if SIMDEE_SSE2
#include "simd_vectors/sse.hpp"

namespace sd {
    using vec4b = sseb;
    using vec4f = ssef;
    using vec4u = sseu;
    using vec4s = sses;
}

//
// Emulated vec4 implementation
//
#else
#include "simd_vectors/dual.hpp"
#include "simd_vectors/dum.hpp"

namespace sd {
    using vec4b = dual<dual<dumb>>;
    using vec4f = dual<dual<dumf>>;
    using vec4u = dual<dual<dumu>>;
    using vec4s = dual<dual<dums>>;
}

#endif

#endif // SIMDEE_VEC4_HPP
