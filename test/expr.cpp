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
