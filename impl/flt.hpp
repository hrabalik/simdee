#ifndef SIMDIFY_FLT
#define SIMDIFY_FLT

#include "common.hpp"

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    // SIMD emulation with a glorified float
    struct flt : simd_base<float, float, uint32_t, flt> {
        INL flt() {}
        INL flt(mm_t r) : simd_base(r) {}
        INL flt(const flt& r) : simd_base(r.mm) {}
        INL explicit flt(zero_t) : simd_base(0) {}
        INL explicit flt(mask_t r) : flt(r.f) {}

        INL fp_t front() const { return mm; }
        INL fp_t back() const { return mm; }
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
