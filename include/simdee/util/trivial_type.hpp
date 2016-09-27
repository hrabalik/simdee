#ifndef SIMDEE_UTIL_TRIVIAL_TYPE_HPP
#define SIMDEE_UTIL_TRIVIAL_TYPE_HPP

#include "inline.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_TRIVIAL_TYPE( CLASS )                                                                    \
                                                                                                         \
SIMDEE_INL ~CLASS() = default;                                                                          \
                                                                                                         \
SIMDEE_INL CLASS() = default;                                                                           \
                                                                                                         \
SIMDEE_INL CLASS(const CLASS&) = default;                                                               \
                                                                                                         \
SIMDEE_INL CLASS& operator=(const CLASS&) = default;                                                    \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // SIMDEE_UTIL_TRIVIAL_TYPE_HPP
