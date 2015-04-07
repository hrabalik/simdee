#ifndef SIMDIFY
#define SIMDIFY

//
// C++, C++11 checks
//
#ifndef __cplusplus
#error "Simdify requires C++."
#endif

#if defined(_MSC_VER) && _MSC_VER < 1600
#error "Simdify requires Visual Studio 2010."
#endif

#if !defined(_MSC_VER) && __cplusplus < 201103L && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#error "Simdify requires C++11. Please use the setting '-std=c++0x' or '-std=c++11'."
#endif

//
// fix MSVC borked SSE macros (ICC has got them right though)
//
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)

#if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
#define __SSE__
#endif

#if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define __SSE2__
#endif

#if defined(__AVX__)
#define __SSE3__
#define __SSSE3__
#define __SSE4_1__
#define __SSE4_2__
#endif // __AVX__

#endif // _MSC_VER

//
// include all supported SIMD types
// use the SIMDIFY_NEED_* macros to force the include
//
#if defined(__AVX__) || defined(SIMDIFY_NEED_AVX)
#include "impl/avx.hpp"
#endif

#if defined(__SSE2__) || defined(SIMDIFY_NEED_SSE)
#include "impl/sse.hpp"
#endif

#if 1 || defined(SIMDIFY_NEED_FLT)
#include "impl/flt.hpp"
#endif

//
// set some (successfully loaded) SIMD type as simd_t
// use SIMDIFY_P
//
namespace simd {
#if defined(SIMDIFY_PREFER_AVX) && defined(SIMDIFY_AVX)
using simd_t = avx;
#elif defined(SIMDIFY_PREFER_SSE) && defined(SIMDIFY_SSE)
using simd_t = sse;
#elif defined(SIMDIFY_PREFER_FLT) && defined(SIMDIFY_FLT)
using simd_t = flt;
#elif defined(SIMDIFY_AVX)
using simd_t = avx;
#elif defined(SIMDIFY_SSE)
using simd_t = sse;
#elif defined(SIMDIFY_FLT)
using simd_t = flt;
#else
#error "Simdify could not determine a suitable SIMD type as simd_t."
#endif
}

#endif // SIMDIFY
