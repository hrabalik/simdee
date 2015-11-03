#ifndef SIMDIFY_COMMON
#define SIMDIFY_COMMON

#include "../util/inline.hpp"
#include "../util/1b.hpp"
#include "../util/integral.hpp"
#include "expr.hpp"
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <array>
#include <utility>
#include <iterator>

namespace simd {

    //
    // forward declarations
    //
    template <typename T>
    struct horizontal_impl;

    template <typename T>
    struct allocator;

    template <typename f_t>
    struct floating_point_wrapper;

    template <typename u_t>
    struct unsigned_wrapper;

    template <typename s_t>
    struct signed_wrapper;

    template<typename f_t, typename u_t, typename s_t>
    struct conversions_impl;

    //
    // SIMD type base class (with derived class as Crtp, CRTP-style)
    //
    template <typename Mm_t_, typename F_t_, typename Crtp>
    struct simd_base {
        using mm_t = Mm_t_;
        using f_t = F_t_;
        using u_t = select_uint_t<sizeof(f_t)>;
        using s_t = select_sint_t<sizeof(f_t)>;

        static const auto W = sizeof(mm_t) / sizeof(f_t);
        using array_f = std::array<f_t, W>;
        using array_u = std::array<u_t, W>;
        using array_s = std::array<s_t, W>;
        using horizontal = horizontal_impl<Crtp>;
        using conversions = conversions_impl<f_t, u_t, s_t>;

        // data
        union { mm_t mm; array_f f; array_u u; array_s i; };

        // constructor
        SIMDIFY_FORCE_INLINE simd_base() {}
        SIMDIFY_FORCE_INLINE simd_base(mm_t r) : mm(r) {}
    };

    // conversions and bit-wise operations with floating-point types
    template<typename f_t, typename u_t, typename s_t>
    struct conversions_impl {
        static_assert(std::is_unsigned<u_t>::value, "u_t must be unsigned");
        static_assert(std::is_integral<u_t>::value, "u_t must be integral");
        static_assert(std::is_signed<s_t>::value, "s_t must be signed");
        static_assert(std::is_integral<s_t>::value, "s_t must be integral");
        static_assert(sizeof(f_t) == sizeof(u_t), "u_t must be the same size as f_t");
        static_assert(sizeof(f_t) == sizeof(s_t), "s_t must be the same size as f_t");

        union union_t { f_t f; u_t u; s_t i; };

        // u_t -- f_t -- s_t conversion
        SIMDIFY_FORCE_INLINE static u_t tou(f_t r) { union_t m; m.f = r; return m.u; }
        SIMDIFY_FORCE_INLINE static f_t tof(u_t r) { union_t m; m.u = r; return m.f; }
        SIMDIFY_FORCE_INLINE static f_t& castf(u_t& r) { return reinterpret_cast<f_t&>(r); }
        SIMDIFY_FORCE_INLINE static f_t& castf(s_t& r) { return reinterpret_cast<f_t&>(r); }
        SIMDIFY_FORCE_INLINE static const f_t& castf(const u_t& r) { return reinterpret_cast<const f_t&>(r); }
        SIMDIFY_FORCE_INLINE static const f_t& castf(const s_t& r) { return reinterpret_cast<const f_t&>(r); }

        // bitwise operations with f_t
        SIMDIFY_FORCE_INLINE static f_t band(f_t l, f_t r) { return tof(tou(l) & tou(r)); }
        SIMDIFY_FORCE_INLINE static f_t bor(f_t l, f_t r) { return tof(tou(l) | tou(r)); }
        SIMDIFY_FORCE_INLINE static f_t bxor(f_t l, f_t r) { return tof(tou(l) ^ tou(r)); }
        SIMDIFY_FORCE_INLINE static f_t bnot(f_t l) { return tof(~tou(l)); }
        SIMDIFY_FORCE_INLINE static f_t bandnot(f_t l, f_t r) { return tof(tou(l) & ~tou(r)); }
    };

    //
    // helper class for lazy evaluation of bitwise not
    //
    template <typename T>
    struct bitwise_not;

    //
    // meta operations - apply to all SIMD types
    //
    template <typename T> SIMDIFY_FORCE_INLINE T& operator &=(T& l, const T& r) { l = l & r; return l; }
    template <typename T> SIMDIFY_FORCE_INLINE T& operator |=(T& l, const T& r) { l = l | r; return l; }
    template <typename T> SIMDIFY_FORCE_INLINE T& operator ^=(T& l, const T& r) { l = l ^ r; return l; }
    template <typename T> SIMDIFY_FORCE_INLINE T& operator +=(T& l, const T& r) { l = l + r; return l; }
    template <typename T> SIMDIFY_FORCE_INLINE T& operator -=(T& l, const T& r) { l = l - r; return l; }
    template <typename T> SIMDIFY_FORCE_INLINE T& operator *=(T& l, const T& r) { l = l * r; return l; }
    template <typename T> SIMDIFY_FORCE_INLINE T& operator /=(T& l, const T& r) { l = l / r; return l; }
    template <typename T> SIMDIFY_FORCE_INLINE const T abs(const T& in) { return in & abs_mask(); }
    template <typename T> SIMDIFY_FORCE_INLINE const T signbit(const T& in) { return in & sign_bit(); }
    template <typename T> SIMDIFY_FORCE_INLINE const T signum(const T& in) { return cond(in > zero(), 1, -1); }

