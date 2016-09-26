#define SIMDIFY_NEED_SSE 1
#include <simdify/simdify.hpp>

#include "catch.hpp"
#include <array>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <functional>

using F = simd::ssef;
using U = simd::sseu;
using S = simd::sses;

const simd::storage<F> bufAF {
    -0.27787193f, +0.70154146f, -2.05181630f, +2.22944568f,
};
const simd::storage<F> bufBF {
    -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
};
const simd::storage<U> bufAU {
    1753029375U, 1117080442U, 3817141237U, 3761735248U,
};
const simd::storage<U> bufBU {
    1679702461U, 2102346647U, 480083363U, 3761735248U,
};
const simd::storage<S> bufAS {
    -1712190449, -48692967, -214510247, 440646957,
};
const simd::storage<S> bufBS {
    724135231, 56848532, 64122653, 440646957,
};
const simd::storage<F> bufZF { 0, 0, 0, 0 };
const simd::storage<U> bufZU { 0, 0, 0, 0 };
const simd::storage<S> bufZS { 0, 0, 0, 0 };

#define SIMD_TYPE "SSE"
#define SIMD_TEST_TAG "[simd_types][sse]"
#define SIMD_WIDTH 4
#define SIMD_LOAD_AF _mm_load_ps(bufAF.data())
#define SIMD_LOAD_AU _mm_load_ps((float*)bufAU.data())
#define SIMD_LOAD_AS _mm_load_ps((float*)bufAS.data())

#include "simd_type_tests.hpp"
