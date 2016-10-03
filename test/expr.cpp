#include "catch.hpp"

#include <simdee/common/expr.hpp>

TEST_CASE("cast_f", "[expr]") {
    auto b32 = sd::bool32_t::F;
    auto f32 = float(123);
    auto u32 = uint32_t(123);
    auto s32 = int32_t(123);
    auto b64 = sd::bool64_t::F;
    auto f64 = double(123);
    auto u64 = uint64_t(123);
    auto s64 = int64_t(123);

    static_assert(std::is_same<decltype(sd::cast_f(b32)), float>::value, "");
    static_assert(std::is_same<decltype(sd::cast_f(f32)), float>::value, "");
    static_assert(std::is_same<decltype(sd::cast_f(u32)), float>::value, "");
    static_assert(std::is_same<decltype(sd::cast_f(s32)), float>::value, "");
    static_assert(std::is_same<decltype(sd::cast_f(b64)), double>::value, "");
    static_assert(std::is_same<decltype(sd::cast_f(f64)), double>::value, "");
    static_assert(std::is_same<decltype(sd::cast_f(u64)), double>::value, "");
    static_assert(std::is_same<decltype(sd::cast_f(s64)), double>::value, "");

    REQUIRE(sd::cast_f(b32) == 0.f);
    REQUIRE(sd::cast_f(f32) == 123.f);
    REQUIRE(sd::cast_f(u32) == 123.f);
    REQUIRE(sd::cast_f(s32) == 123.f);
    REQUIRE(sd::cast_f(b64) == 0.);
    REQUIRE(sd::cast_f(f64) == 123.);
    REQUIRE(sd::cast_f(u64) == 123.);
    REQUIRE(sd::cast_f(s64) == 123.);
}

TEST_CASE("cast_u", "[expr]") {
    auto b8 = sd::bool8_t::T;
    auto u8 = uint8_t(-1);
    auto s8 = int8_t(-1);
    auto b16 = sd::bool16_t::T;
    auto u16 = uint16_t(-1);
    auto s16 = int16_t(-1);
    auto b32 = sd::bool32_t::T;
    auto f32 = float(123);
    auto u32 = uint32_t(-1);
    auto s32 = int32_t(-1);
    auto b64 = sd::bool64_t::T;
    auto f64 = double(123);
    auto u64 = uint64_t(-1);
    auto s64 = int64_t(-1);

    static_assert(std::is_same<decltype(sd::cast_u(b8)), uint8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(u8)), uint8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(s8)), uint8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(b16)), uint16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(u16)), uint16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(s16)), uint16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(b32)), uint32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(f32)), uint32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(u32)), uint32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(s32)), uint32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(b64)), uint64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(f64)), uint64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(u64)), uint64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_u(s64)), uint64_t>::value, "");

    REQUIRE(sd::cast_u(b8) == 0xffU);
    REQUIRE(sd::cast_u(u8) == 0xffU);
    REQUIRE(sd::cast_u(s8) == 0xffU);
    REQUIRE(sd::cast_u(b16) == 0xffffU);
    REQUIRE(sd::cast_u(u16) == 0xffffU);
    REQUIRE(sd::cast_u(s16) == 0xffffU);
    REQUIRE(sd::cast_u(b32) == 0xffffffffU);
    REQUIRE(sd::cast_u(f32) == 123);
    REQUIRE(sd::cast_u(u32) == 0xffffffffU);
    REQUIRE(sd::cast_u(s32) == 0xffffffffU);
    REQUIRE(sd::cast_u(b64) == 0xffffffffffffffffULL);
    REQUIRE(sd::cast_u(f64) == 123);
    REQUIRE(sd::cast_u(u64) == 0xffffffffffffffffULL);
    REQUIRE(sd::cast_u(s64) == 0xffffffffffffffffULL);
}

