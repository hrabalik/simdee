#include "catch.hpp"
#include <simdee/common/storage.hpp>

struct vec_t {
    using scalar_t = char;
    enum { width = 3 };

    void aligned_store(scalar_t* ptr) const {
        for (int i = 0; i < width; i++) {
            ptr[i] = data[i];
        }
    }

    scalar_t data[width];
};

using storage_t = sd::impl::storage<vec_t, vec_t::scalar_t, alignof(vec_t)>;

TEST_CASE("storage data access") {
    SECTION("via operator[]") {
        storage_t s;
        s[0] = 'x'; s[1] = 'y'; s[2] = 'z';
        REQUIRE((s[0] == 'x' && s[1] == 'y' && s[2] == 'z'));
    }
    SECTION("via data()") {
        storage_t s;
        auto d = s.data();
        d[0] = 'u'; d[1] = 'v'; d[2] = 'w';
        REQUIRE((s[0] == 'u' && s[1] == 'v' && s[2] == 'w'));
    }
}

TEST_CASE("storage construction and assigment", "[storage]") {
    auto good = [](const storage_t& s) {
        return s[0] == 'a' && s[1] == 'b' && s[2] == 'c';
    };
    auto implicit_test = [&good](storage_t s) {
        return good(s);
    };

    SECTION("copy") {
        storage_t s;
        s[0] = 'a'; s[1] = 'b'; s[2] = 'c';
        REQUIRE(implicit_test(s));
        storage_t assigned_test;
        assigned_test = s;
        REQUIRE(good(assigned_test));
    }
    SECTION("from vec_t") {
        vec_t v = { { 'a', 'b', 'c' } };
        storage_t explicit_test(v);
        REQUIRE(good(explicit_test));
        storage_t assigned_test;
        assigned_test = v;
        REQUIRE(good(assigned_test));
    }
    SECTION("from init list") {
        REQUIRE(implicit_test({'a', 'b', 'c'}));
        storage_t assigned_test;
        assigned_test = { 'a', 'b', 'c' };
        REQUIRE(good(assigned_test));
    }
}

TEST_CASE("storage comparison", "[storage]") {
    storage_t s1 = { 'd', 'e', 'f' };
    storage_t s2 = { 'd', 'e', 'f' };
    storage_t s3 = { 'd', 'e', 'g' };
    storage_t s4 = { 'd', 'f', 'e' };

    REQUIRE(s1 == s1);
    REQUIRE(s1 == s2);
    REQUIRE(s1 != s3);
    REQUIRE(s1 != s4);
    REQUIRE(s2 == s2);
    REQUIRE(s2 != s3);
    REQUIRE(s2 != s4);
    REQUIRE(s3 == s3);
    REQUIRE(s3 != s4);
    REQUIRE(s4 == s4);

    REQUIRE(s1 <= s1);
    REQUIRE(s1 >= s2);
    REQUIRE(s1 < s3);
    REQUIRE(s4 > s1);
    REQUIRE(s2 <= s3);
    REQUIRE(s4 >= s2);
    REQUIRE(!(s1 > s1));
    REQUIRE(!(s1 < s2));
    REQUIRE(!(s1 >= s3));
    REQUIRE(!(s4 <= s1));
    REQUIRE(!(s2 > s3));
    REQUIRE(!(s4 < s2));
}
