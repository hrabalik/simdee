#include <catch2/catch.hpp>
#include <simdee/util/bit_iterator.hpp>

TEST_CASE("lsb", "[bit_iterator]") {
    REQUIRE(sd::detail::lsb(0x1) == 0);
    REQUIRE(sd::detail::lsb(0xab) == 0);
    REQUIRE(sd::detail::lsb(0x2) == 1);
    REQUIRE(sd::detail::lsb(0x6) == 1);
    REQUIRE(sd::detail::lsb(0xc) == 2);
    REQUIRE(sd::detail::lsb(0xc0) == 6);
    REQUIRE(sd::detail::lsb(0xaa000000) == 25);
    REQUIRE(sd::detail::lsb(0x80000000) == 31);
}

TEST_CASE("bit_iterator", "[bit_iterator]") {
    sd::bit_iterator i{0x80040023};
    sd::bit_iterator end{0};
    REQUIRE(i != end);
    REQUIRE(*i == 0);
    i++;
    REQUIRE(i != end);
    REQUIRE(*i == 1);
    ++i;
    REQUIRE(i != end);
    REQUIRE(*i == 5);
    i++;
    REQUIRE(i != end);
    REQUIRE(*i == 18);
    ++i;
    REQUIRE(i != end);
    REQUIRE(*i == 31);
    i++;
    REQUIRE(i == end);
}
