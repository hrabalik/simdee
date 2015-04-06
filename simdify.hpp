#ifndef SIMDIFY
#define SIMDIFY

//
// C++, C++11 checks
//
#ifndef __cplusplus
#error "Simdify requires C++."
#endif

#if defined(_MSC_VER) && _MSC_VER < 1800
#error "Simdify requires Visual Studio 2013."
#endif

#if !defined(_MSC_VER) && __cplusplus < 201103L && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#error "Simdify requires C++11. Please use the setting '-std=c++0x' or '-std=c++11'."
#endif

//
// fix MSVC borked SSE macros
//
#if defined(_MSC_VER)

#if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP == 2)
#define __SSE2__
#endif

#endif

//
// include all supported SIMD types
//
#if defined(__AVX__)
#include "impl/avx.hpp"
#endif

#if defined(__SSE2__)
#include "impl/sse.hpp"
#endif

#if 1
#include "impl/flt.hpp"
#endif

namespace simd {

//
// set the preferred SIMD type as simd_t
//
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
#warn "Simdify could not determine a suitable SIMD type as simd_t."
#endif

}

#endif // SIMDIFY
