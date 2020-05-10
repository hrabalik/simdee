#include <catch2/catch.hpp>
#include <simdee/simdee.hpp>
#include <type_traits>

#if SIMDEE_SSE2

using B = sd::sseb;
const B::storage_t dataAB{true, false, true, true};
const B::storage_t dataBB{false, true, false, true};

using U = sd::sseu;
const U::storage_t dataAU{1753029375U, 1117080442U, 3817141237U, 3761735248U};
const U::storage_t dataBU{1679702461U, 2102346647U, 480083363U, 3761735248U};

using not_B = sd::expr::deferred_lognot<B>;
using not_U = sd::expr::deferred_bitnot<U>;

TEST_CASE("deferred_not types", "[deferred_not]") {
#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 6
// GCC attempts to be helpful here by warning that some attributes of types such as __m128 are not
// preserved when passsed as a template parameter.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

    REQUIRE((std::is_same<B::vector_t, not_B::vector_t>::value));
    REQUIRE((std::is_same<B::scalar_t, not_B::scalar_t>::value));
    REQUIRE((std::is_same<B::mask_t, not_B::mask_t>::value));
    REQUIRE((std::is_same<B::storage_t, not_B::storage_t>::value));
    REQUIRE((std::size_t(B::width) == not_B::width));

    REQUIRE((std::is_same<U::vector_t, not_U::vector_t>::value));
    REQUIRE((std::is_same<U::scalar_t, not_U::scalar_t>::value));
    REQUIRE((std::is_same<U::mask_t, not_U::mask_t>::value));
    REQUIRE((std::is_same<U::storage_t, not_U::storage_t>::value));
    REQUIRE((std::size_t(U::width) == not_U::width));

#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 6
#pragma GCC diagnostic pop
#endif
}

TEST_CASE("deferred_lognot cond", "[deferred_not]") {
    const B ba = !dataAB;
    const B bb = !dataBB;
    const not_B nba(dataAB);
    const not_B nbb(dataBB);

    auto expB = cond(ba, ba, bb);
    REQUIRE(all(expB == cond(ba, ba, nbb)));
    REQUIRE(all(expB == cond(ba, nba, bb)));
    REQUIRE(all(expB == cond(ba, nba, nbb)));
    REQUIRE(all(expB == cond(nba, ba, bb)));
    REQUIRE(all(expB == cond(nba, ba, nbb)));
    REQUIRE(all(expB == cond(nba, nba, bb)));
    REQUIRE(all(expB == cond(nba, nba, nbb)));

    const U ua = dataAU;
    const U ub = dataBU;
    auto expU = cond(ba, ua, ub);
    REQUIRE(all(expU == cond(ba, ua, ub)));
    REQUIRE(all(expU == cond(nba, ua, ub)));
}

TEST_CASE("deferred_lognot bool arithmetic", "[deferred_not]") {
    const B ba = !dataAB;
    const not_B nba(dataAB);
    SECTION("unary") {
        SECTION("log not") {
            REQUIRE(all((ba) == (nba)));
            REQUIRE(all((!ba) == (!nba)));
            REQUIRE(all((ba) == (!!nba)));
            REQUIRE(all((!!ba) == nba));
            REQUIRE(all((!!ba) == (!!nba)));
            REQUIRE(all((!!!ba) == (!!!nba)));
        }
        SECTION("any, all") {
            const B zero = sd::zero();
            const B allb = sd::all_bits();
            const not_B nzero(B(!zero));
            const not_B nallb(B(!allb));
            REQUIRE(any(ba) == any(nba));
            REQUIRE(all(ba) == all(nba));
            REQUIRE(any(zero) == any(nzero));
            REQUIRE(all(zero) == all(nzero));
            REQUIRE(any(allb) == any(nallb));
            REQUIRE(all(allb) == all(nallb));
        }
    }
    SECTION("binary") {
        const B bb = !dataBB;
        const not_B nbb(dataBB);
        SECTION("log and") {
            B expect = ba && bb;
            REQUIRE(all((ba && nbb) == expect));
            REQUIRE(all((nba && bb) == expect));
            REQUIRE(all((nba && nbb) == expect));
        }
        SECTION("log or") {
            B expect = ba || bb;
            REQUIRE(all((ba || nbb) == expect));
            REQUIRE(all((nba || bb) == expect));
            REQUIRE(all((nba || nbb) == expect));
        }
    }
}

