#ifndef SIMDIFY_DUM
#define SIMDIFY_DUM

#include "common.hpp"

namespace simd {

    struct dumf;
    struct dumu;
    struct dums;

    template<>
    struct is_simd_type<dumf> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dumu> : std::integral_constant<bool, true> {};
    template<>
    struct is_simd_type<dums> : std::integral_constant<bool, true> {};

    template <typename Mm_t>
    struct dum_traits {
        using mm_t = Mm_t;
        using e_t = Mm_t;
        using f_t = float;
        using u_t = uint32_t;
        using s_t = int32_t;
        using vec_f = dumf;
        using vec_u = dumu;
        using vec_s = dums;
    };

    template <>
    struct simd_type_traits<dumf> : dum_traits<float> {};

    template <>
    struct simd_type_traits<dumu> : dum_traits<uint32_t> {};

    template <>
    struct simd_type_traits<dums> : dum_traits<int32_t> {};

    // SIMD emulation with a glorified float
    struct dumf : simd_base<dumf> {
        SIMDIFY_INL ~dumf() = default;

        SIMDIFY_INL dumf() = default;
        SIMDIFY_INL dumf(const dumf&) = default;
        SIMDIFY_INL dumf(mm_t r) { *this = r; }
        template <typename T>
        SIMDIFY_INL dumf(const expr::aligned<T>& r) { *this = r; }
        template <typename T>
        SIMDIFY_INL dumf(const expr::unaligned<T>& r) { *this = r; }
        template <typename T>
        SIMDIFY_INL dumf(const expr::init<T>& r) { *this = r; }

        SIMDIFY_INL dumf& operator=(const dumf&) = default;
        SIMDIFY_INL dumf& operator=(mm_t r) { mm = r; return *this; }
        template <typename T>
        SIMDIFY_INL dumf& operator=(const expr::aligned<T>& r) { mm = *r.ptr; return *this; }
        template <typename T>
        SIMDIFY_INL dumf& operator=(const expr::unaligned<T>& r) { mm = *r.ptr; return *this; }
        template <typename T>
        SIMDIFY_INL dumf& operator=(const expr::init<T>& r) { *this = r.template to<e_t>(); return *this; }

        SIMDIFY_INL void aligned_load(const e_t* r) { mm = *r; }
        SIMDIFY_INL void aligned_store(e_t* r) const { *r = mm; }
        SIMDIFY_INL void unaligned_load(const e_t* r) { mm = *r; }
        SIMDIFY_INL void unaligned_store(e_t* r) const { *r = mm; }

        SIMDIFY_INL void interleaved_load(const e_t* r, std::size_t) { mm = *r; }
        SIMDIFY_INL void interleaved_store(e_t* r, std::size_t) { *r = mm; }

        SIMDIFY_INL e_t first_element() const { return mm; }
    };

    struct dumu : simd_base<dumu> {
        SIMDIFY_INL e_t first_element() const { return mm; }

        struct iterator : std::iterator<std::input_iterator_tag, bit_t> {
            bit_t mask;

            SIMDIFY_INL explicit iterator(bit_t mask_) : mask(mask_) {}
            SIMDIFY_INL bit_t operator*() const { return 0; }
            SIMDIFY_INL bit_t operator->() const { return 0; }
            SIMDIFY_INL iterator& operator++() { mask = 0; return *this; }
            SIMDIFY_INL iterator operator++(int) { iterator r{ mask }; mask = 0; return r; }
            SIMDIFY_INL bool operator!=(const iterator& rhs) const { return mask != rhs.mask; }
        };

        SIMDIFY_INL bit_t front() const { return 0; }
        SIMDIFY_INL iterator begin() const { return iterator(simd::tou(mm) ? 1: 0); }
        SIMDIFY_INL iterator end() const { return iterator(0); }
    };

    struct dums : simd_base<dums> {
        SIMDIFY_INL e_t first_element() const { return mm; }

    };

