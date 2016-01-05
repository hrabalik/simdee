#ifndef SIMDIFY_DUM
#define SIMDIFY_DUM

#include "common.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_DUM_COMMON_DECLARATIONS( CLASS )                                                         \
                                                                                                         \
SIMDIFY_INL ~CLASS() = default;                                                                          \
                                                                                                         \
SIMDIFY_INL CLASS() = default;                                                                           \
                                                                                                         \
SIMDIFY_INL CLASS(const CLASS&) = default;                                                               \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(const CLASS&) = default;                                                    \
                                                                                                         \
SIMDIFY_INL CLASS(const e_t& r) {                                                                        \
    mm = r;                                                                                              \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL CLASS& operator=(const e_t& r) {                                                             \
    mm = r;                                                                                              \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS(const expr::aligned<T>& r) {                                                           \
    aligned_load(r.ptr);                                                                                 \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS& operator=(const expr::aligned<T>& r) {                                                \
    aligned_load(r.ptr);                                                                                 \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS(const expr::unaligned<T>& r) {                                                         \
    unaligned_load(r.ptr);                                                                               \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS& operator=(const expr::unaligned<T>& r) {                                              \
    unaligned_load(r.ptr);                                                                               \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS(const expr::init<T>& r) {                                                              \
    *this = r.template to<e_t>();                                                                        \
}                                                                                                        \
                                                                                                         \
template <typename T>                                                                                    \
SIMDIFY_INL CLASS& operator=(const expr::init<T>& r) {                                                   \
    *this = r.template to<e_t>();                                                                        \
    return *this;                                                                                        \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void aligned_load(const e_t* r) {                                                            \
    mm = *r;                                                                                             \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void aligned_store(e_t* r) const {                                                           \
    *r = mm;                                                                                             \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void unaligned_load(const e_t* r) {                                                          \
    mm = *r;                                                                                             \
}                                                                                                        \
                                                                                                         \
SIMDIFY_INL void unaligned_store(e_t* r) {                                                               \
    *r = mm;                                                                                             \
}                                                                                                        \
                                                                                                         \
void interleaved_load(const e_t* r, std::size_t step) {                                                  \
    mm = *r;                                                                                             \
}                                                                                                        \
                                                                                                         \
void interleaved_store(e_t* r, std::size_t step) {                                                       \
    *r = mm;                                                                                             \
}                                                                                                        \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        SIMDIFY_DUM_COMMON_DECLARATIONS(dumf);

        SIMDIFY_INL e_t first_element() const { return mm; }
    };

    struct dumu : simd_base<dumu> {
        SIMDIFY_DUM_COMMON_DECLARATIONS(dumu);

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
        SIMDIFY_INL iterator begin() const { return iterator((simd::tou(mm) & 0x80000000U) >> 31); }
        SIMDIFY_INL iterator end() const { return iterator(0); }
    };

    struct dums : simd_base<dums> {
        SIMDIFY_DUM_COMMON_DECLARATIONS(dums);

        SIMDIFY_INL e_t first_element() const { return mm; }
    };

    SIMDIFY_INL const dumu operator&(const dumu& l, const dumu& r) { return uval(tou(l.mm) & tou(r.mm)); }
    SIMDIFY_INL const dumu operator|(const dumu& l, const dumu& r) { return uval(tou(l.mm) | tou(r.mm)); }
    SIMDIFY_INL const dumu operator^(const dumu& l, const dumu& r) { return uval(tou(l.mm) ^ tou(r.mm)); }
    SIMDIFY_INL const dumu operator~(const dumu& l) { return uval(~tou(l.mm)); }
    SIMDIFY_INL const dumu nand(const dumu& l, const dumu& r) { return uval(tou(l.mm) & ~tou(r.mm)); }
    SIMDIFY_INL const bool any(const dumu& in) { return (simd::tou(in.mm) & 0x80000000U) != 0; }
    SIMDIFY_INL const bool all(const dumu& in) { return (simd::tou(in.mm) & 0x80000000U) != 0; }

    SIMDIFY_INL const dumu operator<(const dumf& l, const dumf& r) { return (l.mm < r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator>(const dumf& l, const dumf& r) { return (l.mm > r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator<=(const dumf& l, const dumf& r) { return (l.mm <= r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator>=(const dumf& l, const dumf& r) { return (l.mm >= r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator==(const dumf& l, const dumf& r) { return (l.mm == r.mm) ? 0xffffffffU : 0; }
    SIMDIFY_INL const dumu operator!=(const dumf& l, const dumf& r) { return (l.mm != r.mm) ? 0xffffffffU : 0; }

    SIMDIFY_INL const dumf operator-(const dumf& in) { return -in.mm; }
    SIMDIFY_INL const dumf operator+(const dumf& l, const dumf& r) { return l.mm + r.mm; }
    SIMDIFY_INL const dumf operator-(const dumf& l, const dumf& r) { return l.mm - r.mm; }
    SIMDIFY_INL const dumf operator*(const dumf& l, const dumf& r) { return l.mm * r.mm; }
    SIMDIFY_INL const dumf operator/(const dumf& l, const dumf& r) { return l.mm / r.mm; }

    SIMDIFY_INL const dumf min(const dumf& l, const dumf& r) { return std::min(l.mm, r.mm); }
    SIMDIFY_INL const dumf max(const dumf& l, const dumf& r) { return std::max(l.mm, r.mm); }
    SIMDIFY_INL const dumf sqrt(const dumf& l) { return std::sqrt(l.mm); }
    SIMDIFY_INL const dumf rsqrt(const dumf& l) { return 1 / std::sqrt(l.mm); }
    SIMDIFY_INL const dumf rcp(const dumf& l) { return 1 / l.mm; }
    SIMDIFY_INL const dumf abs(const dumf& l) { return std::abs(l.mm); }

    SIMDIFY_INL const dumf cond(const dumu& pred, const dumf& if_true, const dumf& if_false) {
        return any(pred) ? if_true : if_false;
    }
    SIMDIFY_INL const dumu cond(const dumu& pred, const dumu& if_true, const dumu& if_false) {
        return any(pred) ? if_true : if_false;
    }
    SIMDIFY_INL const dums cond(const dumu& pred, const dums& if_true, const dums& if_false) {
        return any(pred) ? if_true : if_false;
    }

    // horizontal operations
    template <typename Crtp>
    struct dum_horizontal_impl : horizontal_impl_base<Crtp> {
        template <binary_op_t F>
        static SIMDIFY_INL const Crtp reduce(const Crtp& in) { return in; }
    };

    template <>
    struct horizontal_impl<dumf> : dum_horizontal_impl<dumf> {};
    template <>
    struct horizontal_impl<dumu> : dum_horizontal_impl<dumu> {};
    template <>
    struct horizontal_impl<dums> : dum_horizontal_impl<dums> {};

}

#undef SIMDIFY_DUM_COMMON_DECLARATIONS

#endif // SIMDIFY_DUM