TEST_CASE("deferred_bitnot uint arithmetic", "[deferred_not]") {
    const U ua = ~dataAU;
    const not_U nua(dataAU);
    SECTION("unary") {
        SECTION("bit not") {
            REQUIRE(all(ua == nua));
            REQUIRE(all(~ua == ~nua));
            REQUIRE(all(ua == ~~nua));
            REQUIRE(all(~~ua == nua));
            REQUIRE(all(~~ua == ~~nua));
            REQUIRE(all(~~~ua == ~~~nua));
        }
    }
    SECTION("binary") {
        const U ub = ~dataBU;
        const not_U nub(dataBU);
        SECTION("bit and") {
            U expect = ua & ub;
            REQUIRE(all((ua & nub) == expect));
            REQUIRE(all((nua & ub) == expect));
            REQUIRE(all((nua & nub) == expect));
        }
        SECTION("bit or") {
            U expect = ua | ub;
            REQUIRE(all((ua | nub) == expect));
            REQUIRE(all((nua | ub) == expect));
            REQUIRE(all((nua | nub) == expect));
        }
        SECTION("bit xor") {
            U expect = ua ^ ub;
            REQUIRE(all((ua ^ nub) == expect));
            REQUIRE(all((nua ^ ub) == expect));
            REQUIRE(all((nua ^ nub) == expect));
        }
    }
}

TEST_CASE("deferred_lognot methods", "[deferred_not]") {
    const B ba = !dataAB;
    const not_B nba(dataAB);
    SECTION("eval") {
        auto res_ba = ba.eval();
        auto res_nba = nba.eval();
        REQUIRE((std::is_same<decltype(res_ba), decltype(res_nba)>::value));
        REQUIRE(all(res_ba == res_nba));
    }
    SECTION("data") { REQUIRE(all(B(ba.data()) == B(nba.data()))); }
    SECTION("store") {
        SECTION("aligned") {
            B::storage_t res_ba{}, res_nba{};
            ba.aligned_store(res_ba.data());
            nba.aligned_store(res_nba.data());
            REQUIRE((res_ba == res_nba));
        }
        SECTION("unaligned") {
            B::storage_t res_ba{}, res_nba{};
            ba.unaligned_store(res_ba.data());
            nba.unaligned_store(res_nba.data());
            REQUIRE((res_ba == res_nba));
        }
        SECTION("interleaved") {
            B::storage_t res_ba[3]{}, res_nba[3]{};
            ba.interleaved_store(res_ba[0].data(), 3);
            nba.interleaved_store(res_nba[0].data(), 3);
            REQUIRE((res_ba[0] == res_nba[0]));
            REQUIRE((res_ba[1] == res_nba[1]));
            REQUIRE((res_ba[2] == res_nba[2]));
        }
    }
    SECTION("reduce") {
        auto res_ba = first_scalar(reduce(ba, [](B a, B b) { return a && b; }));
        auto res_nba = first_scalar(reduce(nba, [](B a, B b) { return a && b; }));
        REQUIRE(res_ba == res_nba);
    }
    SECTION("mask") {
        auto res_ba = mask(ba);
        auto res_nba = mask(nba);
        REQUIRE(res_ba == res_nba);
    }
    SECTION("first_scalar") {
        auto res_ba = first_scalar(ba);
        auto res_nba = first_scalar(nba);
        REQUIRE(res_ba == res_nba);
    }
}

TEST_CASE("deferred_bitnot methods", "[deferred_not]") {
    const U ua = ~dataAU;
    const not_U nua(dataAU);
    SECTION("eval") {
        auto res_ua = ua.eval();
        auto res_nua = nua.eval();
        REQUIRE((std::is_same<decltype(res_ua), decltype(res_nua)>::value));
        REQUIRE(all(res_ua == res_nua));
    }
    SECTION("data") { REQUIRE(all(U(ua.data()) == U(nua.data()))); }
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
        auto res_ua = first_scalar(reduce(ua, [](U a, U b) { return a ^ b; }));
        auto res_nua = first_scalar(reduce(nua, [](U a, U b) { return a ^ b; }));
        REQUIRE(res_ua == res_nua);
    }
    SECTION("first_scalar") {
        auto res_ua = first_scalar(ua);
        auto res_nua = first_scalar(nua);
        REQUIRE(res_ua == res_nua);
    }
}

#endif
