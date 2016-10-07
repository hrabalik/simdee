// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_VEC4_HPP
#define SIMDEE_VEC4_HPP

#include "common/init.hpp"

//
// SSE-accelerated vec4 implementation
//
#if SIMDEE_NEED_SSE || defined(__SSE2__)
#include "simd_types/sse.hpp"

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
#include "simd_types/dum.hpp"
#include "simd_types/dual.hpp"

namespace sd {
    using vec4b = dual<dual<dumb>>;
    using vec4f = dual<dual<dumf>>;
    using vec4u = dual<dual<dumu>>;
    using vec4s = dual<dual<dums>>;
}

#endif

#endif // SIMDEE_VEC4_HPP
