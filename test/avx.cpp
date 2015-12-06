#define SIMDIFY_NEED_AVX
#include <simdify/simdify.hpp>

#include "catch.hpp"
#include <array>
#include <type_traits>
#include <algorithm>
#include <functional>

using T = simd::avx;

alignas(T) const T::array_f bufA = { {
    -0.27787193f, +0.70154146f, -2.05181630f, +2.22944568f,
    -0.82358653f, -1.57705702f, +0.50797465f, -0.59003456f,
} };
alignas(T) const T::array_f bufB = { {
    -0.23645458f, +2.02369089f, -2.25835397f, +2.22944568f,
    +0.33756370f, -0.87587426f, -1.66416447f, -0.59003456f,
} };
alignas(T) const T::array_f bufC = { {
    +0.42862268f, +2.02369089f, +1.87786546f, +0.94070440f,
    +0.78734578f, -0.87587426f, +0.31994913f, -0.55829428f,
} };
alignas(T) const T::array_f bufN = { {
    std::numeric_limits<T::f_t>::infinity(),
    -std::numeric_limits<T::f_t>::infinity(),
    std::numeric_limits<T::f_t>::quiet_NaN(),
    std::numeric_limits<T::f_t>::max(),
    std::numeric_limits<T::f_t>::lowest(),
    std::numeric_limits<T::f_t>::min(),
    std::numeric_limits<T::f_t>::epsilon(),
    std::numeric_limits<T::f_t>::denorm_min(),
} };
alignas(T) const T::array_f bufZ = { { 0, 0, 0, 0, 0, 0, 0, 0 } };


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

TEST_CASE("AVX explicit construction", "[simd_t][x86][avx]") {
    alignas(T)T::array_f r = bufZ;
    auto tor = [&r](const T& t) {
        simd::aligned(r.data()) = t;
    };

    SECTION("default") {
        T t;
    }
    SECTION("copy") {
        T t;
        T t2(t);
    }
    SECTION("from f_t") {
        T t(1.2345678f);
        tor(t); for (auto val : r) REQUIRE(val == 1.2345678f);
    }
    SECTION("from mm_t") {
        T t(_mm256_load_ps(bufA.data()));
        tor(t); REQUIRE(r == bufA);
    }
    SECTION("from aligned, unaligned pointer") {
        T tb(simd::aligned(bufB.data()));
        T tc(simd::unaligned(bufC.data()));
        tor(tb); REQUIRE(r == bufB);
        tor(tc); REQUIRE(r == bufC);
    }
    SECTION("from simd::storage") {
        simd::storage<T> stor;
        stor = bufA;
        T t(stor);
        tor(t); REQUIRE(r == bufA);
    }
    SECTION("from utof, stof, zero, etc. (simd::tof family)") {
        T ta(simd::utof(0xdeadbeef));
        T tb(simd::stof(-123456789));
        T tc(simd::zero());
        T td(simd::all_bits());
        T te(simd::sign_bit());
        T tf(simd::abs_mask());
        tor(ta); for (auto val : r) REQUIRE(simd::tou(val) == 0xdeadbeef);
        tor(tb); for (auto val : r) REQUIRE(simd::tos(val) == -123456789);
        tor(tc); for (auto val : r) REQUIRE(simd::tou(val) == 0x00000000);
        tor(td); for (auto val : r) REQUIRE(simd::tou(val) == 0xffffffff);
        tor(te); for (auto val : r) REQUIRE(simd::tou(val) == 0x80000000);
        tor(tf); for (auto val : r) REQUIRE(simd::tou(val) == 0x7fffffff);
    }
}


