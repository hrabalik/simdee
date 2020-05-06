// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_MALLOC_HPP
#define SIMDEE_UTIL_MALLOC_HPP

#include "inline.hpp"
#include "noexcept.hpp"
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <memory>
#include <type_traits>

namespace sd {

    namespace detail {

#if defined(__GLIBCXX__) && (__GLIBCXX__ < 20150422 || __GLIBCXX__ == 20150623 ||                  \
                             __GLIBCXX__ == 20150626 || __GLIBCXX__ == 20160803)
        // type_traits is buggered in libstdc++ up until GCC 5
        template <typename T>
        using is_trivially_default_constructible = std::has_trivial_default_constructor<T>;
#else
        template <typename T>
        using is_trivially_default_constructible = std::is_trivially_default_constructible<T>;
#endif

        inline constexpr bool is_pow2(std::size_t x) { return x && (x & (x - 1)) == 0; }

        template <typename T, std::size_t Align = alignof(T),
                  bool Switch = (Align > alignof(double))>
        struct alloc;

        template <typename T, std::size_t Align>
        struct alloc<T, Align, false> {
            static T* malloc(std::size_t bytes) { return static_cast<T*>(std::malloc(bytes)); }
            static void free(T* ptr) { std::free(ptr); }
        };

        template <typename T, std::size_t Align>
        struct alloc<T, Align, true> {
            static_assert(detail::is_pow2(Align), "alignment must be a power of 2");
            static_assert(Align <= 128, "alignment is too large");
            static_assert(Align > alignof(double), "alignment is too small -- use malloc");

            SIMDEE_INL static T* malloc(std::size_t bytes) {
                auto orig = uintptr_t(std::malloc(bytes + Align));
                if (orig == 0) return nullptr;
                auto aligned = (orig + Align) & ~(Align - 1);
                auto offset = uint8_t(aligned - orig);
                (reinterpret_cast<uint8_t*>(aligned))[-1] = offset;
                return reinterpret_cast<T*>(aligned);
            }

            SIMDEE_INL static void free(T* aligned) {
                if (aligned == nullptr) return;
                auto offset = (reinterpret_cast<uint8_t*>(aligned))[-1];
                auto orig = uintptr_t(aligned) - offset;
                std::free(reinterpret_cast<void*>(orig));
            }
        };

    } // namespace detail

    template <typename T>
    struct allocator {
        static_assert(detail::is_trivially_default_constructible<T>::value,
                      "sd::allocator is for trivially constructible types only");
        static_assert(std::is_trivially_destructible<T>::value,
                      "sd::allocator is for trivially destructible types only");

        using value_type = T;
        using alloc_t = detail::alloc<T>;

        allocator() = default;

        template <typename S>
        allocator(const allocator<S>&) {}

        SIMDEE_INL T* allocate(std::size_t count) const SIMDEE_NOEXCEPT {
            return alloc_t::malloc(sizeof(T) * count);
        }

        SIMDEE_INL void deallocate(T* ptr, std::size_t) const SIMDEE_NOEXCEPT {
            alloc_t::free(ptr);
        }

        SIMDEE_INL void destroy(T*) const SIMDEE_NOEXCEPT {}

        SIMDEE_INL void construct(T*) const SIMDEE_NOEXCEPT {}

        // default rebind should do just this, doesn't seem to work in MSVC though
        template <typename S>
        struct rebind {
            using other = allocator<S>;
        };
    };

    template <typename T, typename U>
    inline bool operator==(const allocator<T>&, const allocator<U>&) {
        return true;
    }
    template <typename T, typename U>
    inline bool operator!=(const allocator<T>&, const allocator<U>&) {
        return false;
    }

    template <typename T>
    struct deleter {
        template <typename S>
        SIMDEE_INL void operator()(S* ptr) {
            detail::alloc<T>::free(ptr);
        }
    };

} // namespace sd

#endif // SIMDEE_UTIL_MALLOC_HPP
