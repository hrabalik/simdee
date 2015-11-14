#include <simdify/containers.hpp>

#include "catch.hpp"

TEST_CASE("structure_of_arrays", "[containers][structure_of_arrays]") {
    simd::structure_of_arrays<simd::sse, simd::id::x, simd::id::y, simd::id::z> pos;
    decltype(pos)::iterator_vector itv(pos, 0);
    decltype(pos)::iterator it(pos, 0);
}
