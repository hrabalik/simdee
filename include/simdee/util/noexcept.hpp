#ifndef SIMDIFY_UTIL_NOEXCEPT_HPP
#define SIMDIFY_UTIL_NOEXCEPT_HPP

#if defined(_MSC_VER)

#define SIMDIFY_NOEXCEPT throw()
#define SIMDIFY_NOEXCEPT_IF(cond)

#else

#define SIMDIFY_NOEXCEPT noexcept
#define SIMDIFY_NOEXCEPT_IF(cond) noexcept(cond)

#endif

#endif // SIMDIFY_UTIL_NOEXCEPT_HPP
