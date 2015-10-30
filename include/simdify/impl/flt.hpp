#ifndef SIMDIFY_FLT
#define SIMDIFY_FLT

#include "common.hpp"

namespace simd {

    // SIMD emulation with a glorified float
    struct flt : simd_base<float, float, uint32_t, int32_t, flt> {
        SIMDIFY_FORCE_INLINE flt() {}
        SIMDIFY_FORCE_INLINE flt(mm_t r) : simd_base(r) {}
        SIMDIFY_FORCE_INLINE flt(const f_t* r) : simd_base(*r) {}
        SIMDIFY_FORCE_INLINE explicit flt(zero_t) : simd_base(0) {}
        SIMDIFY_FORCE_INLINE explicit flt(const F& r) : simd_base(r.f) {}
        SIMDIFY_FORCE_INLINE explicit flt(const U& r) : simd_base(conversions::castf(r.u)) {}
        SIMDIFY_FORCE_INLINE explicit flt(const I& r) : simd_base(conversions::castf(r.i)) {}
        SIMDIFY_FORCE_INLINE void load(const f_t* r) { mm = *r; }
        SIMDIFY_FORCE_INLINE void load(const u_t* r) { mm = conversions::castf(*r); }
        SIMDIFY_FORCE_INLINE void load(const i_t* r) { mm = conversions::castf(*r); }
        SIMDIFY_FORCE_INLINE void store(f_t* r) { *r = mm; }
        SIMDIFY_FORCE_INLINE void store(u_t* r) { conversions::castf(*r) = mm; }
        SIMDIFY_FORCE_INLINE void store(i_t* r) { conversions::castf(*r) = mm; }
        SIMDIFY_FORCE_INLINE f_t front() const { return mm; }
        SIMDIFY_FORCE_INLINE f_t back() const { return mm; }
    };

    SIMDIFY_FORCE_INLINE const flt operator&(const flt& l, const flt& r) { return flt::conversions::band(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt operator|(const flt& l, const flt& r) { return flt::conversions::bor(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt operator^(const flt& l, const flt& r) { return flt::conversions::bxor(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt operator~(const flt& l) { return flt::conversions::bnot(l.mm); }
    SIMDIFY_FORCE_INLINE const flt operator<(const flt& l, const flt& r) { return l.mm < r.mm ? flt(all_bits_t{}) : flt(zero_t{}); }
    SIMDIFY_FORCE_INLINE const flt operator>(const flt& l, const flt& r) { return l.mm > r.mm ? flt(all_bits_t{}) : flt(zero_t{}); }
    SIMDIFY_FORCE_INLINE const flt operator<=(const flt& l, const flt& r) { return l.mm <= r.mm ? flt(all_bits_t{}) : flt(zero_t{}); }
    SIMDIFY_FORCE_INLINE const flt operator>=(const flt& l, const flt& r) { return l.mm >= r.mm ? flt(all_bits_t{}) : flt(zero_t{}); }
    SIMDIFY_FORCE_INLINE const flt operator==(const flt& l, const flt& r) { return l.mm == r.mm ? flt(all_bits_t{}) : flt(zero_t{}); }
    SIMDIFY_FORCE_INLINE const flt operator!=(const flt& l, const flt& r) { return l.mm != r.mm ? flt(all_bits_t{}) : flt(zero_t{}); }
    SIMDIFY_FORCE_INLINE const flt operator+(const flt& l, const flt& r) { return l.mm + r.mm; }
    SIMDIFY_FORCE_INLINE const flt operator-(const flt& l, const flt& r) { return l.mm - r.mm; }
    SIMDIFY_FORCE_INLINE const flt operator*(const flt& l, const flt& r) { return l.mm * r.mm; }
    SIMDIFY_FORCE_INLINE const flt operator/(const flt& l, const flt& r) { return l.mm / r.mm; }
    SIMDIFY_FORCE_INLINE const flt andnot(const flt& l, const flt& r) { return flt::conversions::bandnot(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt min(const flt& l, const flt& r) { return std::min(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt max(const flt& l, const flt& r) { return std::max(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt cond(const flt& pred, const flt& if_true, const flt& if_false) { return pred.mm != 0 ? if_true : if_false; }

    // horizontal operations
    template <>
    struct horizontal_impl<flt> : horizontal_impl_base<flt> {
        struct find_result_iterator : std::iterator<std::input_iterator_tag, uint> {
            uint mask;

            SIMDIFY_FORCE_INLINE find_result_iterator(uint mask_) : mask(mask_) {}
            SIMDIFY_FORCE_INLINE uint operator*() const { return 0; }
            SIMDIFY_FORCE_INLINE uint operator->() const { return 0; }
            SIMDIFY_FORCE_INLINE find_result_iterator& operator++() { mask = 0; return *this; }
            SIMDIFY_FORCE_INLINE find_result_iterator operator++(int) { find_result_iterator r{ mask }; mask = 0; return r; }
            SIMDIFY_FORCE_INLINE bool operator!=(const find_result_iterator& rhs) const { return mask != rhs.mask; }
        };

        struct find_result {
            uint field;

            SIMDIFY_FORCE_INLINE find_result(uint field_) : field(field_) {}
            SIMDIFY_FORCE_INLINE find_result_iterator begin() const { return field; }
            SIMDIFY_FORCE_INLINE find_result_iterator end() const { return 0; }
        };

        static SIMDIFY_FORCE_INLINE find_result find(const flt& in) { return in.u[0] != 0; }
        static SIMDIFY_FORCE_INLINE bool any(const flt& in) { return in.u[0] != 0; }
        static SIMDIFY_FORCE_INLINE bool all(const flt& in) { return in.u[0] != 0; }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE const flt reduce_vector(const flt& in) { return in; }
    };

}

#endif // SIMDIFY_FLT
