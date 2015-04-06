#ifndef SIMDIFY_UTIL_MALLOC
#define SIMDIFY_UTIL_MALLOC

#include <type_traits>

namespace simd {

	// malloc-like aligned memory allocation, no constructors are called
	template <typename T>
	inline void aligned_malloc(T*& arr, std::size_t count = 1, std::size_t align = __alignof(T))
	{
		arr = (T*)_aligned_malloc(count * sizeof T, align);
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

		if (!std::is_trivially_default_constructible<T>::value && arr != nullptr) {
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
	}

}

#endif // SIMDIFY_UTIL_MALLOC
