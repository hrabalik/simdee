#include "catch.hpp"

#include <simdify/simdify.hpp>

TEST_CASE("expr::tou", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(simd::tou(2.0f) == 0x40000000U);
        REQUIRE(simd::tou(2.0) == 0x4000000000000000ULL);
        REQUIRE(simd::tou(-6.2598534e18f) == 0xdeadbeefU);
        REQUIRE(simd::tou(-1.1885959257070704e+148) == 0xdeadbeefdeadbeefULL);
    }
    SECTION("lvalues") {
        float f = 2;
        double d = 2;
        REQUIRE(simd::tou(f) == 0x40000000U);
        REQUIRE(simd::tou(d) == 0x4000000000000000ULL);
    }
}

TEST_CASE("expr::tos", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(simd::tos(-2.0f) == -0x40000000);
        REQUIRE(simd::tos(-2.0) == -0x4000000000000000LL);
    }
    SECTION("lvalues") {
        float f = -2;
        double d = -2;
        REQUIRE(simd::tos(f) == -0x40000000);
        REQUIRE(simd::tos(d) == -0x4000000000000000LL);
    }
}

TEST_CASE("expr::fval", "[expr]") {
    float f = simd::fval(-1.23456789f).to<float>();
    double d = simd::fval(-1.23456789).to<double>();
    float z = simd::fval(0).to<float>();
    REQUIRE(f == -1.23456789f);
    REQUIRE(d == -1.23456789);
    REQUIRE(z == 0);
}

TEST_CASE("expr::utof", "[expr]") {
    SECTION("float") {
        float f = 0;
        f = simd::utof(0xdeadbeefU).to<float>();
        REQUIRE(simd::tou(f) == 0xdeadbeefU);
        f = simd::utof(std::numeric_limits<uint32_t>::min()).to<float>();
        REQUIRE(simd::tou(f) == std::numeric_limits<uint32_t>::min());
        f = simd::utof(std::numeric_limits<uint32_t>::max()).to<float>();
        REQUIRE(simd::tou(f) == std::numeric_limits<uint32_t>::max());
    }
    SECTION("double") {
        double d = 0;
        d = simd::utof(0xdeadbeefdeadbeefULL).to<double>();
        REQUIRE(simd::tou(d) == 0xdeadbeefdeadbeefULL);
        d = simd::utof(std::numeric_limits<uint64_t>::min()).to<double>();
        REQUIRE(simd::tou(d) == std::numeric_limits<uint64_t>::min());
        d = simd::utof(std::numeric_limits<uint64_t>::max()).to<double>();
        REQUIRE(simd::tou(d) == std::numeric_limits<uint64_t>::max());
    }
}

TEST_CASE("expr::stof", "[expr]") {
    SECTION("float") {
        float f = 0;
        f = simd::stof(-123456789).to<float>();
        REQUIRE(simd::tos(f) == -123456789);
        f = simd::stof(std::numeric_limits<int32_t>::min()).to<float>();
        REQUIRE(simd::tos(f) == std::numeric_limits<int32_t>::min());
        f = simd::stof(std::numeric_limits<int32_t>::max()).to<float>();
        REQUIRE(simd::tos(f) == std::numeric_limits<int32_t>::max());
    }
    SECTION("double") {
        double d = 0;
        d = simd::stof(-1234567890123LL).to<double>();
        REQUIRE(simd::tos(d) == -1234567890123LL);
        d = simd::stof(std::numeric_limits<int64_t>::min()).to<double>();
        REQUIRE(simd::tos(d) == std::numeric_limits<int64_t>::min());
        d = simd::stof(std::numeric_limits<int64_t>::max()).to<double>();
        REQUIRE(simd::tos(d) == std::numeric_limits<int64_t>::max());
    }
}

TEST_CASE("expr::zero", "[expr]") {
    float f = simd::zero().to<float>();
    double d = simd::zero().to<double>();
    REQUIRE(f == 0);
    REQUIRE(d == 0);
}

TEST_CASE("expr::all_bits", "[expr]") {
    float f = simd::all_bits().to<float>();
    double d = simd::all_bits().to<double>();
    REQUIRE(simd::tou(f) == 0xffffffffU);
    REQUIRE(simd::tou(d) == 0xffffffffffffffffULL);
}

TEST_CASE("expr::sign_bit", "[expr]") {
    float f = simd::sign_bit().to<float>();
    double d = simd::sign_bit().to<double>();
    REQUIRE(simd::tou(f) == 0x80000000U);
    REQUIRE(simd::tou(d) == 0x8000000000000000ULL);
}

TEST_CASE("expr::abs_mask", "[expr]") {
    float f = simd::abs_mask().to<float>();
    double d = simd::abs_mask().to<double>();
    REQUIRE(simd::tou(f) == 0x7fffffffU);
    REQUIRE(simd::tou(d) == 0x7fffffffffffffffULL);
}

TEST_CASE("expr::inf", "[expr]") {
    float f = simd::inf().to<float>();
    double d = simd::inf().to<double>();
    REQUIRE(f == std::numeric_limits<float>::infinity());
    REQUIRE(d == std::numeric_limits<double>::infinity());
}

TEST_CASE("expr::ninf", "[expr]") {
    float f = simd::ninf().to<float>();
    double d = simd::ninf().to<double>();
    REQUIRE(f == -std::numeric_limits<float>::infinity());
    REQUIRE(d == -std::numeric_limits<double>::infinity());
}

TEST_CASE("expr::nan", "[expr]") {
    float f = simd::nan().to<float>();
    double d = simd::nan().to<double>();
    REQUIRE(std::isnan(f));
    REQUIRE(std::isnan(d));
}
