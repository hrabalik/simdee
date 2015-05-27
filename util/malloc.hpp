#ifndef SIMDIFY_UTIL_MALLOC
#define SIMDIFY_UTIL_MALLOC

#include <type_traits>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include "noexcept.hpp"

namespace simd {

    namespace detail {
#ifdef __GLIBCXX__
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
        auto orig = static_cast<void**>(aligned)[-1]; // restore orig from right before the start of user data
        std::free(orig);
    }

	/*// malloc-like aligned memory allocation, no constructors are called
	template <typename T>
	inline void aligned_malloc(T*& arr, std::size_t count = 1, std::size_t align = __alignof(T))
	{
		arr = (T*)_aligned_malloc(count * sizeof(T), align);
	}

	// free-like aligned memory deallocation, no destructors are called
	template <typename T>
	inline void aligned_free(T* arr)
	{
		_aligned_free(arr);
	}

	// new-like aligned memory allocation, constructors are called if necessary
	template <typename T>
	inline void aligned_new(T*& arr, std::size_t count = 1, std::size_t align = __alignof(T))
	{
		aligned_malloc(arr, count, align);

		if (!detail::is_trivially_default_constructible<T>::value && arr != nullptr) {
			auto begin = arr;
			auto end = arr + count;
			for (; begin != end; ++begin) {
				new (begin)T;
			}
		}
	}

	// delete-like aligned memory deallocation, destructors are called if necessary
	template <typename T>
	inline void aligned_delete(T* arr, std::size_t count)
	{
		if (!std::is_trivially_destructible<T>::value && arr != nullptr) {
			auto begin = arr;
			auto end = arr + count;
			for (; begin != end; ++begin) {
				begin->~T();
			}
		}

		aligned_free(arr);
	}*/

    // aligned allocator
    template <typename T, std::size_t Align>
    struct aligned_allocator {
        using value_type = T;

        T* allocate(std::size_t count) const SIMDIFY_NOEXCEPT {
            return simd::aligned_malloc<T, Align>(count);
        }

        void deallocate(T* ptr, std::size_t) const SIMDIFY_NOEXCEPT {
            simd::aligned_free(ptr);
        }

        // destroy is a no-op if possible
        void destroy(T* ptr) const SIMDIFY_NOEXCEPT_IF(std::is_nothrow_destructible<T>::value) {
            if (!std::is_trivially_destructible<T>::value) {
                ptr->~T();
            }
        }

        // construct without arguments is a no-op if possible
        void construct(T* ptr) const SIMDIFY_NOEXCEPT_IF(std::is_nothrow_constructible<T>::value) {
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

    // allocator that throws when used to construct/destroy anything
    template <typename T>
    struct dummy_allocator {
        using value_type = T;

        T* allocate(std::size_t) const { return nullptr; }
        void deallocate(T*, std::size_t) const {}
        void destroy(T*) const { throw std::runtime_error("dummy_allocator::destroy() used"); }

        template <typename... A>
        void construct(T*, A&&...) const { throw std::runtime_error("dummy_allocator::construct() used"); }
    };

    template <typename T, typename U>
    bool operator==(const dummy_allocator<T>&, const dummy_allocator<U>&) { return true; }
    template <typename T, typename U>
    bool operator!=(const dummy_allocator<T>&, const dummy_allocator<U>&) { return false; }

}

#endif // SIMDIFY_UTIL_MALLOC
