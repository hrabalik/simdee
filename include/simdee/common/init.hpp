// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_INIT_HPP
#define SIMDEE_COMMON_INIT_HPP

#define SIMDEE
#define SIMDEE_MAJOR 0
#define SIMDEE_MINOR 4
#define SIMDEE_PATCHLEVEL 0

//
// C++, C++11 checks
//
#ifndef __cplusplus
#error "Simdee requires C++."
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
#error "Simdee requires Visual Studio 2015."
#endif

#if !defined(_MSC_VER) && __cplusplus < 201103L && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#error "Simdee requires C++11. Please use the setting '-std=c++0x' or '-std=c++11'."
#endif

//
// defaults for feature requests
//
#ifndef SIMDEE_NEED_INT
#define SIMDEE_NEED_INT 1 // require integer arithmetic for SIMD types
#endif

//
// fix borked x86 + SSE in MSVC
//
#if defined(_M_IX86_FP) && _M_IX86_FP >= 1
#if !defined(__SSE__)
#define __SSE__
#endif
#endif

//
// fix borked x86 + SSE2 in MSVC
//
#if defined(_M_IX86_FP) && _M_IX86_FP >= 2
#if !defined(__SSE2__)
#define __SSE2__
#endif
#endif

//
// enforce instruction sets on the AMD64 architecture
//
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) ||          \
    defined(_M_X64) || defined(_M_AMD64)
#if !defined(__SSE2__)
#define __SSE2__
#endif
#endif

//
// enforce instruction sets implied by AVX2
//
#if defined(__AVX2__)
#if !defined(__AVX__)
#define __AVX__
#endif
#endif

//
// enforce instructions sets implied by AVX
//
#if defined(__AVX__)
#if !defined(__SSE2__)
#define __SSE2__
#endif
#if !defined(__SSE3__)
#define __SSE3__
#endif
#if !defined(__SSSE3__)
#define __SSSE3__
#endif
#if !defined(__SSE4_1__)
#define __SSE4_1__
#endif
#if !defined(__SSE4_2__)
#define __SSE4_2__
#endif
#endif

//
// enforce instructions sets implied by SSE2
//
#if defined(__SSE2__)
#if !defined(__SSE__)
#define __SSE__
#endif
#endif

//
// detect instruction sets
//
#if defined(__SSE2__)
#define SIMDEE_SSE2 1
#else
#define SIMDEE_SSE2 0
#endif
#if defined(__SSSE3__)
#define SIMDEE_SSSE3 1
#else
#define SIMDEE_SSSE3 0
#endif
#if defined(__SSE4_1__)
#define SIMDEE_SSE41 1
#else
#define SIMDEE_SSE41 0
#endif
#if defined(__AVX__)
#define SIMDEE_AVX 1
#else
#define SIMDEE_AVX 0
#endif
#if defined(__AVX2__)
#define SIMDEE_AVX2 1
#else
#define SIMDEE_AVX2 0
#endif
#if defined(__ARM_NEON)
#define SIMDEE_NEON 1
#else
#define SIMDEE_NEON 0
#endif
#if defined(__aarch64__) || defined(_M_ARM64)
#define SIMDEE_ARM64 1
#else
#define SIMDEE_ARM64 0
#endif

#endif // SIMDEE_COMMON_INIT_HPP
