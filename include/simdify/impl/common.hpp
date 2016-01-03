#ifndef SIMDIFY_COMMON
#define SIMDIFY_COMMON

#include "../util/inline.hpp"
#include "../util/1b.hpp"
#include "../util/integral.hpp"
#include "expr.hpp"
#include <array>
#include <type_traits>

namespace simd {

    //
    // forward declarations
    //
    template <typename T>
    struct is_simd_type : std::integral_constant<bool, false> {};
    template <typename T>
    struct horizontal_impl;
    template <typename T>
    struct simd_type_traits;

    //
    // SIMD type base class (with derived class as Crtp, CRTP-style)
    //
    template <typename Crtp>
    struct simd_base {
        using traits_t = simd_type_traits<Crtp>;
        using mm_t = typename traits_t::mm_t;
        using e_t = typename traits_t::e_t;

        enum : std::size_t {
            W = sizeof(mm_t) / sizeof(e_t)
        };

        using array_e = std::array<e_t, W>;
        using horizontal = horizontal_impl<Crtp>;

        // data
        mm_t mm;
    };

    //
    // meta operations - apply to all SIMD types
    //
    template <typename T> SIMDIFY_INL T& operator &=(T& l, const T& r) { l = l & r; return l; }
    template <typename T> SIMDIFY_INL T& operator |=(T& l, const T& r) { l = l | r; return l; }
    template <typename T> SIMDIFY_INL T& operator ^=(T& l, const T& r) { l = l ^ r; return l; }
    template <typename T> SIMDIFY_INL T& operator +=(T& l, const T& r) { l = l + r; return l; }
    template <typename T> SIMDIFY_INL T& operator -=(T& l, const T& r) { l = l - r; return l; }
    template <typename T> SIMDIFY_INL T& operator *=(T& l, const T& r) { l = l * r; return l; }
    template <typename T> SIMDIFY_INL T& operator /=(T& l, const T& r) { l = l / r; return l; }
    template <typename T> SIMDIFY_INL const T operator+(const T& in) { return in; }
    template <typename T> SIMDIFY_INL const T abs(const T& in) { return in & abs_mask(); }
    template <typename T> SIMDIFY_INL const T signbit(const T& in) { return in & sign_bit(); }
    template <typename T> SIMDIFY_INL const T signum(const T& in) { return cond(in > zero(), 1, -1); }

    template <typename T, typename Mask, typename N>
    SIMDIFY_INL const T apply_mask(const T& in, const Mask& mask, const N& neutral_value) {
        return cond(mask, in, neutral_value);
    }

    //
    // provides access to min, max, operator+, operator* before they have been declared
    //
    template <typename T>
    struct operators {
        static SIMDIFY_INL const T min_(const T& l, const T& r) { return min(l, r); }
        static SIMDIFY_INL const T max_(const T& l, const T& r) { return max(l, r); }
        static SIMDIFY_INL const T add_(const T& l, const T& r) { return l + r; }
        static SIMDIFY_INL const T mul_(const T& l, const T& r) { return l * r; }
    };

    //
    // base class for horizontal<T>
    //
    template <typename T>
    struct horizontal_impl_base {
        using unary_op_t = const T(*)(const T& in);
        using binary_op_t = const T(*)(const T& l, const T& r);
        using e_t = typename T::e_t;
        using reduce_find_t = std::pair<e_t, bit_t>;
        using ops = operators<T>;
        using mask_t = typename simd_type_traits<T>::vec_u;

        template <binary_op_t F>
        static SIMDIFY_INL e_t reduce(const T& in) { return reduce_vector<F>(in).front(); }

        template <binary_op_t F>
        static SIMDIFY_INL const T reduce_vector(const T& in) { return horizontal_impl<T>::template reduce_vector<F>(in); }

        template <binary_op_t F>
        static SIMDIFY_INL reduce_find_t reduce_find(const T& in) {
            auto in_reduced = reduce_vector<F>(in);
            auto selected = horizontal_impl<T>::find(in == in_reduced);
            return std::make_pair(in_reduced.front(), selected);
        }

        template <binary_op_t F, typename N>
        static SIMDIFY_INL e_t reduce_with_mask(const T& in, const mask_t& mask, const N& neutral_value) {
            return reduce<F>(apply_mask(in, mask, neutral_value));
        }

        template <binary_op_t F, typename N>
        static SIMDIFY_INL reduce_find_t reduce_find_with_mask(const T& in, const mask_t& mask, N&& neutral_value) {
            return reduce_find<F>(apply_mask(in, mask, std::forward<N>(neutral_value)));
        }

        static SIMDIFY_INL e_t min(const T& in) { return reduce<ops::min_>(in); }
        static SIMDIFY_INL e_t max(const T& in) { return reduce<ops::max_>(in); }
        static SIMDIFY_INL e_t sum(const T& in) { return reduce<ops::add_>(in); }
        static SIMDIFY_INL e_t product(const T& in) { return reduce<ops::mul_>(in); }
        static SIMDIFY_INL reduce_find_t min_find(const T& in) { return reduce_find<ops::min_>(in); }
        static SIMDIFY_INL reduce_find_t max_find(const T& in) { return reduce_find<ops::max_>(in); }

        static SIMDIFY_INL e_t min_with_mask(const T& in, const mask_t& mask) {
            return reduce_with_mask<ops::min_>(in, mask, std::numeric_limits<e_t>::max());
        }
        static SIMDIFY_INL e_t max_with_mask(const T& in, const mask_t& mask) {
            return reduce_with_mask<ops::max_>(in, mask, std::numeric_limits<e_t>::lowest());
        }
        static SIMDIFY_INL e_t sum_with_mask(const T& in, const mask_t& mask) {
            return reduce_with_mask<ops::add_>(in, mask, zero());
        }
        static SIMDIFY_INL e_t product_with_mask(const T& in, const mask_t& mask) {
            return reduce_with_mask<ops::add_>(in, mask, e_t(1));
        }
        static SIMDIFY_INL reduce_find_t min_find_with_mask(const T& in, const mask_t& mask) {
            return reduce_find_with_mask<ops::min_>(in, mask, std::numeric_limits<e_t>::max());
        }
        static SIMDIFY_INL reduce_find_t max_find_with_mask(const T& in, const mask_t& mask) {
            return reduce_find_with_mask<ops::max_>(in, mask, std::numeric_limits<e_t>::lowest());
        }
    };

}

#endif // SIMDIFY_COMMON
