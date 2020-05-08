#include <catch2/catch.hpp>
#include <simdee/simd_vectors/dual.hpp>
#include <simdee/simd_vectors/dum.hpp>
#include <simdee/simdee.hpp>

using B = sd::dual<sd::dual<sd::dumb>>;
using F = sd::dual<sd::dual<sd::dumf>>;
using U = sd::dual<sd::dual<sd::dumu>>;
using S = sd::dual<sd::dual<sd::dums>>;

#define SIMD_TYPE "dum4"
#define SIMD_TEST_TAG "[simd_vectors][dum4]"
#define SIMD_WIDTH 4

#include "simd_vector_data4.inl"

#include "simd_vector.inl"
