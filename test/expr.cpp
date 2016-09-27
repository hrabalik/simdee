#include "catch.hpp"

#include <simdee/common/expr.hpp>

TEST_CASE("expr::tof", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::tof(0x40000000U) == 2.0f);
        REQUIRE(sd::tof(0x4000000000000000ULL) == 2.0);
        REQUIRE(sd::tof(0xdeadbeefU) == -6.2598534e18f);
        REQUIRE(sd::tof(0xdeadbeefdeadbeefULL) == -1.1885959257070704e+148);
        REQUIRE(sd::tof(-0x40000000) == -2.0f);
        REQUIRE(sd::tof(-0x4000000000000000LL) == -2.0);
    }
    SECTION("lvalues") {
        unsigned int u = 0x40000000U;
        unsigned long long ul = 0x4000000000000000ULL;
        int s = -0x40000000;
        long long sl = -0x4000000000000000LL;
        REQUIRE(sd::tof(u) == 2.0f);
        REQUIRE(sd::tof(ul) == 2.0);
        REQUIRE(sd::tof(s) == -2.0f);
        REQUIRE(sd::tof(sl) == -2.0);
    }
}

TEST_CASE("expr::tou", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::tou(2.0f) == 0x40000000U);
        REQUIRE(sd::tou(2.0) == 0x4000000000000000ULL);
        REQUIRE(sd::tou(-6.2598534e18f) == 0xdeadbeefU);
        REQUIRE(sd::tou(-1.1885959257070704e+148) == 0xdeadbeefdeadbeefULL);
    }
    SECTION("lvalues") {
        float f = 2;
        double d = 2;
        REQUIRE(sd::tou(f) == 0x40000000U);
        REQUIRE(sd::tou(d) == 0x4000000000000000ULL);
    }
}

TEST_CASE("expr::tos", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::tos(-2.0f) == -0x40000000);
        REQUIRE(sd::tos(-2.0) == -0x4000000000000000LL);
    }
    SECTION("lvalues") {
        float f = -2;
        double d = -2;
        REQUIRE(sd::tos(f) == -0x40000000);
        REQUIRE(sd::tos(d) == -0x4000000000000000LL);
    }
}

TEST_CASE("expr::fval", "[expr]") {
    SECTION("float") {
        unsigned int u = 0;
        u = sd::fval(2.f).to<unsigned int>();
        REQUIRE(u == 0x40000000U);
        u = sd::fval(-6.2598534e18f).to<unsigned int>();
        REQUIRE(u == 0xdeadbeefU);

        int s = 0;
        s = sd::fval(-2.f).to<int>();
        REQUIRE(s == -0x40000000);
    }

    SECTION("double") {
        unsigned long long u = 0;
        u = sd::fval(2.).to<unsigned long long>();
        REQUIRE(u == 0x4000000000000000ULL);
        u = sd::fval(-1.1885959257070704e+148).to<unsigned long long>();
        REQUIRE(u == 0xdeadbeefdeadbeefULL);

        long long s = 0;
        s = sd::fval(-2.).to<long long>();
        REQUIRE(s == -0x4000000000000000LL);
    }
}

TEST_CASE("expr::uval", "[expr]") {
    SECTION("float") {
        float f = 0;
        f = sd::uval(0xdeadbeefU).to<float>();
        REQUIRE(sd::tou(f) == 0xdeadbeefU);
        f = sd::uval(std::numeric_limits<uint32_t>::min()).to<float>();
        REQUIRE(sd::tou(f) == std::numeric_limits<uint32_t>::min());
        f = sd::uval(std::numeric_limits<uint32_t>::max()).to<float>();
        REQUIRE(sd::tou(f) == std::numeric_limits<uint32_t>::max());
    }
    SECTION("double") {
        double d = 0;
        d = sd::uval(0xdeadbeefdeadbeefULL).to<double>();
        REQUIRE(sd::tou(d) == 0xdeadbeefdeadbeefULL);
        d = sd::uval(std::numeric_limits<uint64_t>::min()).to<double>();
        REQUIRE(sd::tou(d) == std::numeric_limits<uint64_t>::min());
        d = sd::uval(std::numeric_limits<uint64_t>::max()).to<double>();
        REQUIRE(sd::tou(d) == std::numeric_limits<uint64_t>::max());
    }
}

TEST_CASE("expr::sval", "[expr]") {
    SECTION("float") {
        float f = 0;
        f = sd::sval(-123456789).to<float>();
        REQUIRE(sd::tos(f) == -123456789);
        f = sd::sval(std::numeric_limits<int32_t>::min()).to<float>();
        REQUIRE(sd::tos(f) == std::numeric_limits<int32_t>::min());
        f = sd::sval(std::numeric_limits<int32_t>::max()).to<float>();
        REQUIRE(sd::tos(f) == std::numeric_limits<int32_t>::max());
    }
    SECTION("double") {
        double d = 0;
        d = sd::sval(-1234567890123LL).to<double>();
        REQUIRE(sd::tos(d) == -1234567890123LL);
        d = sd::sval(std::numeric_limits<int64_t>::min()).to<double>();
        REQUIRE(sd::tos(d) == std::numeric_limits<int64_t>::min());
        d = sd::sval(std::numeric_limits<int64_t>::max()).to<double>();
        REQUIRE(sd::tos(d) == std::numeric_limits<int64_t>::max());
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
    REQUIRE(sd::tou(f) == 0xffffffffU);
    REQUIRE(sd::tou(d) == 0xffffffffffffffffULL);
}

TEST_CASE("expr::sign_bit", "[expr]") {
    float f = sd::sign_bit().to<float>();
    double d = sd::sign_bit().to<double>();
    REQUIRE(sd::tou(f) == 0x80000000U);
    REQUIRE(sd::tou(d) == 0x8000000000000000ULL);
}

TEST_CASE("expr::abs_mask", "[expr]") {
    float f = sd::abs_mask().to<float>();
    double d = sd::abs_mask().to<double>();
    REQUIRE(sd::tou(f) == 0x7fffffffU);
    REQUIRE(sd::tou(d) == 0x7fffffffffffffffULL);
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
