#include <catch2/catch.hpp>
#include <simdee/simd_vectors/dual.hpp>
#include <simdee/simdee.hpp>

using B = sd::dual<sd::vec4b>;
using F = sd::dual<sd::vec4f>;
using U = sd::dual<sd::vec4u>;
using S = sd::dual<sd::vec4s>;

#define SIMD_TYPE "Dual"
#define SIMD_TEST_TAG "[simd_vectors][dual]"
#define SIMD_WIDTH 8

#include "simd_vector_data8.inl"

#include "simd_vector.inl"
