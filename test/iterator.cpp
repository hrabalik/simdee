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
    SECTION("detail::get<>()") {
        REQUIRE(simd::detail::get<0>(pos) == pos.x);
        REQUIRE(simd::detail::get<1>(pos) == pos.y);
        REQUIRE(simd::detail::get<2>(pos) == pos.z);
    }
    SECTION("operator .") {
        REQUIRE(pos.x == 23);
        REQUIRE(pos.y == 34);
        REQUIRE(pos.z == 45);
    }
    SECTION("at() -- reverse order -- unintuitive") {
        REQUIRE(pos.at(0) == pos.z);
        REQUIRE(pos.at(1) == pos.y);
        REQUIRE(pos.at(2) == pos.x);
    }
    SECTION("operator [] -- reverse order -- unintuitive") {
        REQUIRE(pos[0] == pos.z);
        REQUIRE(pos[1] == pos.y);
        REQUIRE(pos[2] == pos.x);
    }
    SECTION("get<>() -- reverse order -- unintuitive") {
        REQUIRE(std::get<0>(pos) == pos.z);
        REQUIRE(std::get<1>(pos) == pos.y);
        REQUIRE(std::get<2>(pos) == pos.x);
    }
    SECTION("swap()") {
        decltype(pos) pos2;
        pos2.x = 56;
        pos2.y = 67;
        pos2.z = 78;
        std::swap(pos, pos2);
        REQUIRE(pos.x == 56);
        REQUIRE(pos.y == 67);
        REQUIRE(pos.z == 78);
        REQUIRE(pos2.x == 23);
        REQUIRE(pos2.y == 34);
        REQUIRE(pos2.z == 45);
    }
}