    template<typename T>
    SIMDIFY_FORCE_INLINE const T operator~(const bitwise_not<T>& l) { return l.neg; }
    template<typename T>
    SIMDIFY_FORCE_INLINE const T operator&(const bitwise_not<T>& l, const T& r) { return andnot(l.neg, r); }
    template<typename T>
    SIMDIFY_FORCE_INLINE const T operator&(const T& l, const bitwise_not<T>& r) { return andnot(r.neg, l); }
    template<typename T>
    SIMDIFY_FORCE_INLINE const bitwise_not<T> operator&(const bitwise_not<T>& l, const bitwise_not<T>& r) { return bitwise_not<T>(l.neg | r.neg); }
    template<typename T>
    SIMDIFY_FORCE_INLINE const bitwise_not<T> operator|(const bitwise_not<T>& l, const bitwise_not<T>& r) { return bitwise_not<T>(l.neg & r.neg); }
    template<typename T>
    SIMDIFY_FORCE_INLINE const T operator^(const bitwise_not<T>& l, const bitwise_not<T>& r) { return l.neg ^ r.neg; }

    template <typename T>
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, const expr::zero&) {
        return cond(mask, in, zero());
    }
    template <typename T>
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, typename T::F neutral_value) {
        return cond(mask, in, T(neutral_value));
    }
    template <typename T>
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, typename T::U neutral_value) {
        return cond(mask, in, T(neutral_value));
    }
    template <typename T>
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, typename T::I neutral_value) {
        return cond(mask, in, T(neutral_value));
    }
    template <typename T>
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, const T& neutral_value_vector) {
        return cond(mask, in, neutral_value_vector);
    }

    //
    // provides access to min, max, operator+, operator* before they have been declared
    //
    template <typename T>
    struct operators {
        static SIMDIFY_FORCE_INLINE const T min_(const T& l, const T& r) { return min(l, r); }
        static SIMDIFY_FORCE_INLINE const T max_(const T& l, const T& r) { return max(l, r); }
        static SIMDIFY_FORCE_INLINE const T add_(const T& l, const T& r) { return l + r; }
        static SIMDIFY_FORCE_INLINE const T mul_(const T& l, const T& r) { return l * r; }
    };

    //
    // base class for horizontal<T>
    //
    template <typename T>
    struct horizontal_impl_base {
        using unary_op_t = const T(*)(const T& in);
        using binary_op_t = const T(*)(const T& l, const T& r);
        using f_t = typename T::f_t;
        using reduce_find_t = std::pair<f_t, bit_t>;
        using ops = operators<T>;

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE f_t reduce(const T& in) { return reduce_vector<F>(in).front(); }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE const T reduce_vector(const T& in) { return horizontal_impl<T>::template reduce_vector<F>(in); }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE reduce_find_t reduce_find(const T& in) {
            auto in_reduced = reduce_vector<F>(in);
            auto selected = horizontal_impl<T>::find(in == in_reduced);
            return std::make_pair(in_reduced.front(), selected);
        }

        template <binary_op_t F, typename N>
        static SIMDIFY_FORCE_INLINE f_t reduce_with_mask(const T& in, const T& mask, N&& neutral_value) {
            return reduce<F>(apply_mask(in, mask, std::forward<N>(neutral_value)));
        }

        template <binary_op_t F, typename N>
        static SIMDIFY_FORCE_INLINE reduce_find_t reduce_find_with_mask(const T& in, const T& mask, N&& neutral_value) {
            return reduce_find<F>(apply_mask(in, mask, std::forward<N>(neutral_value)));
        }

        static SIMDIFY_FORCE_INLINE f_t min(const T& in) { return reduce<ops::min_>(in); }
        static SIMDIFY_FORCE_INLINE f_t max(const T& in) { return reduce<ops::max_>(in); }
        static SIMDIFY_FORCE_INLINE f_t sum(const T& in) { return reduce<ops::add_>(in); }
        static SIMDIFY_FORCE_INLINE f_t product(const T& in) { return reduce<ops::mul_>(in); }
        static SIMDIFY_FORCE_INLINE reduce_find_t min_find(const T& in) { return reduce_find<ops::min_>(in); }
        static SIMDIFY_FORCE_INLINE reduce_find_t max_find(const T& in) { return reduce_find<ops::max_>(in); }

        static SIMDIFY_FORCE_INLINE f_t min_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::min_>(in, mask, std::numeric_limits<f_t>::max());
        }
        static SIMDIFY_FORCE_INLINE f_t max_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::max_>(in, mask, std::numeric_limits<f_t>::min());
        }
        static SIMDIFY_FORCE_INLINE f_t sum_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::add_>(in, mask, zero());
        }
        static SIMDIFY_FORCE_INLINE f_t product_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::add_>(in, mask, f_t(1));
        }
        static SIMDIFY_FORCE_INLINE reduce_find_t min_find_with_mask(const T& in, const T& mask) {
            return reduce_find_with_mask<ops::min_>(in, mask, std::numeric_limits<f_t>::max());
        }
        static SIMDIFY_FORCE_INLINE reduce_find_t max_find_with_mask(const T& in, const T& mask) {
            return reduce_find_with_mask<ops::max_>(in, mask, std::numeric_limits<f_t>::min());
        }
    };

}

#endif // SIMDIFY_COMMON
