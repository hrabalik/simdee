#include <simdify/containers.hpp>

#include "catch.hpp"

TEST_CASE("simd::reference", "[other]") {
    simd::storage<float> f;
    f = 2.3f;
    REQUIRE(f == 2.3f);

    simd::storage<simd::flt> fl;
    fl = 5.6f;
    REQUIRE(simd::flt::horizontal::all(fl == 5.6f));

    simd::reference<simd::storage<float>> fr;
    fr.reset(&f);
    REQUIRE(fr == 2.3f);
    fr = 3.4f;
    REQUIRE(f == 3.4f);

    simd::const_reference<simd::storage<float>> fcr;
    fcr.reset(&f);
    REQUIRE(fcr == 3.4f);
    f = 4.5f;
    REQUIRE(fcr == 4.5f);

    simd::reference<simd::storage<simd::flt>> flr;
    flr.reset(&fl);
    REQUIRE(simd::flt::horizontal::all(flr == 5.6f));
    flr = 6.7f;
    REQUIRE(simd::flt::horizontal::all(fl == 6.7f));
}
