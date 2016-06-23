#define SIMDIFY_NEED_DUM 1
#include <simdify/simd_types.hpp>
#include <simdify/storage.hpp>

#include "catch.hpp"
#include <array>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <functional>

using F = simd::dumf;
using U = simd::dumu;
using S = simd::dums;

alignas(F) const F::array_t bufAF = { {
        -0.27787193f,
    } };
alignas(F) const F::array_t bufBF = { {
        -0.23645458f,
    } };
alignas(U) const U::array_t bufAU = { {
        1753029375U,
    } };
alignas(U) const U::array_t bufBU = { {
        1679702461U,
    } };
alignas(S) const S::array_t bufAS = { {
        -1712190449,
    } };
alignas(S) const S::array_t bufBS = { {
        724135231,
    } };
alignas(F) const F::array_t bufZF = { { 0 } };
alignas(U) const U::array_t bufZU = { { 0 } };
alignas(S) const S::array_t bufZS = { { 0 } };

#define SIMD_TYPE "DUM"
#define SIMD_TEST_TAG "[simd_types][dum]"
#define SIMD_WIDTH 1
#define SIMD_LOAD_AF *bufAF.data()
#define SIMD_LOAD_AU *bufAU.data()
#define SIMD_LOAD_AS *bufAS.data()

#include "simd_type_tests.hpp"
