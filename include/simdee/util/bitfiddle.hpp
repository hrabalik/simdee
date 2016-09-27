// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_UTIL_BITFIDDLE_HPP
#define SIMDEE_UTIL_BITFIDDLE_HPP

#include "inline.hpp"
#include <iterator>
#include <cstdint>
#include <cstring>

#if defined(__GNUC__) // GCC, Clang

namespace sd {
    // least significant "1" bit index; result undefined if in == 0
    SIMDEE_INL int lsb(unsigned int in) { return __builtin_ctz(in); }
    // most significant "1" bit index; result undefined if in == 0
    SIMDEE_INL int msb(unsigned int in) { return (8*int(sizeof(unsigned int)) - 1) - __builtin_clz(in); }
    // least significant "1" bit index; result undefined if in == 0
    SIMDEE_INL int lsb(unsigned long in) { return __builtin_ctzl(in); }
    // most significant "1" bit index; result undefined if in == 0
    SIMDEE_INL int msb(unsigned long in) { return (8*int(sizeof(unsigned long)) - 1) - __builtin_clzl(in); }
    // least significant "1" bit index; result undefined if in == 0
    SIMDEE_INL int lsb(unsigned long long in) { return __builtin_ctzll(in); }
    // most significant "1" bit index; result undefined if in == 0
    SIMDEE_INL int msb(unsigned long long in) { return (8*int(sizeof(unsigned long long)) - 1) - __builtin_clzll(in); }
    // convert little/big endian
    SIMDEE_INL uint16_t bswap(uint16_t in) { return __builtin_bswap16(in); }
    // convert little/big endian
    SIMDEE_INL uint32_t bswap(uint32_t in) { return __builtin_bswap32(in); }
    // convert little/big endian
    SIMDEE_INL uint64_t bswap(uint64_t in) { return __builtin_bswap64(in); }
}

#elif defined(_MSC_VER) // Visual Studio

#include <intrin.h>

namespace sd {
    // least significant "1" bit index; result undefined if in == 0
    SIMDEE_INL unsigned long lsb(unsigned __int32 in) { unsigned long res; _BitScanForward(&res, in); return res; }
    // most significant "1" bit index; result undefined if in == 0
    SIMDEE_INL unsigned long msb(unsigned __int32 in) { unsigned long res; _BitScanReverse(&res, in); return res; }
    // convert little/big endian
    SIMDEE_INL uint16_t bswap(uint16_t in) { return _byteswap_ushort(in); }
    // convert little/big endian
    SIMDEE_INL uint32_t bswap(uint32_t in) { return _byteswap_ulong(in); }
    // convert little/big endian
    SIMDEE_INL uint64_t bswap(uint64_t in) { return _byteswap_uint64(in); }

#if defined(_M_X64)
    // least significant "1" bit index; result undefined if in == 0
    SIMDEE_INL unsigned long lsb(unsigned __int64 in) { unsigned long res; _BitScanForward64(&res, in); return res; }
    // most significant "1" bit index; result undefined if in == 0
    SIMDEE_INL unsigned long msb(unsigned __int64 in) { unsigned long res; _BitScanReverse64(&res, in); return res; }
#endif
}

#else

#error "util_1b.h: incompatible compiler"

#endif

namespace sd {

    // provides indices of set (1) bits, ordered from least significant to most significant
    struct bit_iterator : std::iterator<std::input_iterator_tag, uint32_t> {
        uint32_t mask;

        SIMDEE_INL constexpr bit_iterator(uint32_t mask_) : mask(mask_) {}
        SIMDEE_INL uint32_t operator*() const { return uint32_t(lsb(mask)); }
        SIMDEE_INL uint32_t operator->() const { return uint32_t(lsb(mask)); }
        SIMDEE_INL bit_iterator& operator++() { mask = mask & (mask - 1); return *this; }
        SIMDEE_INL bit_iterator operator++(int) { bit_iterator r = mask; operator++(); return r; }
        SIMDEE_INL bool operator!=(const bit_iterator& rhs) const { return mask != rhs.mask; }
    };

    // float-to-int rounding conversion
    SIMDEE_INL int32_t round_to_int32(double in) {
        in += double((1LL << 52) + (1LL << 51));
        int32_t out[2];
        std::memcpy(out, &in, sizeof(double));
        return out[0]; // assuming little-endian architecture, use out[1] for big-endian
    }

}

#endif // SIMDEE_UTIL_BITFIDDLE_HPP
