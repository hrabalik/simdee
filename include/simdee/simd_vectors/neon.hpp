// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_NEON_HPP
#define SIMDEE_SIMD_TYPES_NEON_HPP

#include "common.hpp"

#if !SIMDEE_NEON
#error "NEON intrinsics are required to use the NEON SIMD type. Please check your build options."
#endif

#include <arm_neon.h>

namespace sd {
    struct neonb {};
    struct neonf {};
    struct neonu {};
    struct neons {};
}

#endif // SIMDEE_SIMD_TYPES_NEON_HPP
