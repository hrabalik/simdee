#define SIMDIFY_NEED_SSE 1
#include <simdify/simdify.hpp>

#include "catch.hpp"
#include <type_traits>

using U = simd::sseu;
const U::storage_t dataA{ 1753029375U, 1117080442U, 3817141237U, 3761735248U };

using not_U = simd::expr::deferred_not<U>;

TEST_CASE("deferred_not types", "[deferred_not]") {
    REQUIRE((std::is_same<U::vector_t, not_U::vector_t>::value));
    REQUIRE((std::is_same<U::scalar_t, not_U::scalar_t>::value));
    REQUIRE((std::is_same<U::mask_t, not_U::mask_t>::value));
    REQUIRE((std::is_same<U::storage_t, not_U::storage_t>::value));
    REQUIRE((std::is_same<U::binary_op_t, not_U::binary_op_t>::value));
    REQUIRE((std::size_t(U::width) == not_U::width));
}

TEST_CASE("deferred_not arithmetic", "[deferred_not]") {
    U ua = ~dataA;
    not_U nua(dataA);
    SECTION("unary") {
        SECTION("bit not") {
            REQUIRE(all(ua == nua));
            REQUIRE(all(~ua == ~nua));
        }
        SECTION("any, all") {
            U zero = simd::zero();
            U allb = simd::all_bits();
            not_U nzero(U(~zero));
            not_U nallb(U(~allb));
            REQUIRE(any(ua) == any(nua));
            REQUIRE(all(ua) == all(nua));
            REQUIRE(any(zero) == any(nzero));
            REQUIRE(all(zero) == all(nzero));
            REQUIRE(any(allb) == any(nallb));
            REQUIRE(all(allb) == all(nallb));
        }
    }
}

TEST_CASE("deferred_not methods", "[deferred_not]") {
    const U ua = ~dataA;
    const not_U nua(dataA);
    SECTION("data") {
        REQUIRE(all(U(ua.data()) == U(nua.data())));
    }
    SECTION("store") {
        SECTION("aligned") {
            U::storage_t res_ua{}, res_nua{};
            ua.aligned_store(res_ua.data());
            nua.aligned_store(res_nua.data());
            REQUIRE((res_ua == res_nua));
        }
        SECTION("unaligned") {
            U::storage_t res_ua{}, res_nua{};
            ua.unaligned_store(res_ua.data());
            nua.unaligned_store(res_nua.data());
            REQUIRE((res_ua == res_nua));
        }
        SECTION("interleaved") {
            U::storage_t res_ua[3]{}, res_nua[3]{};
            ua.interleaved_store(res_ua[0].data(), 3);
            nua.interleaved_store(res_nua[0].data(), 3);
            REQUIRE((res_ua[0] == res_nua[0]));
            REQUIRE((res_ua[1] == res_nua[1]));
            REQUIRE((res_ua[2] == res_nua[2]));
        }
    }
    SECTION("reduce") {
        auto res_ua = ua.reduce(simd::operator^).first_element();
        auto res_nua = nua.reduce(simd::operator^).first_element();
        REQUIRE(res_ua == res_nua);
    }
    SECTION("mask") {
        auto res_ua = ua.mask();
        auto res_nua = nua.mask();
        REQUIRE(res_ua == res_nua);
    }
    SECTION("first_element") {
        auto res_ua = ua.first_element();
        auto res_nua = nua.first_element();
        REQUIRE(res_ua == res_nua);
    }
}
