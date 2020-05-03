#include <catch2/catch.hpp>
#include <simdee/simd_vectors/dum.hpp>
#include <simdee/simdee.hpp>

using B = sd::dumb;
using F = sd::dumf;
using U = sd::dumu;
using S = sd::dums;

#define SIMD_TYPE "DUM"
#define SIMD_TEST_TAG "[simd_vectors][dum]"
#define SIMD_WIDTH 1

#include "simd_vector_data1.inl"

#include "simd_vector.inl"
