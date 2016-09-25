#define SIMDIFY_NEED_AVX 1
#include <simdify/simd_types.hpp>

#include "catch.hpp"
#include <array>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <functional>

using F = simd::avxf;
using U = simd::avxu;
using S = simd::avxs;

const simd::storage<F> bufAF {
    -0.27787193f, +0.70154146f, -2.05181630f, +2.22944568f,
    -0.82358653f, -1.57705702f, +0.50797465f, -0.59003456f,
};
const simd::storage<F> bufBF {
    -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
    +0.33756370f, -0.87587426f, -1.66416447f, -0.59003456f,
};
const simd::storage<U> bufAU {
    1753029375U, 1117080442U, 3817141237U, 3761735248U,
    244284091U, 1874242234U, 3400252368U, 2648404707U,
};
const simd::storage<U> bufBU {
    1679702461U, 2102346647U, 480083363U, 779803484U,
    1213208312U, 2121108407U, 2718691794U, 3724041010U,
};
const simd::storage<S> bufAS {
    -1712190449, -48692967, -214510247, 440646957,
    471289320, 614985478, -2035465541, 248883859,
};
const simd::storage<S> bufBS {
    724135231, 56848532, 64122653, 1641874147,
    -899302812, -2112882416, 77287484, 1066617619,
};
const simd::storage<F> bufZF { 0, 0, 0, 0, 0, 0, 0, 0 };
const simd::storage<U> bufZU { 0, 0, 0, 0, 0, 0, 0, 0 };
const simd::storage<S> bufZS { 0, 0, 0, 0, 0, 0, 0, 0 };

#define SIMD_TYPE "AVX"
#define SIMD_TEST_TAG "[simd_types][avx]"
#define SIMD_WIDTH 8
#define SIMD_LOAD_AF _mm256_load_ps(bufAF.data())
#define SIMD_LOAD_AU _mm256_load_ps((float*)bufAU.data())
#define SIMD_LOAD_AS _mm256_load_ps((float*)bufAS.data())

#include "simd_type_tests.hpp"