    SIMDIFY_INL const dumf operator&(const dumf& l, const dumf& r) { return uval(tou(l.mm) & tou(r.mm)); }
    SIMDIFY_INL const dumf operator|(const dumf& l, const dumf& r) { return uval(tou(l.mm) | tou(r.mm)); }
    SIMDIFY_INL const dumf operator^(const dumf& l, const dumf& r) { return uval(tou(l.mm) ^ tou(r.mm)); }
    SIMDIFY_INL const dumf operator~(const dumf& l) { return uval(~tou(l.mm)); }
    SIMDIFY_INL const dumf operator<(const dumf& l, const dumf& r) { return l.mm < r.mm ? dumf(all_bits()) : dumf(zero()); }
    SIMDIFY_INL const dumf operator>(const dumf& l, const dumf& r) { return l.mm > r.mm ? dumf(all_bits()) : dumf(zero()); }
    SIMDIFY_INL const dumf operator<=(const dumf& l, const dumf& r) { return l.mm <= r.mm ? dumf(all_bits()) : dumf(zero()); }
    SIMDIFY_INL const dumf operator>=(const dumf& l, const dumf& r) { return l.mm >= r.mm ? dumf(all_bits()) : dumf(zero()); }
    SIMDIFY_INL const dumf operator==(const dumf& l, const dumf& r) { return l.mm == r.mm ? dumf(all_bits()) : dumf(zero()); }
    SIMDIFY_INL const dumf operator!=(const dumf& l, const dumf& r) { return l.mm != r.mm ? dumf(all_bits()) : dumf(zero()); }
    SIMDIFY_INL const dumf operator+(const dumf& l, const dumf& r) { return l.mm + r.mm; }
    SIMDIFY_INL const dumf operator-(const dumf& l, const dumf& r) { return l.mm - r.mm; }
    SIMDIFY_INL const dumf operator*(const dumf& l, const dumf& r) { return l.mm * r.mm; }
    SIMDIFY_INL const dumf operator/(const dumf& l, const dumf& r) { return l.mm / r.mm; }
    SIMDIFY_INL const dumf operator-(const dumf& in) { return -in.mm; }
    SIMDIFY_INL const dumf nand(const dumf& l, const dumf& r) { return uval(tou(l.mm) & ~tou(r.mm)); }
    SIMDIFY_INL const dumf min(const dumf& l, const dumf& r) { return std::min(l.mm, r.mm); }
    SIMDIFY_INL const dumf max(const dumf& l, const dumf& r) { return std::max(l.mm, r.mm); }
    SIMDIFY_INL const dumf cond(const dumf& pred, const dumf& if_true, const dumf& if_false) { return pred.mm != 0 ? if_true : if_false; }

    // horizontal operations
    template <>
    struct horizontal_impl<dumf> : horizontal_impl_base<dumf> {
        struct find_result_iterator : std::iterator<std::input_iterator_tag, bit_t> {
            bit_t mask;

            SIMDIFY_INL find_result_iterator(bit_t mask_) : mask(mask_) {}
            SIMDIFY_INL bit_t operator*() const { return 0; }
            SIMDIFY_INL bit_t operator->() const { return 0; }
            SIMDIFY_INL find_result_iterator& operator++() { mask = 0; return *this; }
            SIMDIFY_INL find_result_iterator operator++(int) { find_result_iterator r{ mask }; mask = 0; return r; }
            SIMDIFY_INL bool operator!=(const find_result_iterator& rhs) const { return mask != rhs.mask; }
        };

        struct find_result {
            bit_t field;

            SIMDIFY_INL find_result(bit_t field_) : field(field_) {}
            SIMDIFY_INL find_result_iterator begin() const { return field; }
            SIMDIFY_INL find_result_iterator end() const { return 0; }
        };

        static SIMDIFY_INL find_result find(const dumf& in) { return simd::tou(in.mm) != 0; }
        static SIMDIFY_INL bool any(const dumf& in) { return simd::tou(in.mm) != 0; }
        static SIMDIFY_INL bool all(const dumf& in) { return simd::tou(in.mm) != 0; }

        template <binary_op_t F>
        static SIMDIFY_INL const dumf reduce_vector(const dumf& in) { return in; }
    };

}

#endif // SIMDIFY_DUM
