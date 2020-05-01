#include "catch.hpp"
#include <simdee/simdee.hpp>

using B = sd::vec4b;
using F = sd::vec4f;
using U = sd::vec4u;
using S = sd::vec4s;

#define SIMD_TYPE "vec4"
#define SIMD_TEST_TAG "[simd_vectors][vec4]"
#define SIMD_WIDTH 4

#include "simd_vector_data4.inl"

#include "simd_vector.inl"
