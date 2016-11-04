#include "catch.hpp"
#include <simdee/simdee.hpp>
#include <simdee/simd_vectors/dual.hpp>

using B = sd::dual<sd::vec4b>;
using F = sd::dual<sd::vec4f>;
using U = sd::dual<sd::vec4u>;
using S = sd::dual<sd::vec4s>;

const B::storage_t bufAB{
    true, false, true, true,
    false, true, false, true,
};
const B::storage_t bufBB{
    false, true, false, true,
    true, false, false, false,
};
const F::storage_t bufAF{
    -0.27787193f, +0.70154146f, -2.05181630f, +2.22944568f,
    -0.82358653f, -1.57705702f, +0.50797465f, -0.59003456f,
};
const F::storage_t bufBF{
    -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
    +0.33756370f, -0.87587426f, -1.66416447f, -0.59003456f,
};
const U::storage_t bufAU{
    1753029375U, 1117080442U, 3817141237U, 3761735248U,
    244284091U, 1874242234U, 3400252368U, 2648404707U,
};
const U::storage_t bufBU{
    1679702461U, 2102346647U, 480083363U, 3761735248U,
    1213208312U, 2121108407U, 2718691794U, 3724041010U,
};
const S::storage_t bufAS{
    -1712190449, -48692967, -214510247, 440646957,
    471289320, 614985478, -2035465541, 248883859,
};
const S::storage_t bufBS{
    724135231, 56848532, 64122653, 440646957,
    -899302812, -2112882416, 77287484, 1066617619,
};
const B::storage_t bufZB{
    false, false, false, false,
    false, false, false, false,
};
const F::storage_t bufZF{ 0, 0, 0, 0, 0, 0, 0, 0 };
const U::storage_t bufZU{ 0, 0, 0, 0, 0, 0, 0, 0 };
const S::storage_t bufZS{ 0, 0, 0, 0, 0, 0, 0, 0 };

#define SIMD_TYPE "Dual"
#define SIMD_TEST_TAG "[simd_vectors][dual]"
#define SIMD_WIDTH 8

#include "simd_vector.inl"
