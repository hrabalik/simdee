#ifndef SIMDIFY_FLT
#define SIMDIFY_FLT

#include "common.hpp"

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    // SIMD off -- glorified float
    struct flt {
        using mm_t = float;
        using float_t = float;
        using bitmask_t = uint;
        using mask_t = mask<flt>;
        static const auto W = sizeof(mm_t) / sizeof(float_t);
        using array_t = std::array<float_t, W>;
        using iterator = array_t::iterator;
        using const_iterator = array_t::const_iterator;
        union { mm_t mm; array_t f; };
        INL flt() {}
        INL flt(mm_t r) : mm(r) {}
        INL flt(const flt& r) { mm = r.mm; }
        INL explicit flt(zero_t) : mm(0) {}
        INL explicit flt(all_bits_t) : flt(mask_t(ALL_BITS)) {}
        INL explicit flt(abs_mask_t) : flt(mask_t(ABS_MASK)) {}
        INL explicit flt(sign_bit_t) : flt(mask_t(SIGN_BIT)) {}
        INL explicit flt(mask_t r) : mm(r.f) {}
        INL flt(std::initializer_list<float_t> r) { std::copy(r.begin(), r.end(), f.begin()); }
        INL flt& operator=(const flt& r) { mm = r.mm; return *this; }
        INL iterator begin() { return f.begin(); }
        INL iterator end() { return f.end(); }
        INL const_iterator cbegin() const { return f.cbegin(); }
        INL const_iterator cend() const { return f.cend(); }
        INL float_t front() const { return mm; }
        INL float_t back() const { return mm; }
    };

    INL const flt operator&(const flt& l, const flt& r) { return flt::mask_t::andf(l.mm, r.mm); }
    INL const flt operator|(const flt& l, const flt& r) { return flt::mask_t::orf(l.mm, r.mm); }
    INL const flt operator^(const flt& l, const flt& r) { return flt::mask_t::xorf(l.mm, r.mm); }
    INL const flt operator~(const flt& l) { return flt::mask_t::notf(l.mm); }
    INL const flt operator<(const flt& l, const flt& r) { return l.mm < r.mm ? flt(ALL_BITS) : flt(ZERO); }
    INL const flt operator>(const flt& l, const flt& r) { return l.mm > r.mm ? flt(ALL_BITS) : flt(ZERO); }
    INL const flt operator<=(const flt& l, const flt& r) { return l.mm <= r.mm ? flt(ALL_BITS) : flt(ZERO); }
    INL const flt operator>=(const flt& l, const flt& r) { return l.mm >= r.mm ? flt(ALL_BITS) : flt(ZERO); }
    INL const flt operator==(const flt& l, const flt& r) { return l.mm == r.mm ? flt(ALL_BITS) : flt(ZERO); }
    INL const flt operator!=(const flt& l, const flt& r) { return l.mm != r.mm ? flt(ALL_BITS) : flt(ZERO); }
    INL const flt operator+(const flt& l, const flt& r) { return l.mm + r.mm; }
    INL const flt operator-(const flt& l, const flt& r) { return l.mm - r.mm; }
    INL const flt operator*(const flt& l, const flt& r) { return l.mm * r.mm; }
    INL const flt operator/(const flt& l, const flt& r) { return l.mm / r.mm; }
    INL const flt andnot(const flt& l, const flt& r) { return flt::mask_t::andnotf(l.mm, r.mm); }
    INL const flt min(const flt& l, const flt& r) { return std::min(l.mm, r.mm); }
    INL const flt max(const flt& l, const flt& r) { return std::max(l.mm, r.mm); }
    INL const flt cond(const flt& pred, const flt& if_true, const flt& if_false) { return pred.mm != 0 ? if_true : if_false; }

    // horizontal operations
    template <>
    struct horizontal<flt> : horizontal_base<flt> {
        template <binary_op_t F>
        static INL const flt reduce_wide(const flt& in) {
            return in;
        }
        static INL uint find(const flt& in) {
            return 0;
        }
    };

}

#undef INL

#endif // SIMDIFY_FLT
