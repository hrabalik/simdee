// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_MALLOC_HPP
#define SIMDEE_UTIL_MALLOC_HPP

#include "inline.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <memory>
#include <type_traits>

namespace sd {

    namespace detail {

        inline constexpr bool is_pow2(std::size_t x) { return x && (x & (x - 1)) == 0; }

        template <typename T, std::size_t Align = alignof(T),
                  bool Switch = (Align > alignof(std::max_align_t))>
        struct alloc;

        template <typename T, std::size_t Align>
        struct alloc<T, Align, false> {
            SIMDEE_INL static T* malloc(std::size_t bytes) noexcept {
                return static_cast<T*>(std::malloc(bytes));
            }
            SIMDEE_INL static void free(T* ptr) noexcept { std::free(ptr); }
        };

        template <typename T, std::size_t Align>
        struct alloc<T, Align, true> {
            static_assert(detail::is_pow2(Align), "alignment must be a power of 2");
            static_assert(Align <= 128, "alignment is too large");
            static_assert(Align > alignof(std::max_align_t),
                          "alignment is too small -- use malloc");

            SIMDEE_INL static T* malloc(std::size_t bytes) noexcept {
                auto orig = uintptr_t(std::malloc(bytes + Align));
                if (orig == 0) return nullptr;
                auto aligned = (orig + Align) & ~(Align - 1);
                auto offset = uint8_t(aligned - orig);
                (reinterpret_cast<uint8_t*>(aligned))[-1] = offset;
                return reinterpret_cast<T*>(aligned);
            }

            SIMDEE_INL static void free(T* aligned) noexcept {
                if (aligned == nullptr) return;
                auto offset = (reinterpret_cast<uint8_t*>(aligned))[-1];
                auto orig = uintptr_t(aligned) - offset;
                std::free(reinterpret_cast<void*>(orig));
            }
        };

    } // namespace detail

    template <typename T>
    class allocator {
    public:
        using value_type = T;

        allocator() = default;

        template <typename S>
        allocator(const allocator<S>&) {}

        SIMDEE_INL T* allocate(std::size_t count) const {
            T* res = detail::alloc<T>::malloc(sizeof(T) * count);
            if (!res) { throw std::bad_alloc{}; }
            return res;
        }

        SIMDEE_INL void deallocate(T* ptr, std::size_t) const noexcept {
            detail::alloc<T>::free(ptr);
        }
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
