#ifndef SIMDIFY_UTIL_TRIVIAL_TYPE_HPP
#define SIMDIFY_UTIL_TRIVIAL_TYPE_HPP

#include "inline.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_TRIVIAL_TYPE( CLASS )                                                                    \
                                                                                                         \
SIMDIFY_INL ~CLASS() = default;                                                                          \
                                                                                                         \
SIMDIFY_INL CLASS() = default;                                                                           \
                                                                                                         \
SIMDIFY_INL CLASS(const CLASS&) = default;                                                               \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(const CLASS&) = default;                                                    \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // SIMDIFY_UTIL_TRIVIAL_TYPE_HPP
