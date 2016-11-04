#include "catch.hpp"
#include <simdee/simdee.hpp>
#include <simdee/simd_vectors/dum.hpp>

using B = sd::dumb;
using F = sd::dumf;
using U = sd::dumu;
using S = sd::dums;

const B::storage_t bufAB{
    true,
};
const B::storage_t bufBB{
    false,
};
const F::storage_t bufAF {
    -0.27787193f,
};
const F::storage_t bufBF {
    -0.23645458f,
};
const U::storage_t bufAU {
    1753029375U,
};
const U::storage_t bufBU {
    1679702461U,
};
const S::storage_t bufAS {
    -1712190449,
};
const S::storage_t bufBS {
    724135231,
};
const B::storage_t bufZB{
    false,
};
const F::storage_t bufZF { 0 };
const U::storage_t bufZU { 0 };
const S::storage_t bufZS { 0 };

#define SIMD_TYPE "DUM"
#define SIMD_TEST_TAG "[simd_vectors][dum]"
#define SIMD_WIDTH 1

#include "simd_vector.inl"
