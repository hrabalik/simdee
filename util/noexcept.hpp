#ifndef SIMDIFY_UTIL_NOEXCEPT
#define SIMDIFY_UTIL_NOEXCEPT

#if defined(_MSC_VER)

#define SIMDIFY_NOEXCEPT throw()
#define SIMDIFY_NOEXCEPT_IF(cond)

#else

#define SIMDIFY_NOEXCEPT noexcept
#define SIMDIFY_NOEXCEPT_IF(cond) noexcept(cond)

#endif

#endif
