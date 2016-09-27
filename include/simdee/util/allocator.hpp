// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_MALLOC_HPP
#define SIMDEE_UTIL_MALLOC_HPP

#include <type_traits>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include "noexcept.hpp"

namespace sd {

    namespace detail {
#if defined(__GLIBCXX__) && (__GLIBCXX__ < 20150422 || __GLIBCXX__ == 20150623 || __GLIBCXX__ == 20150626 || __GLIBCXX__ == 20160803)
        // type_traits is buggered in libstdc++ up until GCC 5
        template <typename T>
        using is_trivially_default_constructible = std::has_trivial_default_constructor<T>;
#else
        template <typename T>
        using is_trivially_default_constructible = std::is_trivially_default_constructible<T>;
#endif

        template <std::size_t x>
        using is_power_of_2 = std::integral_constant<bool, x && (x & (x - 1)) == 0>;
    }

    template <typename T, std::size_t Align>
    T* aligned_malloc(std::size_t count)
    {
        static_assert(detail::is_power_of_2<Align>::value, "alignment must be a power of 2");
        auto size = (sizeof(T) * count) + sizeof(void*) + Align - 1;
        auto orig = std::malloc(size);
        if (orig == nullptr) return nullptr;
        auto aligned = reinterpret_cast<T*>((reinterpret_cast<std::uintptr_t>(orig) + sizeof(void*) + Align - 1) & ~(Align - 1));
        reinterpret_cast<void**>(aligned)[-1] = orig; // save orig right before the start of user data
        return aligned;
    }

    template <typename T>
    void aligned_free(T* aligned)
    {
        if (aligned == nullptr) return;
        auto orig = reinterpret_cast<void**>(aligned)[-1]; // restore orig from right before the start of user data
        std::free(orig);
    }

    struct aligned_deleter {
        template <typename T>
        void operator()(T* ptr) {
            sd::aligned_free(ptr);
        }
    };

    // aligned allocator
    template <typename T, std::size_t Align>
    struct aligned_allocator {
        using value_type = T;

        aligned_allocator() = default;
        template <typename S>
        aligned_allocator(const aligned_allocator<S, Align>&) {}

        T* allocate(std::size_t count) const SIMDEE_NOEXCEPT {
            return sd::aligned_malloc<T, Align>(count);
        }

        void deallocate(T* ptr, std::size_t) const SIMDEE_NOEXCEPT {
            sd::aligned_free(ptr);
        }

        // destroy is a no-op if possible
        void destroy(T* ptr) const SIMDEE_NOEXCEPT_IF(std::is_nothrow_destructible<T>::value) {
            if (!std::is_trivially_destructible<T>::value) {
                ptr->~T();
            }
        }

        // construct without arguments is a no-op if possible
        void construct(T* ptr) const SIMDEE_NOEXCEPT_IF(std::is_nothrow_constructible<T>::value) {
            if (!detail::is_trivially_default_constructible<T>::value) {
                new (ptr)T;
            }
        }

        // construct with arguments forwarded to placement new
        template <typename A1, typename... A>
        void construct(T* ptr, A1&& a1, A&&... a2) const {
            new (ptr)T(std::forward<A1>(a1), std::forward<A...>(a2)...);
        }

        // default rebind should do just this, doesn't seem to work in MSVC though
        template <typename S>
        struct rebind {
            using other = aligned_allocator<S, Align>;
        };
    };

    // allocator equality
    template <typename T, typename U, std::size_t TS, std::size_t US>
    bool operator==(const aligned_allocator<T, TS>&, const aligned_allocator<U, US>&) { return true; }
    template <typename T, typename U, std::size_t TS, std::size_t US>
    bool operator!=(const aligned_allocator<T, TS>&, const aligned_allocator<U, US>&) { return false; }

}

#endif // SIMDEE_UTIL_MALLOC_HPP
