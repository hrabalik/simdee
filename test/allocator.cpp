#include "catch.hpp"

#include <simdee/util/allocator.hpp>
#include <vector>
#include <memory>

struct alignas(64)T {
    T(int64_t val) { v1 = val; v2 = val; }
    T(const T&) = default;
    T& operator=(const T&) = default;
    int64_t v1, v2;
};

TEST_CASE("sd::detail::alloc::malloc/free", "[allocator]") {
    SECTION("alloc T") {
        auto ptr = sd::detail::alloc<T>::malloc(sizeof(T));
        REQUIRE(uintptr_t(ptr) % alignof(T) == 0);
        ptr->v1 = -1;
        ptr->v2 = -1;
        sd::detail::alloc<T>::free(ptr);
    }
    SECTION("alloc double") {
        auto ptr = sd::detail::alloc<double>::malloc(sizeof(double));
        REQUIRE(uintptr_t(ptr) % alignof(double) == 0);
        *ptr = 1.23;
        sd::detail::alloc<double>::free(ptr);
    }
}

TEST_CASE("allocator", "[allocator]") {
    SECTION("vector<T>") {
        std::vector<T, sd::allocator<T>> vec;
        for (int i = 0; i < 123; i++) {
            vec.push_back(T{i});
        }
        for (int i : {0, 1, 10, 100, 122}) {
            REQUIRE(vec[i].v1 == i);
        }
    }
    SECTION("vector<double>") {
        std::vector<double, sd::allocator<double>> vec;
        for (int i = 0; i < 123; i++) {
            vec.push_back(i);
        }
        for (int i : {0, 1, 10, 100, 122}) {
            REQUIRE(vec[i] == i);
        }
    }
}

TEST_CASE("deleter", "[allocator]") {
    SECTION("unique_ptr<T>") {
        auto obj = sd::detail::alloc<T>::malloc(sizeof(T));
        new (obj)T{42};
        std::unique_ptr<T, sd::deleter<T>> ptr(obj);
        REQUIRE(ptr->v1 == 42);
        REQUIRE(ptr->v2 == 42);
    }
    SECTION("unique_ptr<double>") {
        auto obj = sd::detail::alloc<double>::malloc(sizeof(T));
        new (obj)double;
        std::unique_ptr<double, sd::deleter<double>> ptr(obj);
    }
}
