#include <catch2/catch.hpp>
#include <memory>
#include <simdee/util/allocator.hpp>
#include <vector>

struct alignas(64) T {
    T() = default;
    T(unsigned int val) : v1(val), v2(val) {}
    T(const T&) = default;
    T& operator=(const T&) = default;
    uint64_t v1, v2;
    char pad[48];
};

TEST_CASE("sd::detail::alloc::malloc/free", "[allocator]") {
    SECTION("alloc T") {
        auto ptr = sd::detail::alloc<T>::malloc(sizeof(T));
        REQUIRE(uintptr_t(ptr) % alignof(T) == 0);
        ptr->v1 = uint64_t(-1);
        ptr->v2 = uint64_t(-1);
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
        for (unsigned int i = 0; i < 123; i++) { vec.push_back(T{i}); }
        for (unsigned int i : {0u, 1u, 10u, 100u, 122u}) { REQUIRE(vec[i].v1 == i); }
    }
    SECTION("vector<double>") {
        std::vector<double, sd::allocator<double>> vec;
        for (unsigned int i = 0; i < 123; i++) { vec.push_back(i); }
        for (unsigned int i : {0u, 1u, 10u, 100u, 122u}) { REQUIRE(vec[i] == i); }
    }
}

TEST_CASE("deleter", "[allocator]") {
    SECTION("unique_ptr<T>") {
        auto obj = sd::detail::alloc<T>::malloc(sizeof(T));
        new (obj) T{42};
        std::unique_ptr<T, sd::deleter<T>> ptr(obj);
        REQUIRE(ptr->v1 == 42);
        REQUIRE(ptr->v2 == 42);
    }
    SECTION("unique_ptr<double>") {
        auto obj = sd::detail::alloc<double>::malloc(sizeof(T));
        new (obj) double;
        std::unique_ptr<double, sd::deleter<double>> ptr(obj);
    }
}
