#include "catch.hpp"
#include <simdee/simdee.hpp>

using B = sd::vec4b;
using F = sd::vec4f;
using U = sd::vec4u;
using S = sd::vec4s;

const B::storage_t bufAB{
    B::scalar_t::T, B::scalar_t::F, B::scalar_t::T, B::scalar_t::T,
};
const B::storage_t bufBB{
    B::scalar_t::F, B::scalar_t::T, B::scalar_t::F, B::scalar_t::T,
};
const F::storage_t bufAF {
    -0.27787193f, +0.70154146f, -2.05181630f, +2.22944568f,
};
const F::storage_t bufBF {
    -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
};
const U::storage_t bufAU {
    1753029375U, 1117080442U, 3817141237U, 3761735248U,
};
const U::storage_t bufBU {
    1679702461U, 2102346647U, 480083363U, 3761735248U,
};
const S::storage_t bufAS {
    -1712190449, -48692967, -214510247, 440646957,
};
const S::storage_t bufBS {
    724135231, 56848532, 64122653, 440646957,
};
const B::storage_t bufZB{
    B::scalar_t::F, B::scalar_t::F, B::scalar_t::F, B::scalar_t::F,
};
const F::storage_t bufZF { 0, 0, 0, 0 };
const U::storage_t bufZU { 0, 0, 0, 0 };
const S::storage_t bufZS { 0, 0, 0, 0 };

#define SIMD_TYPE "vec4"
#define SIMD_TEST_TAG "[simd_types][vec4]"
#define SIMD_WIDTH 4

#include "simd_type.inl"