TEST_CASE("AVX implicit construction", "[simd_t][x86][avx]") {
    alignas(T)T::array_f r = bufZ;
    auto implicit_test = [&r](const T& t) {
        simd::aligned(r.data()) = t;
    };

    SECTION("from f_t") {
        implicit_test(1.2345678f);
        for (auto val : r) REQUIRE(val == 1.2345678f);
    }
    SECTION("from mm_t") {
        implicit_test(_mm256_load_ps(bufA.data()));
        REQUIRE(r == bufA);
    }
    SECTION("from aligned, unaligned pointer") {
        implicit_test(simd::aligned(bufB.data()));
        REQUIRE(r == bufB);
        implicit_test(simd::unaligned(bufC.data()));
        REQUIRE(r == bufC);
    }
    SECTION("from simd::storage") {
        simd::storage<T> stor;
        stor = bufA;
        implicit_test(stor);
        REQUIRE(r == bufA);
    }
    SECTION("from utof, stof, zero, etc. (simd::tof family)") {
        implicit_test(simd::utof(0xdeadbeef));
        for (auto val : r) REQUIRE(simd::tou(val) == 0xdeadbeef);
        implicit_test(simd::stof(-123456789));
        for (auto val : r) REQUIRE(simd::tos(val) == -123456789);
        implicit_test(simd::zero());
        for (auto val : r) REQUIRE(simd::tou(val) == 0x00000000);
        implicit_test(simd::all_bits());
        for (auto val : r) REQUIRE(simd::tou(val) == 0xffffffff);
        implicit_test(simd::sign_bit());
        for (auto val : r) REQUIRE(simd::tou(val) == 0x80000000);
        implicit_test(simd::abs_mask());
        for (auto val : r) REQUIRE(simd::tou(val) == 0x7fffffff);
    }
}

TEST_CASE("AVX assignment", "[simd_t][x86][avx]") {
    alignas(T)T::array_f r = bufZ;
    T t;
    auto tor = [&r, &t]() {
        simd::aligned(r.data()) = t;
    };

    SECTION("copy") {
        T t2;
        t2 = t;
    }
    SECTION("from f_t") {
        t = 1.2345678f;
        tor(); for (auto val : r) REQUIRE(val == 1.2345678f);
    }
    SECTION("from mm_t") {
        t = _mm256_load_ps(bufA.data());
        tor(); REQUIRE(r == bufA);
    }
    SECTION("from aligned, unaligned pointer") {
        t = simd::aligned(bufB.data());
        tor(); REQUIRE(r == bufB);
        t = simd::unaligned(bufC.data());
        tor(); REQUIRE(r == bufC);
    }
    SECTION("from simd::storage") {
        simd::storage<T> stor;
        stor = bufA;
        t = stor;
        tor(); REQUIRE(r == bufA);
    }
    SECTION("from an expression involving simd::storage") {
        simd::storage<T> stor;
        stor = bufA;
        t =  1.234f * stor;
        int i = 0;
        tor(); for (auto val : r) REQUIRE(val == 1.234f * bufA[i++]);
    }
    SECTION("from utof, stof, zero, etc. (simd::tof family)") {
        t = simd::utof(0xdeadbeef);
        tor(); for (auto val : r) REQUIRE(simd::tou(val) == 0xdeadbeef);
        t = simd::stof(-123456789);
        tor(); for (auto val : r) REQUIRE(simd::tos(val) == -123456789);
        t = simd::zero();
        tor(); for (auto val : r) REQUIRE(simd::tou(val) == 0x00000000);
        t = simd::all_bits();
        tor(); for (auto val : r) REQUIRE(simd::tou(val) == 0xffffffff);
        t = simd::sign_bit();
        tor(); for (auto val : r) REQUIRE(simd::tou(val) == 0x80000000);
        t = simd::abs_mask();
        tor(); for (auto val : r) REQUIRE(simd::tou(val) == 0x7fffffff);
    }
}

