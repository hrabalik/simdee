// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_MACROS_HPP
#define SIMDEE_UTIL_MACROS_HPP

#include "inline.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_TRIVIAL_TYPE( CLASS )                                                                     \
                                                                                                         \
SIMDEE_INL ~CLASS() = default;                                                                           \
                                                                                                         \
SIMDEE_INL CLASS() = default;                                                                            \
                                                                                                         \
SIMDEE_INL CLASS(const CLASS&) = default;                                                                \
                                                                                                         \
SIMDEE_INL CLASS& operator=(const CLASS&) = default;                                                     \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_BASE_CTOR_COMPLEX( CLASS, ARGTYPE )                                                       \
                                                                                                         \
SIMDEE_INL CLASS (const ARGTYPE & r) {                                                                   \
    operator=(r);                                                                                        \
}                                                                                                        \
                                                                                                         \
Crtp& operator=(const ARGTYPE & r)                                                                       \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // SIMDEE_UTIL_MACROS_HPP
