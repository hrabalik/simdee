#define SIMDIFY_NEED_DUM 1
#include <simdify/simd_types.hpp>
#include <simdify/common/storage.hpp>

#include "catch.hpp"
#include <array>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <functional>

using F = simd::dumf;
using U = simd::dumu;
using S = simd::dums;

const simd::storage<F> bufAF {
    -0.27787193f,
};
const simd::storage<F> bufBF {
    -0.23645458f,
};
const simd::storage<U> bufAU {
    1753029375U,
};
const simd::storage<U> bufBU {
    1679702461U,
};
const simd::storage<S> bufAS {
    -1712190449,
};
const simd::storage<S> bufBS {
    724135231,
};
const simd::storage<F> bufZF { 0 };
const simd::storage<U> bufZU { 0 };
const simd::storage<S> bufZS { 0 };

#define SIMD_TYPE "DUM"
#define SIMD_TEST_TAG "[simd_types][dum]"
#define SIMD_WIDTH 1
#define SIMD_LOAD_AF *bufAF.data()
#define SIMD_LOAD_AU *bufAU.data()
#define SIMD_LOAD_AS *bufAS.data()

#include "simd_type_tests.hpp"
