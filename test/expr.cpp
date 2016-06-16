#include "catch.hpp"

#include <simdify/simd_types.hpp>

TEST_CASE("expr::tof", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(simd::tof(0x40000000U) == 2.0f);
        REQUIRE(simd::tof(0x4000000000000000ULL) == 2.0);
        REQUIRE(simd::tof(0xdeadbeefU) == -6.2598534e18f);
        REQUIRE(simd::tof(0xdeadbeefdeadbeefULL) == -1.1885959257070704e+148);
        REQUIRE(simd::tof(-0x40000000) == -2.0f);
        REQUIRE(simd::tof(-0x4000000000000000LL) == -2.0);
    }
    SECTION("lvalues") {
        unsigned int u = 0x40000000U;
        unsigned long long ul = 0x4000000000000000ULL;
        int s = -0x40000000;
        long long sl = -0x4000000000000000LL;
        REQUIRE(simd::tof(u) == 2.0f);
        REQUIRE(simd::tof(ul) == 2.0);
        REQUIRE(simd::tof(s) == -2.0f);
        REQUIRE(simd::tof(sl) == -2.0);
    }
}

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
    SECTION("float") {
        unsigned int u = 0;
        u = simd::fval(2.f).to<unsigned int>();
        REQUIRE(u == 0x40000000U);
        u = simd::fval(-6.2598534e18f).to<unsigned int>();
        REQUIRE(u == 0xdeadbeefU);

        int s = 0;
        s = simd::fval(-2.f).to<int>();
        REQUIRE(s == -0x40000000);
    }

    SECTION("double") {
        unsigned long long u = 0;
        u = simd::fval(2.).to<unsigned long long>();
        REQUIRE(u == 0x4000000000000000ULL);
        u = simd::fval(-1.1885959257070704e+148).to<unsigned long long>();
        REQUIRE(u == 0xdeadbeefdeadbeefULL);

        long long s = 0;
        s = simd::fval(-2.).to<long long>();
        REQUIRE(s == -0x4000000000000000LL);
    }
}

TEST_CASE("expr::uval", "[expr]") {
    SECTION("float") {
        float f = 0;
        f = simd::uval(0xdeadbeefU).to<float>();
        REQUIRE(simd::tou(f) == 0xdeadbeefU);
        f = simd::uval(std::numeric_limits<uint32_t>::min()).to<float>();
        REQUIRE(simd::tou(f) == std::numeric_limits<uint32_t>::min());
        f = simd::uval(std::numeric_limits<uint32_t>::max()).to<float>();
        REQUIRE(simd::tou(f) == std::numeric_limits<uint32_t>::max());
    }
    SECTION("double") {
        double d = 0;
        d = simd::uval(0xdeadbeefdeadbeefULL).to<double>();
        REQUIRE(simd::tou(d) == 0xdeadbeefdeadbeefULL);
        d = simd::uval(std::numeric_limits<uint64_t>::min()).to<double>();
        REQUIRE(simd::tou(d) == std::numeric_limits<uint64_t>::min());
        d = simd::uval(std::numeric_limits<uint64_t>::max()).to<double>();
        REQUIRE(simd::tou(d) == std::numeric_limits<uint64_t>::max());
    }
}

TEST_CASE("expr::sval", "[expr]") {
    SECTION("float") {
        float f = 0;
        f = simd::sval(-123456789).to<float>();
        REQUIRE(simd::tos(f) == -123456789);
        f = simd::sval(std::numeric_limits<int32_t>::min()).to<float>();
        REQUIRE(simd::tos(f) == std::numeric_limits<int32_t>::min());
        f = simd::sval(std::numeric_limits<int32_t>::max()).to<float>();
        REQUIRE(simd::tos(f) == std::numeric_limits<int32_t>::max());
    }
    SECTION("double") {
        double d = 0;
        d = simd::sval(-1234567890123LL).to<double>();
        REQUIRE(simd::tos(d) == -1234567890123LL);
        d = simd::sval(std::numeric_limits<int64_t>::min()).to<double>();
        REQUIRE(simd::tos(d) == std::numeric_limits<int64_t>::min());
        d = simd::sval(std::numeric_limits<int64_t>::max()).to<double>();
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
