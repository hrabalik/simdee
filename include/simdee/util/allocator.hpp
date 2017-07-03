// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_MALLOC_HPP
#define SIMDEE_UTIL_MALLOC_HPP

#include <type_traits>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <memory>
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

        inline constexpr bool is_pow2(std::size_t x) {
            return x && (x & (x - 1)) == 0;
        }

        template <typename T, std::size_t Align>
        struct aligned_allocator;
        template <typename T, std::size_t Align = alignof(T), bool Switch = (Align > alignof(double))>
        struct alloc;

        template <typename T, std::size_t Align>
        struct alloc<T, Align, false> {
            static T* malloc(std::size_t bytes) {
                return (T*)std::malloc(bytes);
            }
            static void free(T* ptr) {
                std::free(ptr);
            }
            using allocator = std::allocator<T>;
            using deleter = std::default_delete<T>;
        };

        template <typename T, std::size_t Align>
        struct alloc<T, Align, true> {
            static_assert(detail::is_pow2(Align), "alignment must be a power of 2");
            static_assert(Align <= 128, "alignment is too large");
            static_assert(Align > alignof(double), "alignment is too small -- use malloc");

            static T* malloc(std::size_t bytes) {
                auto orig = (uintptr_t)std::malloc(bytes + Align);
                if (orig == 0) return nullptr;
                auto aligned = (orig + Align) & ~(Align - 1);
                auto offset = int8_t(orig - aligned);
                ((int8_t*)aligned)[-1] = offset;
                return (T*)aligned;
            }

            static void free(T* aligned) {
                if (aligned == nullptr) return;
                auto offset = ((int8_t*)aligned)[-1];
                auto orig = uintptr_t(aligned) + offset;
                std::free((void*)orig);
            }

            using allocator = aligned_allocator<T, Align>;

            struct deleter {
                template <typename S>
                void operator()(S* ptr) {
                    free(ptr);
                }
            };
        };

        template <typename T, std::size_t Align>
        struct aligned_allocator {
            using value_type = T;
            using alloc_t = alloc<T, Align>;

            aligned_allocator() = default;

            template <typename S>
            aligned_allocator(const aligned_allocator<S, Align>&) {}

            T* allocate(std::size_t count) const SIMDEE_NOEXCEPT {
                return alloc_t::malloc(sizeof(T) * count);
            }

            void deallocate(T* ptr, std::size_t) const SIMDEE_NOEXCEPT {
                alloc_t::free(ptr);
            }

            void destroy(T* ptr) const SIMDEE_NOEXCEPT_IF(std::is_nothrow_destructible<T>::value) {
                if (!std::is_trivially_destructible<T>::value) {
                    ptr->~T();
                }
                else {
                    no_op(ptr); // just to suppress MSVC warning "ptr not referenced"
                }
            }

            static void no_op(T*) {}

            void construct(T* ptr) const SIMDEE_NOEXCEPT_IF(std::is_nothrow_constructible<T>::value) {
                if (!is_trivially_default_constructible<T>::value) {
                    new (ptr)T;
                }
            }

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

        template <typename T, typename U, std::size_t TS, std::size_t US>
        inline bool operator==(const aligned_allocator<T, TS>&, const aligned_allocator<U, US>&) { return true; }
        template <typename T, typename U, std::size_t TS, std::size_t US>
        inline bool operator!=(const aligned_allocator<T, TS>&, const aligned_allocator<U, US>&) { return false; }
    }

    template <typename T, std::size_t Align = alignof(T)>
    using allocator = typename detail::alloc<T, Align>::allocator;
    template <typename T, std::size_t Align = alignof(T)>
    using deleter = typename detail::alloc<T, Align>::deleter;
}

#endif // SIMDEE_UTIL_MALLOC_HPP
