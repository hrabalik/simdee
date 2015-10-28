#ifndef SIMDIFY_UTIL_INLINE
#define SIMDIFY_UTIL_INLINE

#if defined(__clang__)

#define SIMDIFY_FORCE_INLINE __inline__ __attribute__((always_inline, nodebug))

#elif defined(__GNUC__)

#define SIMDIFY_FORCE_INLINE __inline__ __attribute__((always_inline))

#elif defined(_MSC_VER)

#define SIMDIFY_FORCE_INLINE __forceinline

#else

#error "util_inline.h: incompatible compiler"

#endif

#endif // SIMDIFY_UTIL_INLINE
