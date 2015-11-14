#include <simdify/containers.hpp>

#include "catch.hpp"
#include <algorithm>

using T = simd::structure_of_arrays<simd::sse, simd::id::x, simd::id::y, simd::id::z>;

TEST_CASE("structure_of_arrays construction", "[containers][structure_of_arrays]") {
    SECTION("default") {
        T t;
        REQUIRE(t.size() == 0);
        REQUIRE(t.capacity() == 0);
    }
    SECTION("with size") {
        T t(13);
        REQUIRE(t.size() == 13);
        REQUIRE(t.capacity() >= 13);
    }
    SECTION("with size and value") {
        T t(13, std::make_tuple(11.f, 22.f, 33.f));
        bool win = std::all_of(t.cbegin(), t.cend(), [](T::const_reference ref) {
            return ref.x == 11.f && ref.y == 22.f && ref.z == 33.f;
        });
        REQUIRE(win);
        bool win2 = std::all_of(t.cbegin_head(), t.cend_head(), [](T::const_reference_vector ref) {
            return simd::sse::horizontal::all(simd::sse(ref.x) == 11.f & simd::sse(ref.y) == 22.f & simd::sse(ref.z) == 33.f);
        });
        REQUIRE(win2);
    }
}

TEST_CASE("structure_of_arrays dynamic size", "[containers][structure_of_arrays]") {
    T t;

    SECTION("reserve") {
        t.reserve(13);
        REQUIRE(t.size() == 0);
        REQUIRE(t.capacity() >= 13);
        auto cap = t.capacity();

        t.reserve(0);
        REQUIRE(t.capacity() == cap);

        t.reserve(cap + 1);
        REQUIRE(t.capacity() > cap);
    }
}
