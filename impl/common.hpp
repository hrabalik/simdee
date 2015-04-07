#ifndef SIMDIFY_COMMON
#define SIMDIFY_COMMON

#include "../util/inline.hpp"
#include "../util/type.hpp"
#include "../util/1b.hpp"
#include <cmath>
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <array>

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    //
    // types for overload selection/disambiguation
    //
    const struct zero_t {} ZERO;
    const struct all_bits_t {} ALL_BITS;
    const struct abs_mask_t {} ABS_MASK;
    const struct sign_bit_t {} SIGN_BIT;

    //
    // wrapper around the bit mask type for a specific SIMD type
    //
    template <typename T>
    struct mask {
        // these types must be present in a SIMD type
        using mm_t = typename T::mm_t;
        using float_t = typename T::float_t;
        using bitmask_t = typename T::bitmask_t;

        // type-related sanity checks
        static_assert(sizeof(float_t) == sizeof(bitmask_t), "size of bitmask_t must match the size of float_t");
        static_assert(std::is_integral<bitmask_t>::value, "bitmask_t must be an integral type");
        static_assert(std::is_unsigned<bitmask_t>::value, "bitmask_t must be an unsigned type");
        static_assert(sizeof(mm_t) % sizeof(bitmask_t) == 0, "size of mm_t must be a multiple of the size of bitmask_t");

        // useful bit masks
        static const bitmask_t ZERO_BIT_MASK = 0;
        static const bitmask_t ALL_BITS_MASK = ~ZERO_BIT_MASK;
        static const bitmask_t ABS_BIT_MASK = ALL_BITS_MASK >> 1;
        static const bitmask_t SIGN_BIT_MASK = ~ABS_BIT_MASK;

        // data
        union {
            float f;
            bitmask_t b;
        };

        // constructors
        INL explicit mask() {}
        INL explicit mask(bitmask_t r) : b(r) {}
        INL explicit mask(zero_t r) : b(ZERO_BIT_MASK) {}
        INL explicit mask(all_bits_t r) : b(ALL_BITS_MASK) {}
        INL explicit mask(abs_mask_t r) : b(ABS_BIT_MASK) {}
        INL explicit mask(sign_bit_t r) : b(SIGN_BIT_MASK) {}

        // bitmask_t -- float_t conversion
        INL static bitmask_t tob(float_t l) { mask m; m.f = l; return m.b; }
        INL static float_t tof(bitmask_t l) { mask m; m.b = l; return m.f; }

        // bitwise operations with float_t
        INL static float_t andf(float_t l, float_t r) { return tof(tob(l) & tob(r)); }
        INL static float_t orf(float_t l, float_t r) { return tof(tob(l) | tob(r)); }
        INL static float_t xorf(float_t l, float_t r) { return tof(tob(l) ^ tob(r)); }
        INL static float_t notf(float_t l) { return tof(~tob(l)); }
        INL static float_t andnotf(float_t l, float_t r) { return tof(tob(l) & ~tob(r)); }
    };

    //
    // helper class for lazy evaluation of bitwise not
    //
    template <typename T>
    struct bitwise_not;

    //
    // meta operations - apply to all SIMD types
    //
    template <typename T> INL T& operator &=(T& l, const T& r) { l = l & r; return l; }
    template <typename T> INL T& operator |=(T& l, const T& r) { l = l | r; return l; }
    template <typename T> INL T& operator ^=(T& l, const T& r) { l = l ^ r; return l; }
    template <typename T> INL T& operator +=(T& l, const T& r) { l = l + r; return l; }
    template <typename T> INL T& operator -=(T& l, const T& r) { l = l - r; return l; }
    template <typename T> INL T& operator *=(T& l, const T& r) { l = l * r; return l; }
    template <typename T> INL T& operator /=(T& l, const T& r) { l = l / r; return l; }
    template <typename T> INL const T abs(const T& in) { return in & T(ABS_MASK); }
    template <typename T> INL const T signbit(const T& in) { return in & T(SIGN_BIT); }
    template <typename T> INL const T signum(const T& in) { return cond(in > T(0), T(1), T(-1)); }

    template<typename T>
    INL const T operator&(const T& l, const bitwise_not<T>& r) { return andnot(l, r.neg); }
    template<typename T>
    INL const T operator&(const bitwise_not<T>& r, const T& l) { return andnot(l, r.neg); }
    template<typename T>
    INL const T operator&(const bitwise_not<T>& l, const bitwise_not<T>& r) { return andnot(l, r.neg); }

    template <typename T>
    INL const T apply_mask(const T& in, const T& mask, float_t neutral_value) {
        return cond(mask, in, T(neutral_value));
    }
    template <typename T>
    INL const T apply_mask(const T& in, const T& mask, zero_t zero_neutral_value) {
        return cond(mask, in, T(ZERO));
    }

    //
    // horizontal operations
    //
    template <typename T>
    struct horizontal;

    template <typename T>
    struct horizontal_base {
        using unary_op_t = ptr<const T(const T& in)>;
        using binary_op_t = ptr<const T(const T& l, const T& r)>;
        using float_t = typename T::float_t;
        using find_t = uint;
        using reduce_find_t = std::pair<float_t, uint>;

        template <binary_op_t F>
        static INL float_t reduce(const T& in) { return reduce_wide<F>(in).front(); }

        template <binary_op_t F>
        static INL const T reduce_wide(const T& in) { return horizontal<T>::template reduce_wide<F>(in); }

        template <binary_op_t F>
        static INL std::pair<float_t, uint> reduce_find(const T& in) {
            auto in_reduced = reduce_wide<F>(in);
            auto selected = horizontal<T>::find(in == in_reduced);
            return std::make_pair(in_reduced.front(), selected);
        }
    };

}

#undef INL

#endif // SIMDIFY_COMMON
