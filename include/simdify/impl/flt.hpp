#ifndef SIMDIFY_FLT
#define SIMDIFY_FLT

#include "common.hpp"

namespace simd {

    // specialization of is_simd_type
    struct flt;
    template<>
    struct is_simd_type<flt> : std::integral_constant<bool, true> {};

    // SIMD emulation with a glorified float
    struct flt : simd_base<float, float, flt> {
        SIMDIFY_FORCE_INLINE ~flt() = default;
        SIMDIFY_FORCE_INLINE flt() = default;
        SIMDIFY_FORCE_INLINE flt(const flt&) = default;
        SIMDIFY_FORCE_INLINE flt(flt&&) = default;
        SIMDIFY_FORCE_INLINE flt& operator=(const flt&) = default;
        SIMDIFY_FORCE_INLINE flt& operator=(flt&&) = default;

        SIMDIFY_FORCE_INLINE flt(mm_t r) : simd_base(r) {}
        SIMDIFY_FORCE_INLINE flt(const expr::zero&) : simd_base(0) {}
        SIMDIFY_FORCE_INLINE void load(const e_t* r) { mm = *r; }
        SIMDIFY_FORCE_INLINE void store(e_t* r) const { *r = mm; }
        SIMDIFY_FORCE_INLINE e_t front() const { return mm; }
        SIMDIFY_FORCE_INLINE e_t back() const { return mm; }

        template <typename T>
        SIMDIFY_FORCE_INLINE flt(const expr::aligned<T>& r) : simd_base(*r.ptr) {}
        template <typename T>
        SIMDIFY_FORCE_INLINE flt(const expr::unaligned<T>& r) : simd_base(*r.ptr) {}
        template <typename T>
        SIMDIFY_FORCE_INLINE flt(const expr::init<T>& r) : flt(r.template to<e_t>()) {}

        SIMDIFY_FORCE_INLINE void interleaved_load(const e_t* r, std::size_t) { load(r); }
        SIMDIFY_FORCE_INLINE void interleaved_store(e_t* r, std::size_t) { store(r); }
    };

    SIMDIFY_FORCE_INLINE const flt operator&(const flt& l, const flt& r) { return utof(tou(l.mm) & tou(r.mm)); }
    SIMDIFY_FORCE_INLINE const flt operator|(const flt& l, const flt& r) { return utof(tou(l.mm) | tou(r.mm)); }
    SIMDIFY_FORCE_INLINE const flt operator^(const flt& l, const flt& r) { return utof(tou(l.mm) ^ tou(r.mm)); }
    SIMDIFY_FORCE_INLINE const flt operator~(const flt& l) { return utof(~tou(l.mm)); }
    SIMDIFY_FORCE_INLINE const flt operator<(const flt& l, const flt& r) { return l.mm < r.mm ? flt(all_bits()) : flt(zero()); }
    SIMDIFY_FORCE_INLINE const flt operator>(const flt& l, const flt& r) { return l.mm > r.mm ? flt(all_bits()) : flt(zero()); }
    SIMDIFY_FORCE_INLINE const flt operator<=(const flt& l, const flt& r) { return l.mm <= r.mm ? flt(all_bits()) : flt(zero()); }
    SIMDIFY_FORCE_INLINE const flt operator>=(const flt& l, const flt& r) { return l.mm >= r.mm ? flt(all_bits()) : flt(zero()); }
    SIMDIFY_FORCE_INLINE const flt operator==(const flt& l, const flt& r) { return l.mm == r.mm ? flt(all_bits()) : flt(zero()); }
    SIMDIFY_FORCE_INLINE const flt operator!=(const flt& l, const flt& r) { return l.mm != r.mm ? flt(all_bits()) : flt(zero()); }
    SIMDIFY_FORCE_INLINE const flt operator+(const flt& l, const flt& r) { return l.mm + r.mm; }
    SIMDIFY_FORCE_INLINE const flt operator-(const flt& l, const flt& r) { return l.mm - r.mm; }
    SIMDIFY_FORCE_INLINE const flt operator*(const flt& l, const flt& r) { return l.mm * r.mm; }
    SIMDIFY_FORCE_INLINE const flt operator/(const flt& l, const flt& r) { return l.mm / r.mm; }
    SIMDIFY_FORCE_INLINE const flt operator-(const flt& in) { return -in.mm; }
    SIMDIFY_FORCE_INLINE const flt andnot(const flt& l, const flt& r) { return utof(tou(l.mm) & ~tou(r.mm)); }
    SIMDIFY_FORCE_INLINE const flt min(const flt& l, const flt& r) { return std::min(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt max(const flt& l, const flt& r) { return std::max(l.mm, r.mm); }
    SIMDIFY_FORCE_INLINE const flt cond(const flt& pred, const flt& if_true, const flt& if_false) { return pred.mm != 0 ? if_true : if_false; }

    // horizontal operations
    template <>
    struct horizontal_impl<flt> : horizontal_impl_base<flt> {
        struct find_result_iterator : std::iterator<std::input_iterator_tag, bit_t> {
            bit_t mask;

            SIMDIFY_FORCE_INLINE find_result_iterator(bit_t mask_) : mask(mask_) {}
            SIMDIFY_FORCE_INLINE bit_t operator*() const { return 0; }
            SIMDIFY_FORCE_INLINE bit_t operator->() const { return 0; }
            SIMDIFY_FORCE_INLINE find_result_iterator& operator++() { mask = 0; return *this; }
            SIMDIFY_FORCE_INLINE find_result_iterator operator++(int) { find_result_iterator r{ mask }; mask = 0; return r; }
            SIMDIFY_FORCE_INLINE bool operator!=(const find_result_iterator& rhs) const { return mask != rhs.mask; }
        };

        struct find_result {
            bit_t field;

            SIMDIFY_FORCE_INLINE find_result(bit_t field_) : field(field_) {}
            SIMDIFY_FORCE_INLINE find_result_iterator begin() const { return field; }
            SIMDIFY_FORCE_INLINE find_result_iterator end() const { return 0; }
        };

        static SIMDIFY_FORCE_INLINE find_result find(const flt& in) { return simd::tou(in.mm) != 0; }
        static SIMDIFY_FORCE_INLINE bool any(const flt& in) { return simd::tou(in.mm) != 0; }
        static SIMDIFY_FORCE_INLINE bool all(const flt& in) { return simd::tou(in.mm) != 0; }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE const flt reduce_vector(const flt& in) { return in; }
    };

}

#endif // SIMDIFY_FLT
