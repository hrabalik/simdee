#ifndef SIMDIFY_UTIL_1B
#define SIMDIFY_UTIL_1B

#include "inline.hpp"

#if defined(__GNUC__) // GCC, Clang

namespace simd {
	// least significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE int ls1b(unsigned int in) { return __builtin_ctz(in); }
	// most significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE int ms1b(unsigned int in) { return (8*sizeof(unsigned int) - 1) - __builtin_clz(in); }
	// least significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE int ls1b(unsigned long in) { return __builtin_ctzl(in); }
	// most significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE int ms1b(unsigned long in) { return (8*sizeof(unsigned long) - 1) - __builtin_clzl(in); }
	// least significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE int ls1b(unsigned long long in) { return __builtin_ctzll(in); }
	// most significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE int ms1b(unsigned long long in) { return (8*sizeof(unsigned long long) - 1) - __builtin_clzll(in); }
}

#elif defined(_MSC_VER) // Visual Studio

#include <intrin.h>

namespace simd {
	// least significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE unsigned long ls1b(unsigned long in) { unsigned long res; _BitScanForward(&res, in); return res; }
	// most significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE unsigned long ms1b(unsigned long in) { unsigned long res; _BitScanReverse(&res, in); return res; }
}

#if defined(_M_X64)
	// least significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE unsigned long ls1b(unsigned __int64 in) { unsigned long res; _BitScanForward64(&res, in); return res; }
	// most significant "1" bit index; result undefined if in == 0
	SIMDIFY_FORCE_INLINE unsigned long ms1b(unsigned __int64 in) { unsigned long res; _BitScanReverse64(&res, in); return res; }
#endif

#else

#error "util_1b.h: incompatible compiler"

#endif

#endif // SIMDIFY_UTIL_1B
