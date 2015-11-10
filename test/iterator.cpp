#include <simdify/containers.hpp>

#include "catch.hpp"

TEST_CASE("id_pack", "[containers][iterator]") {
    SECTION("sizeof") {
        REQUIRE(sizeof(simd::id_pack<char>) == 1);
        REQUIRE(sizeof(simd::id_pack<char, simd::id::x>) == 1);
        REQUIRE(sizeof(simd::id_pack<char, simd::id::a, simd::id::b>) == 2);
    }
    SECTION("indexing") {
        simd::id_pack<int, simd::id::x, simd::id::y, simd::id::z> pos;
        pos.x = 23;
        pos.y = 34;
        pos.z = 45;
        REQUIRE(pos.x + pos.y + pos.z == 102);
    }
    SECTION("as_array") {
        simd::id_pack<int, simd::id::x, simd::id::y, simd::id::z> pos;
        pos.x = 23;
        pos.y = 34;
        pos.z = 45;
        std::array<int, 3> pos_arr = simd::as_array(pos);
        REQUIRE(pos_arr.at(0) == 45);
        REQUIRE(pos_arr.at(1) == 34);
        REQUIRE(pos_arr.at(2) == 23);
    }
}
