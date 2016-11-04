// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_INIT_HPP
#define SIMDEE_COMMON_INIT_HPP

#define SIMDEE
#define SIMDEE_MAJOR 0
#define SIMDEE_MINOR 2
#define SIMDEE_PATCHLEVEL 0

//
// C++, C++11 checks
//
#ifndef __cplusplus
#   error "Simdee requires C++."
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
#   error "Simdee requires Visual Studio 2015."
#endif

#if !defined(_MSC_VER) && __cplusplus < 201103L && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#   error "Simdee requires C++11. Please use the setting '-std=c++0x' or '-std=c++11'."
#endif

//
// defaults for feature requests
//
#ifndef SIMDEE_NEED_INT
#   define SIMDEE_NEED_INT 1 // require integer operations for SIMD types
#endif

//
// fix MSVC borked SSE macros (ICC has got them right though)
//
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#   if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
#       define __SSE__
#   endif
#   if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#       define __SSE2__
#   endif
#   if defined(__AVX__)
#       define __SSE3__
#       define __SSSE3__
#       define __SSE4_1__
#       define __SSE4_2__
#   endif
#endif

#endif // SIMDEE_COMMON_INIT_HPP
