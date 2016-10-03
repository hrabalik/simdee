#include "catch.hpp"

#include <simdee/common/expr.hpp>

TEST_CASE("dirty::expr::tof", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::tof(0x40000000U) == 2.0f);
        REQUIRE(sd::dirty::tof(0x4000000000000000ULL) == 2.0);
        REQUIRE(sd::dirty::tof(0xdeadbeefU) == -6.2598534e18f);
        REQUIRE(sd::dirty::tof(0xdeadbeefdeadbeefULL) == -1.1885959257070704e+148);
        REQUIRE(sd::dirty::tof(-0x40000000) == -2.0f);
        REQUIRE(sd::dirty::tof(-0x4000000000000000LL) == -2.0);
    }
    SECTION("lvalues") {
        unsigned int u = 0x40000000U;
        unsigned long long ul = 0x4000000000000000ULL;
        int s = -0x40000000;
        long long sl = -0x4000000000000000LL;
        REQUIRE(sd::dirty::tof(u) == 2.0f);
        REQUIRE(sd::dirty::tof(ul) == 2.0);
        REQUIRE(sd::dirty::tof(s) == -2.0f);
        REQUIRE(sd::dirty::tof(sl) == -2.0);
    }
}

TEST_CASE("dirty::expr::tou", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::tou(2.0f) == 0x40000000U);
        REQUIRE(sd::dirty::tou(2.0) == 0x4000000000000000ULL);
        REQUIRE(sd::dirty::tou(-6.2598534e18f) == 0xdeadbeefU);
        REQUIRE(sd::dirty::tou(-1.1885959257070704e+148) == 0xdeadbeefdeadbeefULL);
        REQUIRE(sd::dirty::tou(sd::bool8_t::F) == 0);
        REQUIRE(sd::dirty::tou(sd::bool8_t::T) == uint8_t(-1));
        REQUIRE(sd::dirty::tou(sd::bool16_t::T) == uint16_t(-1));
        REQUIRE(sd::dirty::tou(sd::bool32_t::T) == uint32_t(-1));
        REQUIRE(sd::dirty::tou(sd::bool64_t::T) == uint64_t(-1));
    }
    SECTION("lvalues") {
        float f = 2;
        double d = 2;
        REQUIRE(sd::dirty::tou(f) == 0x40000000U);
        REQUIRE(sd::dirty::tou(d) == 0x4000000000000000ULL);
    }
}

TEST_CASE("dirty::expr::tos", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::tos(-2.0f) == -0x40000000);
        REQUIRE(sd::dirty::tos(-2.0) == -0x4000000000000000LL);
    }
    SECTION("lvalues") {
        float f = -2;
        double d = -2;
        REQUIRE(sd::dirty::tos(f) == -0x40000000);
        REQUIRE(sd::dirty::tos(d) == -0x4000000000000000LL);
    }
}

TEST_CASE("expr::zero", "[expr]") {
    float f = sd::zero().to<float>();
    double d = sd::zero().to<double>();
    REQUIRE(f == 0);
    REQUIRE(d == 0);
}

TEST_CASE("expr::all_bits", "[expr]") {
    float f = sd::all_bits().to<float>();
    double d = sd::all_bits().to<double>();
    REQUIRE(sd::dirty::tou(f) == 0xffffffffU);
    REQUIRE(sd::dirty::tou(d) == 0xffffffffffffffffULL);
}

TEST_CASE("expr::sign_bit", "[expr]") {
    float f = sd::sign_bit().to<float>();
    double d = sd::sign_bit().to<double>();
    REQUIRE(sd::dirty::tou(f) == 0x80000000U);
    REQUIRE(sd::dirty::tou(d) == 0x8000000000000000ULL);
}

TEST_CASE("expr::abs_mask", "[expr]") {
    float f = sd::abs_mask().to<float>();
    double d = sd::abs_mask().to<double>();
    REQUIRE(sd::dirty::tou(f) == 0x7fffffffU);
    REQUIRE(sd::dirty::tou(d) == 0x7fffffffffffffffULL);
}

TEST_CASE("expr::inf", "[expr]") {
    float f = sd::inf().to<float>();
    double d = sd::inf().to<double>();
    REQUIRE(f == std::numeric_limits<float>::infinity());
    REQUIRE(d == std::numeric_limits<double>::infinity());
}

TEST_CASE("expr::ninf", "[expr]") {
    float f = sd::ninf().to<float>();
    double d = sd::ninf().to<double>();
    REQUIRE(f == -std::numeric_limits<float>::infinity());
    REQUIRE(d == -std::numeric_limits<double>::infinity());
}

TEST_CASE("expr::nan", "[expr]") {
    float f = sd::nan().to<float>();
    double d = sd::nan().to<double>();
    REQUIRE(std::isnan(f));
    REQUIRE(std::isnan(d));
}