TEST_CASE("cast_s", "[expr]") {
    auto b8 = sd::bool8_t::T;
    auto u8 = uint8_t(-1);
    auto s8 = int8_t(-1);
    auto b16 = sd::bool16_t::T;
    auto u16 = uint16_t(-1);
    auto s16 = int16_t(-1);
    auto b32 = sd::bool32_t::T;
    auto f32 = float(-1);
    auto u32 = uint32_t(-1);
    auto s32 = int32_t(-1);
    auto b64 = sd::bool64_t::T;
    auto f64 = double(-1);
    auto u64 = uint64_t(-1);
    auto s64 = int64_t(-1);

    static_assert(std::is_same<decltype(sd::cast_s(b8)), int8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(u8)), int8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(s8)), int8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(b16)), int16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(u16)), int16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(s16)), int16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(b32)), int32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(f32)), int32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(u32)), int32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(s32)), int32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(b64)), int64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(f64)), int64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(u64)), int64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_s(s64)), int64_t>::value, "");

    REQUIRE(sd::cast_s(b8) == -1);
    REQUIRE(sd::cast_s(u8) == -1);
    REQUIRE(sd::cast_s(s8) == -1);
    REQUIRE(sd::cast_s(b16) == -1);
    REQUIRE(sd::cast_s(u16) == -1);
    REQUIRE(sd::cast_s(s16) == -1);
    REQUIRE(sd::cast_s(b32) == -1);
    REQUIRE(sd::cast_s(f32) == -1);
    REQUIRE(sd::cast_s(u32) == -1);
    REQUIRE(sd::cast_s(s32) == -1);
    REQUIRE(sd::cast_s(b64) == -1);
    REQUIRE(sd::cast_s(f64) == -1);
    REQUIRE(sd::cast_s(u64) == -1);
    REQUIRE(sd::cast_s(s64) == -1);
}

TEST_CASE("dirty::as_f", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::as_f(0x40000000U) == 2.0f);
        REQUIRE(sd::dirty::as_f(0x4000000000000000ULL) == 2.0);
        REQUIRE(sd::dirty::as_f(0xdeadbeefU) == -6.2598534e18f);
        REQUIRE(sd::dirty::as_f(0xdeadbeefdeadbeefULL) == -1.1885959257070704e+148);
        REQUIRE(sd::dirty::as_f(-0x40000000) == -2.0f);
        REQUIRE(sd::dirty::as_f(-0x4000000000000000LL) == -2.0);
    }
    SECTION("lvalues") {
        unsigned int u = 0x40000000U;
        unsigned long long ul = 0x4000000000000000ULL;
        int s = -0x40000000;
        long long sl = -0x4000000000000000LL;
        REQUIRE(sd::dirty::as_f(u) == 2.0f);
        REQUIRE(sd::dirty::as_f(ul) == 2.0);
        REQUIRE(sd::dirty::as_f(s) == -2.0f);
        REQUIRE(sd::dirty::as_f(sl) == -2.0);
    }
}

TEST_CASE("dirty::as_u", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::as_u(2.0f) == 0x40000000U);
        REQUIRE(sd::dirty::as_u(2.0) == 0x4000000000000000ULL);
        REQUIRE(sd::dirty::as_u(-6.2598534e18f) == 0xdeadbeefU);
        REQUIRE(sd::dirty::as_u(-1.1885959257070704e+148) == 0xdeadbeefdeadbeefULL);
        REQUIRE(sd::dirty::as_u(sd::bool8_t::F) == 0);
        REQUIRE(sd::dirty::as_u(sd::bool8_t::T) == uint8_t(-1));
        REQUIRE(sd::dirty::as_u(sd::bool16_t::T) == uint16_t(-1));
        REQUIRE(sd::dirty::as_u(sd::bool32_t::T) == uint32_t(-1));
        REQUIRE(sd::dirty::as_u(sd::bool64_t::T) == uint64_t(-1));
    }
    SECTION("lvalues") {
        float f = 2;
        double d = 2;
        REQUIRE(sd::dirty::as_u(f) == 0x40000000U);
        REQUIRE(sd::dirty::as_u(d) == 0x4000000000000000ULL);
    }
}

TEST_CASE("dirty::as_s", "[expr]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::as_s(-2.0f) == -0x40000000);
        REQUIRE(sd::dirty::as_s(-2.0) == -0x4000000000000000LL);
    }
    SECTION("lvalues") {
        float f = -2;
        double d = -2;
        REQUIRE(sd::dirty::as_s(f) == -0x40000000);
        REQUIRE(sd::dirty::as_s(d) == -0x4000000000000000LL);
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
    REQUIRE(sd::dirty::as_u(f) == 0xffffffffU);
    REQUIRE(sd::dirty::as_u(d) == 0xffffffffffffffffULL);
}

TEST_CASE("expr::sign_bit", "[expr]") {
    float f = sd::sign_bit().to<float>();
    double d = sd::sign_bit().to<double>();
    REQUIRE(sd::dirty::as_u(f) == 0x80000000U);
    REQUIRE(sd::dirty::as_u(d) == 0x8000000000000000ULL);
}

TEST_CASE("expr::abs_mask", "[expr]") {
    float f = sd::abs_mask().to<float>();
    double d = sd::abs_mask().to<double>();
    REQUIRE(sd::dirty::as_u(f) == 0x7fffffffU);
    REQUIRE(sd::dirty::as_u(d) == 0x7fffffffffffffffULL);
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
