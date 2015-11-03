#include "catch.hpp"

#include <array>
#include <type_traits>

#define SIMDIFY_NEED_AVX
#include <simdify/simdify.hpp>

using T = simd::avx;

alignas(T) const T::array_f bufA = {
    -0.27787193f, +0.70154146f, -2.05181630f, -0.35385000f,
    -0.82358653f, -1.57705702f, +0.50797465f, +0.28198406f,
};
alignas(T) const T::array_f bufB = {
    -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
    +0.33756370f, +1.00006082f, -1.66416447f, -0.59003456f,
};
alignas(T) const T::array_f bufC = {
    +0.42862268f, -1.03598478f, +1.87786546f, +0.94070440f,
    +0.78734578f, -0.87587426f, +0.31994913f, -0.55829428f,
};
alignas(T) const T::array_f bufZ = { 0, 0, 0, 0, 0, 0, 0, 0 };


TEST_CASE("AVX basic guarantees", "[simd_t][x86][avx]") {
    REQUIRE(T::W == 8);
    REQUIRE((std::is_same<T::mm_t, __m256>::value));
    REQUIRE((std::is_same<T::f_t, float>::value));
    REQUIRE((std::is_same<T::u_t, uint32_t>::value));
    REQUIRE((std::is_same<T::s_t, int32_t>::value));
    REQUIRE((std::is_same<T::array_f, std::array<T::f_t, T::W>>::value));
    REQUIRE((std::is_same<T::array_u, std::array<T::u_t, T::W>>::value));
    REQUIRE((std::is_same<T::array_s, std::array<T::s_t, T::W>>::value));
    REQUIRE(sizeof(T) == sizeof(T::mm_t));
    REQUIRE(alignof(T) == alignof(T::mm_t));
    REQUIRE(sizeof(T::f_t) * T::W == sizeof(T));
    REQUIRE(sizeof(T::f_t) == sizeof(T::u_t));
    REQUIRE(std::is_integral<T::u_t>::value);
    REQUIRE(std::is_unsigned<T::u_t>::value);
}

TEST_CASE("AVX construction", "[simd_t][x86][avx]") {
    alignas(T)T::array_f res_f = bufZ;

    SECTION("default") {
        T t;
    }
    SECTION("from f_t") {
        T::f_t f = 1.2345678f;
        T t(f);
        simd::aligned(res_f.data()) = t;
        for (auto val : res_f) REQUIRE(val == 1.2345678f);
    }
    SECTION("from f_t must be implicit") {
        auto implicit_test = [&res_f](const T& t) {
            simd::aligned(res_f.data()) = t;
        };
        implicit_test(1.2345678f);
        for (auto val : res_f) REQUIRE(val == 1.2345678f);
    }
    SECTION("from mm") {
        T::mm_t mm = _mm256_load_ps(bufA.data());
        T t(mm);
        simd::aligned(res_f.data()) = t; REQUIRE(res_f == bufA);
    }
    SECTION("from an aligned memory location") {
        T t(simd::aligned(bufB.data()));
        simd::aligned(res_f.data()) = t; REQUIRE(res_f == bufB);
    }
    SECTION("from utof, stof, zero, etc. (simd::tof family)") {
        T ta(simd::utof(0xdeadbeef));
        T tb(simd::stof(-123456789));
        T tc(simd::zero());
        T td(simd::all_bits());
        T te(simd::sign_bit());
        T tf(simd::abs_mask());
        simd::aligned(res_f.data()) = ta; for (auto val : res_f) REQUIRE(simd::tou(val) == 0xdeadbeef);
        simd::aligned(res_f.data()) = tb; for (auto val : res_f) REQUIRE(simd::tos(val) == -123456789);
        simd::aligned(res_f.data()) = tc; for (auto val : res_f) REQUIRE(simd::tou(val) == 0x00000000);
        simd::aligned(res_f.data()) = td; for (auto val : res_f) REQUIRE(simd::tou(val) == 0xffffffff);
        simd::aligned(res_f.data()) = te; for (auto val : res_f) REQUIRE(simd::tou(val) == 0x80000000);
        simd::aligned(res_f.data()) = tf; for (auto val : res_f) REQUIRE(simd::tou(val) == 0x7fffffff);
    }
}

TEST_CASE("AVX assignment", "[simd_t][x86][avx]") {

}

TEST_CASE("AVX arithmetic", "[simd_t][x86][avx]") {

}

TEST_CASE("AVX logical", "[simd_t][x86][avx]") {

}

TEST_CASE("AVX bitwise", "[simd_t][x86][avx]") {

}