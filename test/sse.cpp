#define SIMDIFY_NEED_SSE 1
#include <simdify/simd_types.hpp>
#include <simdify/storage.hpp>

#include "catch.hpp"
#include <array>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <functional>

using F = simd::ssef;
using U = simd::sseu;
using S = simd::sses;

alignas(F) const F::array_e bufAF = { {
        -0.27787193f, +0.70154146f, -2.05181630f, +2.22944568f,
    } };
alignas(F) const F::array_e bufBF = { {
        -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
    } };
alignas(U) const U::array_e bufAU = { {
        1753029375U, 1117080442U, 3817141237U, 3761735248U,
    } };
alignas(U) const U::array_e bufBU = { {
        1679702461U, 2102346647U, 480083363U, 779803484U,
    } };
alignas(S) const S::array_e bufAS = { {
        -1712190449, -48692967, -214510247, 440646957,
    } };
alignas(S) const S::array_e bufBS = { {
        724135231, 56848532, 64122653, 1641874147,
    } };
alignas(F) const F::array_e bufZF = { { 0, 0, 0, 0 } };
alignas(U) const U::array_e bufZU = { { 0, 0, 0, 0 } };
alignas(S) const S::array_e bufZS = { { 0, 0, 0, 0 } };

#define SIMD_TYPE "SSE"
#define SIMD_TEST_TAG "[simd_types][sse]"
#define SIMD_WIDTH 4
#define SIMD_LOAD_AF _mm_load_ps(bufAF.data())
#define SIMD_LOAD_AU _mm_load_ps((float*)bufAU.data())
#define SIMD_LOAD_AS _mm_load_ps((float*)bufAS.data())

#include "simd_type_tests.hpp"
