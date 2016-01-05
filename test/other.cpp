#include <simdify/simdify.hpp>
#include <simdify/storage.hpp>

#include "catch.hpp"

TEST_CASE("simd::reference", "[other]") {
    simd::storage<float> f;
    f = 2.3f;
    REQUIRE(f == 2.3f);

    simd::storage<simd::dumf> fl;
    fl = 5.6f;
    REQUIRE(all(fl == 5.6f));

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

    simd::reference<simd::storage<simd::dumf>> flr;
    flr.reset(&fl);
    REQUIRE(all(flr == 5.6f));
    flr = 6.7f;
    REQUIRE(all(fl == 6.7f));
}

TEST_CASE("storage-aos_storage interop", "[other]") {
    simd::aos_storage<simd::sses, 7> as;
    as[0] = 123;
    as[1] = -234;
    as[2] = 345;
    as[3] = -456;
    simd::storage<simd::sses> s = as;
    REQUIRE(s[0] == 123);
    REQUIRE(s[1] == -234);
    REQUIRE(s[2] == 345);
    REQUIRE(s[3] == -456);
    s[0] = -567;
    s[1] = 678;
    s[2] = -789;
    s[3] = 890;
    as = s;
    REQUIRE(as[0] == -567);
    REQUIRE(as[1] == 678);
    REQUIRE(as[2] == -789);
    REQUIRE(as[3] == 890);
}
