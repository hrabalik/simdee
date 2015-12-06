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
            return simd::sse::horizontal::all(ref.x == 11.f & ref.y == 22.f & ref.z == 33.f);
        });
        REQUIRE(win2);
    }
}

TEST_CASE("structure_of_arrays size management", "[containers][structure_of_arrays]") {
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

    SECTION("resize, clear") {
        t.resize(13, std::make_tuple(11.f, 22.f, 33.f));
        REQUIRE(t.size() == 13);
        REQUIRE(t.size_head() == 12);
        REQUIRE(t.size_tail() == 1);
        REQUIRE(t.size_vector() == 16);
        REQUIRE(t.capacity() >= 13);

        bool win = std::all_of(t.cbegin(), t.cend(), [](T::const_reference ref) {
            return ref.x == 11.f && ref.y == 22.f && ref.z == 33.f;
        });
        REQUIRE(win);

        t.resize(8);
        REQUIRE(t.size() == 8);
        REQUIRE(t.size_head() == 8);
        REQUIRE(t.size_tail() == 0);
        REQUIRE(t.size_vector() == 8);
        REQUIRE(t.capacity() >= 13);

        t.resize(26, std::make_tuple(11.f, 22.f, 33.f));
        REQUIRE(t.size() == 26);
        REQUIRE(t.capacity() >= 26);

        bool win2 = std::all_of(t.cbegin(), t.cend(), [](T::const_reference ref) {
            return ref.x == 11.f && ref.y == 22.f && ref.z == 33.f;
        });
        REQUIRE(win2);

        auto cap = t.capacity();
        t.clear();
        REQUIRE(t.size() == 0);
        REQUIRE(t.capacity() == cap);
    }

    SECTION("push_back, pop_back, back") {
        t.push_back(std::make_tuple(11.f, 22.f, 33.f));
        t.push_back(std::make_tuple(44.f, 55.f, 66.f));
        t.push_back(std::make_tuple(77.f, 88.f, 99.f));

        REQUIRE(t.size() == 3);
        REQUIRE(t.capacity() >= 3);
        REQUIRE(t.at(0).x == 11.f);
        REQUIRE(t.at(1).y == 55.f);
        REQUIRE(t.at(2).z == 99.f);

        auto refBack = t.back();

        REQUIRE(refBack.x == 77.f);
        REQUIRE(refBack.y == 88.f);
        REQUIRE(refBack.z == 99.f);

        refBack = std::make_tuple(12.f, 34.f, 56.f);
        REQUIRE(t.at(2).x == 12.f);
        REQUIRE(t.at(2).y == 34.f);
        REQUIRE(t.at(2).z == 56.f);

        t.pop_back();
        REQUIRE(t.size() == 2);
        t.pop_back();
        REQUIRE(t.size() == 1);
        t.pop_back();
        REQUIRE(t.size() == 0);
    }
}
