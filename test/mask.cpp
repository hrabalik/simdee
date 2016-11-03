#include "catch.hpp"
#include <simdee/common/mask.hpp>
#include <vector>

constexpr uint32_t activeBits = 0x77777777U;
using mask_t = sd::impl::mask<activeBits>;
constexpr uint32_t u1 = 0xdeadbeefU & activeBits;
constexpr uint32_t u2 = 0xabcd1234U & activeBits;

TEST_CASE("mask", "[mask]") {
    constexpr mask_t m0(0);
    constexpr mask_t m1(u1);
    constexpr mask_t m2(u2);
    constexpr mask_t mA(activeBits);

    SECTION("equality") {
        REQUIRE((m1 == m2) == (u1 == u2));
        REQUIRE((m1 != m2) == (u1 != u2));
        REQUIRE((m1 == m1) == (u1 == u1));
        REQUIRE((m1 != m1) == (u1 != u1));
    }

    SECTION("bitwise ops") {
        REQUIRE((m1 & m2).value == (u1 & u2));
        REQUIRE((m1 | m2).value == (u1 | u2));
        REQUIRE((m1 ^ m2).value == (u1 ^ u2));
        REQUIRE((~m1).value == (u1 ^ activeBits));
        REQUIRE((~m2).value == (u2 ^ activeBits));

        {
            mask_t m(m0);
            m |= m1;
            m |= m2;
            REQUIRE(m.value == (m1 | m2).value);
        }

        {
            mask_t m(mA);
            m &= m1;
            m &= m2;
            REQUIRE(m.value == (m1 & m2).value);
        }

        {
            mask_t m(m0);
            m ^= m1;
            m ^= m2;
            REQUIRE(m.value == (m1 ^ m2).value);
        }
    }

    SECTION("any(), all()") {
        REQUIRE(any(m0) == false);
        REQUIRE(any(m1) == true);
        REQUIRE(any(m2) == true);
        REQUIRE(any(mA) == true);

        REQUIRE(all(m0) == false);
        REQUIRE(all(m1) == false);
        REQUIRE(all(m2) == false);
        REQUIRE(all(mA) == true);
    }

    SECTION("iteration") {
        auto received = [](mask_t m) {
            std::vector<uint32_t> res;
            for (auto i : m) res.push_back(i);
            return res;
        };
        auto expected = [](uint32_t u) {
            std::vector<uint32_t> res;
            for (int i = 0; i < 32; ++i) {
                if ((u & (1U << i)) != 0) {
                    res.push_back(i);
                }
            }
            return res;
        };
        REQUIRE(received(m1) == expected(u1));
        REQUIRE(received(m2) == expected(u2));
    }

    SECTION("operator []") {
        auto received = [](mask_t m) {
            std::vector<bool> res;
            for (int i = 0; i < 32; ++i) {
                res.push_back(m[i]);
            }
            return res;
        };
        auto expected = [](uint32_t u) {
            std::vector<bool> res;
            for (int i = 0; i < 32; ++i) {
                res.push_back((u & (1U << i)) != 0);
            }
            return res;
        };
        REQUIRE(received(m1) == expected(u1));
        REQUIRE(received(m2) == expected(u2));
    }
}
