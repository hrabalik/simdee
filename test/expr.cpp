#include "catch.hpp"

#include <array>
#include <type_traits>

#include <simdify/simdify.hpp>

TEST_CASE("expr::uint, expr::sint", "[expr]") {
    static const uint32_t Dbu = 0xdeadbeefU;
    static const uint64_t Dbull = 0xdeadbeefdeadbeefULL;
    static const float Dbf = -6.2598534e18f;
    static const double Dbd = -1.1885959257070704e+148;

    SECTION("verification of Dbf, Dbd") {
        REQUIRE(Dbf == reinterpret_cast<const float&>(Dbu));
        REQUIRE(Dbd == reinterpret_cast<const double&>(Dbull));
    }
    SECTION("construction of floating-point types") {
        SECTION("parentheses") {
            float f(simd::uint(Dbu));
            double d(simd::uint(Dbull));
            REQUIRE(f == Dbf);
            REQUIRE(d == Dbd);
        }
        SECTION("braces") {
            float f{ simd::uint(Dbu) };
            double d{ simd::uint(Dbull) };
            REQUIRE(f == Dbf);
            REQUIRE(d == Dbd);
        }
        SECTION("equals cast") {
            float f = float(simd::uint(Dbu));
            double d = double(simd::uint(Dbull));
            REQUIRE(f == Dbf);
            REQUIRE(d == Dbd);
        }
        SECTION("equals c-style cast") {
            float f = (float)simd::uint(Dbu);
            double d = (double)simd::uint(Dbull);
            REQUIRE(f == Dbf);
            REQUIRE(d == Dbd);
        }
        SECTION("equals uintf/uintd") {
            float f = simd::uintf(Dbu);
            double d = simd::uintd(Dbull);
            REQUIRE(f == Dbf);
            REQUIRE(d == Dbd);
        }
    }
    SECTION("comparison with floating-point types") {
        REQUIRE(Dbf == float(simd::uint(Dbu)));
        REQUIRE(Dbf == (float)simd::uint(Dbu));
        REQUIRE(Dbf == simd::uintf(Dbu));
        REQUIRE(Dbd == double(simd::uint(Dbull)));
        REQUIRE(Dbd == (double)simd::uint(Dbull));
        REQUIRE(Dbd == simd::uintd(Dbull));
        REQUIRE(float(simd::uint(Dbu)) == Dbf);
        REQUIRE((float)simd::uint(Dbu) == Dbf);
        REQUIRE(simd::uintf(Dbu) == Dbf);
        REQUIRE(double(simd::uint(Dbull)) == Dbd);
        REQUIRE((double)simd::uint(Dbull) == Dbd);
        REQUIRE(simd::uintd(Dbull) == Dbd);
    }
    SECTION("sint") {
        float f = simd::sintf(-123456789);
        double d1 = simd::sintd(-9223372036854775807LL);
        double d2 = simd::sintd(+9223372036854775807LL);
        REQUIRE(f == simd::sintf(-123456789));
        REQUIRE(d1 == simd::sintd(-9223372036854775807LL));
        //REQUIRE(d2 == simd::sintd(+9223372036854775807LL)); // fails, NaN != NaN
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