TEST_CASE("AVX arithmetic", "[simd_t][x86][avx]") {
    alignas(T)T::array_f r;
    alignas(T)T::array_f e;
    T a = simd::aligned(bufA.data());
    T b = simd::aligned(bufB.data());
    auto tor = [&r](const T& t) {
        simd::aligned(r.data()) = t;
    };

    SECTION("unary plus") {
        std::transform(begin(bufA), end(bufA), begin(e), [](T::f_t a) { return +a; });
        tor(+a);
        REQUIRE(r == e);
    }
    SECTION("unary minus") {
        std::transform(begin(bufA), end(bufA), begin(e), std::negate<T::f_t>());
        tor(-a);
        REQUIRE(r == e);
    }
    SECTION("plus") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), std::plus<T::f_t>());
        tor(a + b);
        REQUIRE(r == e);
    }
    SECTION("minus") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), std::minus<T::f_t>());
        tor(a - b);
        REQUIRE(r == e);
    }
    SECTION("multiplies") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), std::multiplies<T::f_t>());
        tor(a * b);
        REQUIRE(r == e);
    }
    SECTION("divides") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), std::divides<T::f_t>());
        tor(a / b);
        REQUIRE(r == e);
    }
}

TEST_CASE("AVX comparison", "[simd_t][x86][avx]") {
    auto if_ = [](bool in) { return in ? 0xffffffffU : 0x00000000U; };
    alignas(T)T::array_u r;
    alignas(T)T::array_u e;
    T a = simd::aligned(bufA.data());
    T b = simd::aligned(bufB.data());
    auto tor = [&r](const T& t) {
        simd::aligned(r.data()) = t;
    };

    SECTION("equal to") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [&if_](T::f_t a, T::f_t b) {
            return if_(a == b);
        });
        tor(a == b);
        REQUIRE(r == e);
    }
    SECTION("not equal to") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [&if_](T::f_t a, T::f_t b) {
            return if_(a != b);
        });
        tor(a != b);
        REQUIRE(r == e);
    }
    SECTION("greater") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [&if_](T::f_t a, T::f_t b) {
            return if_(a > b);
        });
        tor(a > b);
        REQUIRE(r == e);
    }
    SECTION("less") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [&if_](T::f_t a, T::f_t b) {
            return if_(a < b);
        });
        tor(a < b);
        REQUIRE(r == e);
    }
    SECTION("greater equal") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [&if_](T::f_t a, T::f_t b) {
            return if_(a >= b);
        });
        tor(a >= b);
        REQUIRE(r == e);
    }
    SECTION("less equal") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [&if_](T::f_t a, T::f_t b) {
            return if_(a <= b);
        });
        tor(a <= b);
        REQUIRE(r == e);
    }
}

TEST_CASE("AVX bitwise", "[simd_t][x86][avx]") {
    alignas(T)T::array_u r;
    alignas(T)T::array_u e;
    T a = simd::aligned(bufA.data());
    T b = simd::aligned(bufB.data());
    auto tor = [&r](const T& t) {
        simd::aligned(r.data()) = t;
    };

    SECTION("bit and") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [](T::f_t a, T::f_t b) {
            return simd::tou(a) & simd::tou(b);
        });
        tor(a & b);
        REQUIRE(r == e);
    }
    SECTION("bit or") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [](T::f_t a, T::f_t b) {
            return simd::tou(a) | simd::tou(b);
        });
        tor(a | b);
        REQUIRE(r == e);
    }
    SECTION("bit xor") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [](T::f_t a, T::f_t b) {
            return simd::tou(a) ^ simd::tou(b);
        });
        tor(a ^ b);
        REQUIRE(r == e);
    }
    SECTION("bit not") {
        std::transform(begin(bufA), end(bufA), begin(e), [](T::f_t a) {
            return ~simd::tou(a);
        });
        tor(~a);
        REQUIRE(r == e);
    }
    SECTION("complex expr") {
        std::transform(begin(bufA), end(bufA), begin(bufB), begin(e), [](T::f_t af, T::f_t bf) {
            auto a = simd::tou(af);
            auto b = simd::tou(bf);
            return ~((~a & ~b) | (~a ^ ~b));
        });
        tor(~((~a & ~b) | (~a ^ ~b)));
        REQUIRE(r == e);
    }
}
