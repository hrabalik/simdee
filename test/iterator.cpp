#include <simdify/containers.hpp>

#include "catch.hpp"

TEST_CASE("named_array", "[containers][iterator]") {
    SECTION("sizeof") {
        REQUIRE(sizeof(simd::named_array<char, simd::id::x>) == 1);
        REQUIRE(sizeof(simd::named_array<char, simd::id::a, simd::id::b>) == 2);
    }
    SECTION("indexing") {
        simd::named_array<int, simd::id::x, simd::id::y, simd::id::z> pos;
        pos.x = 23;
        pos.y = 34;
        pos.z = 45;
        REQUIRE(pos.x + pos.y + pos.z == 102);
    }
    SECTION("as_std_array") {
        simd::named_array<int, simd::id::x, simd::id::y, simd::id::z> pos;
        pos.x = 23;
        pos.y = 34;
        pos.z = 45;
        std::array<int, 3> pos_arr = pos.as_std_array();
        REQUIRE(pos_arr.at(0) == 45);
        REQUIRE(pos_arr.at(1) == 34);
        REQUIRE(pos_arr.at(2) == 23);
    }
}
