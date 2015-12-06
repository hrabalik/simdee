#include <simdify/impl/named_array.hpp>

#include "catch.hpp"
#include <tuple>

TEST_CASE("named_array basic guarantees", "[containers][named_array]") {
    SECTION("sizeof()") {
        REQUIRE(sizeof(simd::named_array<char, simd::id::x>) == 1);
        REQUIRE(sizeof(simd::named_array<char, simd::id::a, simd::id::b>) == 2);
    }
}

using T = simd::named_array<int, simd::id::x, simd::id::y, simd::id::z>;

TEST_CASE("named_array construction", "[containers][named_array]") {
    T pos(std::forward_as_tuple(11, 22, 33));

    SECTION("from tuple") {
        REQUIRE(pos.x == 11);
        REQUIRE(pos.y == 22);
        REQUIRE(pos.z == 33);
    }
    SECTION("copy") {
        T pos2(pos);
        REQUIRE(pos2.x == 11);
        REQUIRE(pos2.y == 22);
        REQUIRE(pos2.z == 33);
    }
}

TEST_CASE("named_array data manipulation", "[containers][named_array]") {
    T pos;
    pos.x = 23;
    pos.y = 34;
    pos.z = 45;

    SECTION("operator .") {
        REQUIRE(pos.x == 23);
        REQUIRE(pos.y == 34);
        REQUIRE(pos.z == 45);
    }
    SECTION("operator []") {
        REQUIRE(pos[0] == pos.x);
        REQUIRE(pos[1] == pos.y);
        REQUIRE(pos[2] == pos.z);
    }
    SECTION("get()") {
        REQUIRE(pos.get() == pos.x);
        REQUIRE(pos.get<0>() == pos.x);
        REQUIRE(pos.get<1>() == pos.y);
        REQUIRE(pos.get<2>() == pos.z);
        REQUIRE(simd::get<0>(pos) == pos.x);
        REQUIRE(simd::get<1>(pos) == pos.y);
        REQUIRE(simd::get<2>(pos) == pos.z);
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
