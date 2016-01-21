#define SIMDIFY_NEED_SSE
#include <simdify/containers.hpp>

#include "catch.hpp"
#include <algorithm>

using T = simd::array_of_vectors<simd::ssef, simd::id::x, simd::id::y, simd::id::z>;

TEST_CASE("array_of_vectors construction", "[containers][array_of_vectors]") {
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
        bool win2 = std::all_of(t.cbegin_body(), t.cend_body(), [](T::const_reference_vector ref) {
            return (ref.x == 11.f & ref.y == 22.f & ref.z == 33.f).all();
        });
        REQUIRE(win2);
    }
}

TEST_CASE("array_of_vectors size management", "[containers][array_of_vectors]") {
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
        REQUIRE(t.size_body() == 12);
        REQUIRE(t.size_tail() == 1);
        REQUIRE(t.size_overspan() == 16);
        REQUIRE(t.capacity() >= 13);

        bool win = std::all_of(t.cbegin(), t.cend(), [](T::const_reference ref) {
            return ref.x == 11.f && ref.y == 22.f && ref.z == 33.f;
        });
        REQUIRE(win);

        t.resize(8);
        REQUIRE(t.size() == 8);
        REQUIRE(t.size_body() == 8);
        REQUIRE(t.size_tail() == 0);
        REQUIRE(t.size_overspan() == 8);
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

        T::reference refBack = t.back();

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

TEST_CASE("array_of_vectors element access", "[containers][array_of_vectors]") {
    T t;
    t.reserve(6);
    t.push_back(std::make_tuple(11.f, 12.f, 13.f));
    t.push_back(std::make_tuple(21.f, 22.f, 23.f));
    t.push_back(std::make_tuple(31.f, 32.f, 33.f));
    t.push_back(std::make_tuple(41.f, 42.f, 43.f));
    t.push_back(std::make_tuple(51.f, 52.f, 53.f));
    t.push_back(std::make_tuple(61.f, 62.f, 63.f));

    SECTION("using values to access values") {
        auto el1 = t[3];
        REQUIRE(el1.x == 41.f);
        REQUIRE(el1.y == 42.f);
        REQUIRE(el1.z == 43.f);
        auto el2 = t[5];
        REQUIRE(el2.x == 61.f);
        REQUIRE(el2.y == 62.f);
        REQUIRE(el2.z == 63.f);
        el1 = std::make_tuple(71.f, 72.f, 73.f);
    }

    SECTION("getting values via operator []") {
        T::value_type el1 = t[3];
        el1.x = 71.f;
        el1.y = 72.f;
        el1.z = 73.f;
        T::value_type el2 = t[3];
        REQUIRE(el2.x == 41.f);
        REQUIRE(el2.y == 42.f);
        REQUIRE(el2.z == 43.f);
    }

    SECTION("getting references to values via operator []") {
        T::reference el1 = t[3];
        el1.x = 71.f;
        el1.y = 72.f;
        el1.z = 73.f;
        T::reference el2 = t[3];
        REQUIRE(el2.x == 71.f);
        REQUIRE(el2.y == 72.f);
        REQUIRE(el2.z == 73.f);
    }

    SECTION("getting values via iterator") {
        T::value_type el1 = *t.begin();
        el1.x = 71.f;
        el1.y = 72.f;
        el1.z = 73.f;
        T::value_type el2 = *t.begin();
        REQUIRE(el2.x == 11.f);
        REQUIRE(el2.y == 12.f);
        REQUIRE(el2.z == 13.f);
    }

    SECTION("getting references to values via iterator") {
        T::reference el1 = *t.begin();
        el1.x = 71.f;
        el1.y = 72.f;
        el1.z = 73.f;
        T::reference el2 = *t.begin();
        REQUIRE(el2.x == 71.f);
        REQUIRE(el2.y == 72.f);
        REQUIRE(el2.z == 73.f);
    }

    SECTION("getting vectors via iterator") {
        T::value_type_vector el1 = *t.begin_overspan();
        el1.x = 71.f;
        el1.y = 72.f;
        el1.z = 73.f;
        T::value_type_vector el2 = *t.begin_overspan();
        REQUIRE(!(el2.x == 71.f).any());
        REQUIRE(!(el2.y == 72.f).any());
        REQUIRE(!(el2.z == 73.f).any());
    }

    SECTION("getting references to vectors via iterator") {
        T::reference_vector el1 = *t.begin_overspan();
        el1.x = 71.f;
        el1.y = 72.f;
        el1.z = 73.f;
        T::reference_vector el2 = *t.begin_overspan();
        REQUIRE((el2.x == 71.f).all());
        REQUIRE((el2.y == 72.f).all());
        REQUIRE((el2.z == 73.f).all());
    }

    SECTION("using vectors to access vectors") {
        simd::storage<simd::sseu> stor;
        stor[0] = 3;
        stor[1] = 2;
        stor[2] = 1;
        stor[3] = 0;
        auto na = t[stor];
        simd::storage<simd::ssef> storf;
        storf = na.x;
        REQUIRE(storf[0] == 41.f);
        REQUIRE(storf[1] == 31.f);
        REQUIRE(storf[2] == 21.f);
        REQUIRE(storf[3] == 11.f);
        storf = na.y;
        REQUIRE(storf[0] == 42.f);
        REQUIRE(storf[1] == 32.f);
        REQUIRE(storf[2] == 22.f);
        REQUIRE(storf[3] == 12.f);
        storf = na.z;
        REQUIRE(storf[0] == 43.f);
        REQUIRE(storf[1] == 33.f);
        REQUIRE(storf[2] == 23.f);
        REQUIRE(storf[3] == 13.f);
    }
}

TEST_CASE("array_of_vectors iteration", "[containers][array_of_vectors]") {
    T t;
    t.push_back(std::make_tuple(1.f, 2.f, 3.f));
    t.push_back(std::make_tuple(2.f, 3.f, 4.f));
    t.push_back(std::make_tuple(3.f, 4.f, 5.f));
    t.push_back(std::make_tuple(4.f, 5.f, 6.f));
    t.push_back(std::make_tuple(5.f, 6.f, 7.f));
    t.push_back(std::make_tuple(6.f, 7.f, 8.f));

    SECTION("standard begin/end iteration") {
        auto begin = t.begin();
        auto end = t.end();

        REQUIRE((begin != end && begin->x == 1.f && begin->y == 2.f && begin->z == 3.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 2.f && begin->y == 3.f && begin->z == 4.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 3.f && begin->y == 4.f && begin->z == 5.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 4.f && begin->y == 5.f && begin->z == 6.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 5.f && begin->y == 6.f && begin->z == 7.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 6.f && begin->y == 7.f && begin->z == 8.f));
        ++begin;
        REQUIRE(begin == end);

        auto first = t.begin();
        auto other = t.end() - 1;
        std::iter_swap(first, other);
        REQUIRE((first->x == 6.f && first->y == 7.f && first->z == 8.f));
        REQUIRE((other->x == 1.f && other->y == 2.f && other->z == 3.f));
    }

    SECTION("overspan iteration") {
        auto begin = t.begin_overspan();
        auto end = t.end_overspan();
        simd::storage<simd::ssef> stor;

        REQUIRE(begin != end);
        stor = begin->x;
        REQUIRE((stor[0] == 1.f && stor[1] == 2.f && stor[2] == 3.f && stor[3] == 4.f));
        stor = begin->y;
        REQUIRE((stor[0] == 2.f && stor[1] == 3.f && stor[2] == 4.f && stor[3] == 5.f));
        stor = begin->z;
        REQUIRE((stor[0] == 3.f && stor[1] == 4.f && stor[2] == 5.f && stor[3] == 6.f));

        ++begin;

        REQUIRE(begin != end);
        stor = begin->x;
        REQUIRE((stor[0] == 5.f && stor[1] == 6.f));
        stor = begin->y;
        REQUIRE((stor[0] == 6.f && stor[1] == 7.f));
        stor = begin->z;
        REQUIRE((stor[0] == 7.f && stor[1] == 8.f));

        ++begin;

        REQUIRE(begin == end);

        auto first = t.begin_overspan();
        auto other = t.end_overspan() - 1;
        std::iter_swap(first, other);
        stor = first->x;
        REQUIRE((stor[0] == 5.f && stor[1] == 6.f));
        stor = first->y;
        REQUIRE((stor[0] == 6.f && stor[1] == 7.f));
        stor = first->z;
        REQUIRE((stor[0] == 7.f && stor[1] == 8.f));
        stor = other->x;
        REQUIRE((stor[0] == 1.f && stor[1] == 2.f && stor[2] == 3.f && stor[3] == 4.f));
        stor = other->y;
        REQUIRE((stor[0] == 2.f && stor[1] == 3.f && stor[2] == 4.f && stor[3] == 5.f));
        stor = other->z;
        REQUIRE((stor[0] == 3.f && stor[1] == 4.f && stor[2] == 5.f && stor[3] == 6.f));
    }

    SECTION("body iteration") {
        auto begin = t.begin_body();
        auto end = t.end_body();
        simd::storage<simd::ssef> stor;

        REQUIRE(begin != end);
        stor = begin->x;
        REQUIRE((stor[0] == 1.f && stor[1] == 2.f && stor[2] == 3.f && stor[3] == 4.f));
        stor = begin->y;
        REQUIRE((stor[0] == 2.f && stor[1] == 3.f && stor[2] == 4.f && stor[3] == 5.f));
        stor = begin->z;
        REQUIRE((stor[0] == 3.f && stor[1] == 4.f && stor[2] == 5.f && stor[3] == 6.f));

        ++begin;

        REQUIRE(begin == end);
    }

    SECTION("tail iteration") {
        auto begin = t.begin_tail();
        auto end = t.end_tail();

        REQUIRE((begin != end && begin->x == 5.f && begin->y == 6.f && begin->z == 7.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 6.f && begin->y == 7.f && begin->z == 8.f));
        ++begin;
        REQUIRE(begin == end);
    }

    SECTION("pointer arithmetic") {
        auto begin = t.begin();
        auto end = t.end();

        REQUIRE(end - begin == 6);
        REQUIRE(begin <= end);
        REQUIRE(begin < end);
        REQUIRE(end > begin);
        REQUIRE(end >= begin);
        begin += 3;
        REQUIRE(end - begin == 3);
        REQUIRE(begin - end == -3);
        REQUIRE((begin != end && begin->x == 4.f && begin->y == 5.f && begin->z == 6.f));
        begin = begin + 3;
        REQUIRE(end - begin == 0);
        REQUIRE(begin - end == 0);
        REQUIRE(begin == end);
        REQUIRE(begin >= end);
        REQUIRE(!(begin > end));
        begin += -1;
        REQUIRE((begin != end && begin->x == 6.f && begin->y == 7.f && begin->z == 8.f));
        begin -= 2;
        REQUIRE((begin != end && begin->x == 4.f && begin->y == 5.f && begin->z == 6.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 5.f && begin->y == 6.f && begin->z == 7.f));
        --begin;
        REQUIRE((begin != end && begin->x == 4.f && begin->y == 5.f && begin->z == 6.f));
    }

    SECTION("sort") {
        std::sort(t.begin(), t.end(), [](T::reference lhs, T::reference rhs) {
            return lhs.x > rhs.x;
        });

        auto begin = t.begin();
        auto end = t.end();

        REQUIRE((begin != end && begin->x == 6.f && begin->y == 7.f && begin->z == 8.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 5.f && begin->y == 6.f && begin->z == 7.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 4.f && begin->y == 5.f && begin->z == 6.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 3.f && begin->y == 4.f && begin->z == 5.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 2.f && begin->y == 3.f && begin->z == 4.f));
        ++begin;
        REQUIRE((begin != end && begin->x == 1.f && begin->y == 2.f && begin->z == 3.f));
        ++begin;
        REQUIRE(begin == end);
    }
}
