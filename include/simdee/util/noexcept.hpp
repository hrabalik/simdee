#ifndef SIMDEE_UTIL_NOEXCEPT_HPP
#define SIMDEE_UTIL_NOEXCEPT_HPP

#if defined(_MSC_VER)

#define SIMDEE_NOEXCEPT throw()
#define SIMDEE_NOEXCEPT_IF(cond)

#else

#define SIMDEE_NOEXCEPT noexcept
#define SIMDEE_NOEXCEPT_IF(cond) noexcept(cond)

#endif

#endif // SIMDEE_UTIL_NOEXCEPT_HPP
