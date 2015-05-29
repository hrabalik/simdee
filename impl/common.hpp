#ifndef SIMDIFY_COMMON
#define SIMDIFY_COMMON

#include "../util/inline.hpp"
#include "../util/type.hpp"
#include "../util/1b.hpp"
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
    // types for overload selection/disambiguation
    //
    struct zero_t {};
    struct all_bits_t {};
    struct abs_mask_t {};
    struct sign_bit_t {};

    //
    // forward declarations
    //
    template <typename T>
    struct horizontal_impl;

    template <typename T>
    struct allocator;

    //
    // SIMD type base class (with derived class as T, CRTP-style)
    //
    template <typename _mm_t, typename _fp_t, typename _bitmask_t, typename T>
    struct simd_base {
        using mm_t = _mm_t;
        using fp_t = _fp_t;
        using bitmask_t = _bitmask_t;

        // type-related sanity checks
        static_assert(sizeof(mm_t) % sizeof(fp_t) == 0, "size of mm_t must be a multiple of the size of fp_t");
        static_assert(sizeof(fp_t) == sizeof(bitmask_t), "size of bitmask_t must match the size of fp_t");
        static_assert(std::is_integral<bitmask_t>::value, "bitmask_t must be an integral type");
        static_assert(std::is_unsigned<bitmask_t>::value, "bitmask_t must be an unsigned type");

        // useful constants and types
        static const auto W = sizeof(mm_t) / sizeof(fp_t);
        using array_t = std::array<fp_t, W>;
        using mask_array_t = std::array<bitmask_t, W>;
        using iterator = typename array_t::iterator;
        using reverse_iterator = typename array_t::reverse_iterator;
        using const_iterator = typename array_t::const_iterator;
        using const_reverse_iterator = typename array_t::const_reverse_iterator;
        using unary_op_t = ptr<const T(const T& in)>;
        using binary_op_t = ptr<const T(const T& l, const T& r)>;
        using horizontal = horizontal_impl<T>;

        // a wrapper to disambiguate construction with bitmask_t and fp_t
        struct mask_t {
            // useful bit masks
            static const bitmask_t ZERO_BIT_MASK = 0;
            static const bitmask_t ALL_BITS_MASK = ~ZERO_BIT_MASK;
            static const bitmask_t ABS_BIT_MASK = ALL_BITS_MASK >> 1;
            static const bitmask_t SIGN_BIT_MASK = ~ABS_BIT_MASK;

            // constructors
            SIMDIFY_FORCE_INLINE explicit mask_t() {}
            SIMDIFY_FORCE_INLINE explicit mask_t(bitmask_t r) : b(r) {}
            SIMDIFY_FORCE_INLINE mask_t(zero_t) : b(ZERO_BIT_MASK) {}
            SIMDIFY_FORCE_INLINE mask_t(all_bits_t) : b(ALL_BITS_MASK) {}
            SIMDIFY_FORCE_INLINE mask_t(abs_mask_t) : b(ABS_BIT_MASK) {}
            SIMDIFY_FORCE_INLINE mask_t(sign_bit_t) : b(SIGN_BIT_MASK) {}

            // bitmask_t -- fp_t conversion
            SIMDIFY_FORCE_INLINE static bitmask_t tob(fp_t l) { mask_t m; m.f = l; return m.b; }
            SIMDIFY_FORCE_INLINE static fp_t tof(bitmask_t l) { mask_t m; m.b = l; return m.f; }

            // bitwise operations with fp_t
            SIMDIFY_FORCE_INLINE static fp_t andf(fp_t l, fp_t r) { return tof(tob(l) & tob(r)); }
            SIMDIFY_FORCE_INLINE static fp_t orf(fp_t l, fp_t r) { return tof(tob(l) | tob(r)); }
            SIMDIFY_FORCE_INLINE static fp_t xorf(fp_t l, fp_t r) { return tof(tob(l) ^ tob(r)); }
            SIMDIFY_FORCE_INLINE static fp_t notf(fp_t l) { return tof(~tob(l)); }
            SIMDIFY_FORCE_INLINE static fp_t andnotf(fp_t l, fp_t r) { return tof(tob(l) & ~tob(r)); }

            // data
            union {
                float f;
                bitmask_t b;
            };
        };

        // data
        union { mm_t mm; array_t f; mask_array_t i; };

        // constructor
        SIMDIFY_FORCE_INLINE simd_base() {}
        SIMDIFY_FORCE_INLINE simd_base(mm_t r) : mm(r) {}
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
    template <typename T> SIMDIFY_FORCE_INLINE const T abs(const T& in) { return in & T(abs_mask_t{}); }
    template <typename T> SIMDIFY_FORCE_INLINE const T signbit(const T& in) { return in & T(sign_bit_t{}); }
    template <typename T> SIMDIFY_FORCE_INLINE const T signum(const T& in) { return cond(in > T(zero_t{}), T(1), T(-1)); }

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
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, zero_t) {
        return cond(mask, in, T(zero_t{}));
    }
    template <typename T>
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, typename T::fp_t neutral_value) {
        return cond(mask, in, T(neutral_value));
    }
    template <typename T>
    SIMDIFY_FORCE_INLINE const T apply_mask(const T& in, const T& mask, typename T::mask_t neutral_mask) {
        return cond(mask, in, T(neutral_mask));
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
        using unary_op_t = ptr<const T(const T& in)>;
        using binary_op_t = ptr<const T(const T& l, const T& r)>;
        using fp_t = typename T::fp_t;
        using reduce_find_t = std::pair<fp_t, uint>;
        using ops = operators<T>;

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE fp_t reduce(const T& in) { return reduce_vector<F>(in).front(); }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE const T reduce_vector(const T& in) { return horizontal_impl<T>::template reduce_vector<F>(in); }

        template <binary_op_t F>
        static SIMDIFY_FORCE_INLINE reduce_find_t reduce_find(const T& in) {
            auto in_reduced = reduce_vector<F>(in);
            auto selected = horizontal_impl<T>::find(in == in_reduced);
            return std::make_pair(in_reduced.front(), selected);
        }

        template <binary_op_t F, typename N>
        static SIMDIFY_FORCE_INLINE fp_t reduce_with_mask(const T& in, const T& mask, N&& neutral_value) {
            return reduce<F>(apply_mask(in, mask, std::forward<N>(neutral_value)));
        }

        template <binary_op_t F, typename N>
        static SIMDIFY_FORCE_INLINE reduce_find_t reduce_find_with_mask(const T& in, const T& mask, N&& neutral_value) {
            return reduce_find<F>(apply_mask(in, mask, std::forward<N>(neutral_value)));
        }

        static SIMDIFY_FORCE_INLINE fp_t min(const T& in) { return reduce<ops::min_>(in); }
        static SIMDIFY_FORCE_INLINE fp_t max(const T& in) { return reduce<ops::max_>(in); }
        static SIMDIFY_FORCE_INLINE fp_t sum(const T& in) { return reduce<ops::add_>(in); }
        static SIMDIFY_FORCE_INLINE fp_t product(const T& in) { return reduce<ops::mul_>(in); }
        static SIMDIFY_FORCE_INLINE reduce_find_t min_find(const T& in) { return reduce_find<ops::min_>(in); }
        static SIMDIFY_FORCE_INLINE reduce_find_t max_find(const T& in) { return reduce_find<ops::max_>(in); }

        static SIMDIFY_FORCE_INLINE fp_t min_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::min_>(in, mask, std::numeric_limits<fp_t>::max());
        }
        static SIMDIFY_FORCE_INLINE fp_t max_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::max_>(in, mask, std::numeric_limits<fp_t>::min());
        }
        static SIMDIFY_FORCE_INLINE fp_t sum_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::add_>(in, mask, zero_t{});
        }
        static SIMDIFY_FORCE_INLINE fp_t product_with_mask(const T& in, const T& mask) {
            return reduce_with_mask<ops::add_>(in, mask, fp_t(1));
        }
        static SIMDIFY_FORCE_INLINE reduce_find_t min_find_with_mask(const T& in, const T& mask) {
            return reduce_find_with_mask<ops::min_>(in, mask, std::numeric_limits<fp_t>::max());
        }
        static SIMDIFY_FORCE_INLINE reduce_find_t max_find_with_mask(const T& in, const T& mask) {
            return reduce_find_with_mask<ops::max_>(in, mask, std::numeric_limits<fp_t>::min());
        }
    };

}

#endif // SIMDIFY_COMMON
