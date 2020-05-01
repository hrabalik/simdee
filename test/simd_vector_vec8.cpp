#include "catch.hpp"
#include <simdee/simdee.hpp>

using B = sd::vec8b;
using F = sd::vec8f;
using U = sd::vec8u;
using S = sd::vec8s;

#define SIMD_TYPE "vec8"
#define SIMD_TEST_TAG "[simd_vectors][vec8]"
#define SIMD_WIDTH 8

#include "simd_vector_data8.inl"

#include "simd_vector.inl"
