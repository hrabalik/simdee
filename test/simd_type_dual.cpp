#define SIMDEE_NEED_SSE 1
#include <simdee/simdee.hpp>
#include <simdee/simd_types/dual.hpp>

#include "catch.hpp"
#include <array>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <functional>

using B = sd::dual<sd::sseb>;
using F = sd::dual<sd::ssef>;
using U = sd::dual<sd::sseu>;
using S = sd::dual<sd::sses>;

const B::storage_t bufAB{
    B::scalar_t::T, B::scalar_t::F, B::scalar_t::T, B::scalar_t::T,
    B::scalar_t::F, B::scalar_t::T, B::scalar_t::F, B::scalar_t::T,
};
const B::storage_t bufBB{
    B::scalar_t::F, B::scalar_t::T, B::scalar_t::F, B::scalar_t::T,
    B::scalar_t::T, B::scalar_t::F, B::scalar_t::F, B::scalar_t::F,
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
    B::scalar_t::F, B::scalar_t::F, B::scalar_t::F, B::scalar_t::F,
    B::scalar_t::F, B::scalar_t::F, B::scalar_t::F, B::scalar_t::F,
};
const F::storage_t bufZF{ 0, 0, 0, 0, 0, 0, 0, 0 };
const U::storage_t bufZU{ 0, 0, 0, 0, 0, 0, 0, 0 };
const S::storage_t bufZS{ 0, 0, 0, 0, 0, 0, 0, 0 };

#define SIMD_TYPE "Dual"
#define SIMD_TEST_TAG "[simd_types][dual]"
#define SIMD_WIDTH 8
#define SIMD_LOAD_AB sd::impl::pair<sd::sseb>{_mm_load_ps((float*)bufAB.data()), _mm_load_ps((float*)bufAB.data() + 4)}
#define SIMD_LOAD_AF sd::impl::pair<sd::ssef>{_mm_load_ps((float*)bufAF.data()), _mm_load_ps((float*)bufAF.data() + 4)}
#define SIMD_LOAD_AU sd::impl::pair<sd::sseu>{_mm_load_ps((float*)bufAU.data()), _mm_load_ps((float*)bufAU.data() + 4)}
#define SIMD_LOAD_AS sd::impl::pair<sd::sses>{_mm_load_ps((float*)bufAS.data()), _mm_load_ps((float*)bufAS.data() + 4)}

#include "simd_type.inl"
