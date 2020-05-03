#include "catch.hpp"

#include <simdee/common/casts.hpp>

template <typename T>
int32_t int32_t_(T val) {
    return int32_t(val);
}

TEST_CASE("cast_b", "[casts]") {
    auto b8 = sd::bool8_t(true);
    auto u8 = uint8_t(-1);
    auto s8 = int8_t(-1);
    auto b16 = sd::bool16_t(true);
    auto u16 = uint16_t(-1);
    auto s16 = int16_t(-1);
    auto b32 = sd::bool32_t(true);
    auto u32 = uint32_t(-1);
    auto s32 = int32_t_(-1);
    auto b64 = sd::bool64_t(true);
    auto u64 = uint64_t(-1);
    auto s64 = int64_t(-1);

    static_assert(std::is_same<decltype(sd::cast_b(b8)), sd::bool8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(u8)), sd::bool8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(s8)), sd::bool8_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(b16)), sd::bool16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(u16)), sd::bool16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(s16)), sd::bool16_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(b32)), sd::bool32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(u32)), sd::bool32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(s32)), sd::bool32_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(b64)), sd::bool64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(u64)), sd::bool64_t>::value, "");
    static_assert(std::is_same<decltype(sd::cast_b(s64)), sd::bool64_t>::value, "");

    REQUIRE(sd::cast_b(b8) == sd::bool8_t(true));
    REQUIRE(sd::cast_b(u8) == sd::bool8_t(true));
    REQUIRE(sd::cast_b(s8) == sd::bool8_t(true));
    REQUIRE(sd::cast_b(b16) == sd::bool16_t(true));
    REQUIRE(sd::cast_b(u16) == sd::bool16_t(true));
    REQUIRE(sd::cast_b(s16) == sd::bool16_t(true));
    REQUIRE(sd::cast_b(b32) == sd::bool32_t(true));
    REQUIRE(sd::cast_b(u32) == sd::bool32_t(true));
    REQUIRE(sd::cast_b(s32) == sd::bool32_t(true));
    REQUIRE(sd::cast_b(b64) == sd::bool64_t(true));
    REQUIRE(sd::cast_b(u64) == sd::bool64_t(true));
    REQUIRE(sd::cast_b(s64) == sd::bool64_t(true));
}

TEST_CASE("cast_f", "[casts]") {
    auto b32 = sd::bool32_t(false);
    auto f32 = float(123);
    auto u32 = uint32_t(123);
    auto s32 = int32_t_(123);
    auto b64 = sd::bool64_t(false);
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

TEST_CASE("cast_u", "[casts]") {
    auto b8 = sd::bool8_t(true);
    auto u8 = uint8_t(-1);
    auto s8 = int8_t(-1);
    auto b16 = sd::bool16_t(true);
    auto u16 = uint16_t(-1);
    auto s16 = int16_t(-1);
    auto b32 = sd::bool32_t(true);
    auto f32 = float(123);
    auto u32 = uint32_t(-1);
    auto s32 = int32_t_(-1);
    auto b64 = sd::bool64_t(true);
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

TEST_CASE("cast_s", "[casts]") {
    auto b8 = sd::bool8_t(true);
    auto u8 = uint8_t(-1);
    auto s8 = int8_t(-1);
    auto b16 = sd::bool16_t(true);
    auto u16 = uint16_t(-1);
    auto s16 = int16_t(-1);
    auto b32 = sd::bool32_t(true);
    auto f32 = float(-1);
    auto u32 = uint32_t(-1);
    auto s32 = int32_t_(-1);
    auto b64 = sd::bool64_t(true);
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

TEST_CASE("dirty::as_b", "[casts]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::as_b(uint16_t(-1)) == sd::bool16_t(true));
        REQUIRE(sd::dirty::as_b(uint32_t(-1)) == sd::bool32_t(true));
        REQUIRE(sd::dirty::as_b(int16_t(-1)) == sd::bool16_t(true));
        REQUIRE(sd::dirty::as_b(int32_t_(-1)) == sd::bool32_t(true));
        REQUIRE(sd::dirty::as_b(0.f) == sd::bool32_t(false));
        REQUIRE(sd::dirty::as_b(0.) == sd::bool64_t(false));
    }
    SECTION("lvalues") {
        auto a1 = uint16_t(-1);
        auto a2 = uint32_t(-1);
        auto a3 = int16_t(-1);
        auto a4 = int32_t_(-1);
        auto a5 = 0.f;
        auto a6 = 0.;
        REQUIRE(sd::dirty::as_b(a1) == sd::bool16_t(true));
        REQUIRE(sd::dirty::as_b(a2) == sd::bool32_t(true));
        REQUIRE(sd::dirty::as_b(a3) == sd::bool16_t(true));
        REQUIRE(sd::dirty::as_b(a4) == sd::bool32_t(true));
        REQUIRE(sd::dirty::as_b(a5) == sd::bool32_t(false));
        REQUIRE(sd::dirty::as_b(a6) == sd::bool64_t(false));
    }
}

TEST_CASE("dirty::as_f", "[casts]") {
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

TEST_CASE("dirty::as_u", "[casts]") {
    SECTION("rvalues") {
        REQUIRE(sd::dirty::as_u(2.0f) == 0x40000000U);
        REQUIRE(sd::dirty::as_u(2.0) == 0x4000000000000000ULL);
        REQUIRE(sd::dirty::as_u(-6.2598534e18f) == 0xdeadbeefU);
        REQUIRE(sd::dirty::as_u(-1.1885959257070704e+148) == 0xdeadbeefdeadbeefULL);
        REQUIRE(sd::dirty::as_u(sd::bool8_t(false)) == 0);
        REQUIRE(sd::dirty::as_u(sd::bool8_t(true)) == uint8_t(-1));
        REQUIRE(sd::dirty::as_u(sd::bool16_t(true)) == uint16_t(-1));
        REQUIRE(sd::dirty::as_u(sd::bool32_t(true)) == uint32_t(-1));
        REQUIRE(sd::dirty::as_u(sd::bool64_t(true)) == uint64_t(-1));
    }
    SECTION("lvalues") {
        float f = 2;
        double d = 2;
        REQUIRE(sd::dirty::as_u(f) == 0x40000000U);
        REQUIRE(sd::dirty::as_u(d) == 0x4000000000000000ULL);
    }
}

TEST_CASE("dirty::as_s", "[casts]") {
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
