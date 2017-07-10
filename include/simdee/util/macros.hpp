// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

// clang-format off

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
#define SIMDEE_CTOR( CLASS, ARGTYPE, IMPL )                                                              \
                                                                                                         \
SIMDEE_INL CLASS (const ARGTYPE & r) {                                                                   \
    IMPL ;                                                                                               \
}                                                                                                        \
                                                                                                         \
SIMDEE_INL CLASS & operator=(const ARGTYPE & r) {                                                        \
    IMPL ;                                                                                               \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_CTOR_FLAG( CLASS, ARGTYPE, IMPL )                                                         \
                                                                                                         \
SIMDEE_INL CLASS (const ARGTYPE &) {                                                                     \
    IMPL ;                                                                                               \
}                                                                                                        \
                                                                                                         \
SIMDEE_INL CLASS & operator=(const ARGTYPE &) {                                                          \
    IMPL ;                                                                                               \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_CTOR_TPL( CLASS, ARGTYPE, IMPL )                                                          \
                                                                                                         \
template <typename T>                                                                                    \
SIMDEE_INL CLASS (const ARGTYPE & r) {                                                                   \
    IMPL ;                                                                                               \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDEE_INL CLASS & operator=(const ARGTYPE & r) {                                                        \
    IMPL ;                                                                                               \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_BASE_CTOR( CLASS, ARGTYPE, IMPL )                                                         \
                                                                                                         \
SIMDEE_INL CLASS (const ARGTYPE & r) {                                                                   \
    IMPL ;                                                                                               \
}                                                                                                        \
                                                                                                         \
SIMDEE_INL Crtp& operator=(const ARGTYPE & r) {                                                          \
    IMPL ;                                                                                               \
    return self();                                                                                       \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_BASE_CTOR_FLAG( CLASS, ARGTYPE, IMPL )                                                    \
                                                                                                         \
SIMDEE_INL CLASS (const ARGTYPE &) {                                                                     \
    IMPL ;                                                                                               \
}                                                                                                        \
                                                                                                         \
SIMDEE_INL Crtp& operator=(const ARGTYPE &) {                                                            \
    IMPL ;                                                                                               \
    return self();                                                                                       \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_BASE_CTOR_TPL( CLASS, ARGTYPE, IMPL )                                                     \
                                                                                                         \
template <typename T>                                                                                    \
SIMDEE_INL CLASS (const ARGTYPE & r) {                                                                   \
    IMPL ;                                                                                               \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDEE_INL Crtp& operator=(const ARGTYPE & r) {                                                          \
    IMPL ;                                                                                               \
    return self();                                                                                       \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_UNOP( ARGTYPE, RESTYPE, NAME, IMPL )                                                      \
                                                                                                         \
SIMDEE_INL friend const RESTYPE NAME (const ARGTYPE & l) {                                               \
    return IMPL ;                                                                                        \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDEE_BINOP( ARGTYPE, RESTYPE, NAME, IMPL )                                                     \
                                                                                                         \
SIMDEE_INL friend const RESTYPE NAME (const ARGTYPE & l, const ARGTYPE & r) {                            \
    return IMPL ;                                                                                        \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // SIMDEE_UTIL_MACROS_HPP
