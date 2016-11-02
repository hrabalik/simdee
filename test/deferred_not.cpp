#include "catch.hpp"
#include <simdee/simdee.hpp>
#include <type_traits>

using B = sd::vec4b;
const B::storage_t dataAB{ B::scalar_t(true), B::scalar_t(false), B::scalar_t(true), B::scalar_t(true) };
const B::storage_t dataBB{ B::scalar_t(false), B::scalar_t(true), B::scalar_t(false), B::scalar_t(true) };

using U = sd::vec4u;
const U::storage_t dataAU{ 1753029375U, 1117080442U, 3817141237U, 3761735248U };
const U::storage_t dataBU{ 1679702461U, 2102346647U, 480083363U, 3761735248U };

using not_B = sd::expr::deferred_lognot<B>;
using not_U = sd::expr::deferred_bitnot<U>;

TEST_CASE("deferred_not types", "[deferred_not]") {
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
}

TEST_CASE("deferred_not cond", "[deferred_not]") {
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

TEST_CASE("deferred_not bool arithmetic", "[deferred_not]") {
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
        SECTION("log andnot") {
            B expect = andnot(ba, bb);
            REQUIRE(all(andnot(ba, nbb) == expect));
            REQUIRE(all(andnot(nba, bb) == expect));
            REQUIRE(all(andnot(nba, nbb) == expect));
        }
    }
}

TEST_CASE("deferred_not uint arithmetic", "[deferred_not]") {
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
        SECTION("bit andnot") {
            U expect = andnot(ua, ub);
            REQUIRE(all(andnot(ua, nub) == expect));
            REQUIRE(all(andnot(nua, ub) == expect));
            REQUIRE(all(andnot(nua, nub) == expect));
        }
    }
}

TEST_CASE("deferred_not methods", "[deferred_not]") {
    const B ba = !dataAB;
    const not_B nba(dataAB);
    SECTION("eval") {
        auto res_ua = ba.eval();
        auto res_nua = nba.eval();
        REQUIRE((std::is_same<decltype(res_ua), decltype(res_nua)>::value));
        REQUIRE(all(res_ua == res_nua));
    }
    SECTION("data") {
        REQUIRE(all(B(ba.data()) == B(nba.data())));
    }
    SECTION("store") {
        SECTION("aligned") {
            B::storage_t res_ua{}, res_nua{};
            ba.aligned_store(res_ua.data());
            nba.aligned_store(res_nua.data());
            REQUIRE((res_ua == res_nua));
        }
        SECTION("unaligned") {
            B::storage_t res_ua{}, res_nua{};
            ba.unaligned_store(res_ua.data());
            nba.unaligned_store(res_nua.data());
            REQUIRE((res_ua == res_nua));
        }
        SECTION("interleaved") {
            B::storage_t res_ua[3]{}, res_nua[3]{};
            ba.interleaved_store(res_ua[0].data(), 3);
            nba.interleaved_store(res_nua[0].data(), 3);
            REQUIRE((res_ua[0] == res_nua[0]));
            REQUIRE((res_ua[1] == res_nua[1]));
            REQUIRE((res_ua[2] == res_nua[2]));
        }
    }
    SECTION("reduce") {
        auto res_ba = reduce(ba, [](B a, B b) { return a && b; }).first_scalar();
        auto res_nba = reduce(nba, [](B a, B b) { return a && b; }).first_scalar();
        REQUIRE(res_ba == res_nba);
    }
    SECTION("mask") {
        auto res_ba = ba.mask();
        auto res_nba = nba.mask();
        REQUIRE(res_ba == res_nba);
    }
    SECTION("first_scalar") {
        auto res_ba = ba.first_scalar();
        auto res_nba = nba.first_scalar();
        REQUIRE(res_ba == res_nba);
    }
}
