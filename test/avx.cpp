#include "catch.hpp"

#include <array>
#include <type_traits>

#define SIMDIFY_NEED_AVX
#include <simdify/simdify.hpp>

using T = simd::avx;
using buf_t = std::array<T::fp_t, T::W>;

alignas(T) const buf_t BA = {
    -0.27787193f, +0.70154146f, -2.05181630f, -0.35385000f,
    -0.82358653f, -1.57705702f, +0.50797465f, +0.28198406f,
};
alignas(T) const buf_t BB = {
    -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
    +0.33756370f, +1.00006082f, -1.66416447f, -0.59003456f,
};
alignas(T) const buf_t BC = {
    +0.42862268f, -1.03598478f, +1.87786546f, +0.94070440f,
    +0.78734578f, -0.87587426f, +0.31994913f, -0.55829428f,
};

TEST_CASE("AVX", "[simd][x86][avx]") {
    SECTION("basic guarantees") {
        REQUIRE(T::W == 8);
        REQUIRE((std::is_same<T::mm_t, __m256>::value));
        REQUIRE((std::is_same<T::fp_t, float>::value));
        REQUIRE((std::is_same<T::bitmask_t, uint32_t>::value));
        REQUIRE((std::is_same<T::array_t, buf_t>::value));
        REQUIRE(sizeof(T) == sizeof(T::mm_t));
        REQUIRE(alignof(T) == alignof(T::mm_t));
        REQUIRE(sizeof(T::fp_t) * T::W == sizeof(T));
    }
    SECTION("construction") {
        SECTION("default") {
            T t;
        }
        SECTION("from mm") {
            T::mm_t mm = _mm256_load_ps(BA.data());
            T t(mm);
            REQUIRE(t.f == BA);
        }
        //SECTION("from pointer") {
        //    T t(BA.data());
        //    REQUIRE(t.f == BA);
        //}
        SECTION("from zero_t") {
            T t(simd::ZERO);
            for (auto val : t.i) REQUIRE(val == 0);
        }
        SECTION("from all_bits_t") {
            T t(simd::ALL_BITS);
            for (auto val : t.i) REQUIRE(val == ~T::bitmask_t(0));
        }
        SECTION("from abs_mask_t") {
            T t(simd::ABS_MASK);
            for (auto val : t.i) REQUIRE(val == (~T::bitmask_t(0)) >> 1);
        }
        SECTION("from sign_bit_t") {
            T t(simd::SIGN_BIT);
            for (auto val : t.f) REQUIRE(val == -0.f);
        }
    }
    SECTION("operations") {
        SECTION("arithmetic") {
        
        }
    }
}
