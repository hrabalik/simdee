#ifndef SIMDEE_UTIL_INLINE_HPP
#define SIMDEE_UTIL_INLINE_HPP

#if defined(__clang__)

#define SIMDEE_INL __inline__ __attribute__((always_inline, nodebug))

#elif defined(__GNUC__)

#define SIMDEE_INL __inline__ __attribute__((always_inline))

#elif defined(_MSC_VER)

#define SIMDEE_INL __forceinline

#else

#error "util_inline.h: incompatible compiler"

#endif

#endif // SIMDEE_UTIL_INLINE_HPP
