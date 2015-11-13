#include <simdify/containers.hpp>

#include "catch.hpp"

TEST_CASE("named_array", "[containers][iterator]") {
    simd::named_array<int, simd::id::x, simd::id::y, simd::id::z> pos;
    pos.x = 23;
    pos.y = 34;
    pos.z = 45;

    SECTION("sizeof()") {
        REQUIRE(sizeof(simd::named_array<char, simd::id::x>) == 1);
        REQUIRE(sizeof(simd::named_array<char, simd::id::a, simd::id::b>) == 2);
    }
    SECTION("operator .") {
        REQUIRE(pos.x == 23);
        REQUIRE(pos.y == 34);
        REQUIRE(pos.z == 45);
    }
    SECTION("get<>()") {
        REQUIRE(simd::get<0>(pos) == pos.x);
        REQUIRE(simd::get<1>(pos) == pos.y);
        REQUIRE(simd::get<2>(pos) == pos.z);
    }
    SECTION("swap()") {
        decltype(pos) pos2;
        pos2.x = 56;
        pos2.y = 67;
        pos2.z = 78;
        swap(pos, pos2);
        REQUIRE(pos.x == 56);
        REQUIRE(pos.y == 67);
        REQUIRE(pos.z == 78);
        REQUIRE(pos2.x == 23);
        REQUIRE(pos2.y == 34);
        REQUIRE(pos2.z == 45);
    }
}
