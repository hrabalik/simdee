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
        using iterator = typename array_t::iterator;
        using reverse_iterator = typename array_t::reverse_iterator;
        using const_iterator = typename array_t::const_iterator;
        using const_reverse_iterator = typename array_t::const_reverse_iterator;
        using unary_op_t = ptr<const T(const T& in)>;
        using binary_op_t = ptr<const T(const T& l, const T& r)>;

        // a wrapper to disambiguate construction with bitmask_t and fp_t
        struct mask_t {
            // data
            union {
                float f;
                bitmask_t b;
            };

            // useful bit masks
            static const bitmask_t ZERO_BIT_MASK = 0;
            static const bitmask_t ALL_BITS_MASK = ~ZERO_BIT_MASK;
            static const bitmask_t ABS_BIT_MASK = ALL_BITS_MASK >> 1;
            static const bitmask_t SIGN_BIT_MASK = ~ABS_BIT_MASK;

            // constructors
            INL explicit mask_t() {}
            INL explicit mask_t(bitmask_t r) : b(r) {}
            INL mask_t(zero_t r) : b(ZERO_BIT_MASK) {}
            INL mask_t(all_bits_t r) : b(ALL_BITS_MASK) {}
            INL mask_t(abs_mask_t r) : b(ABS_BIT_MASK) {}
            INL mask_t(sign_bit_t r) : b(SIGN_BIT_MASK) {}

            // bitmask_t -- fp_t conversion
            INL static bitmask_t tob(fp_t l) { mask_t m; m.f = l; return m.b; }
            INL static fp_t tof(bitmask_t l) { mask_t m; m.b = l; return m.f; }

            // bitwise operations with fp_t
            INL static fp_t andf(fp_t l, fp_t r) { return tof(tob(l) & tob(r)); }
            INL static fp_t orf(fp_t l, fp_t r) { return tof(tob(l) | tob(r)); }
            INL static fp_t xorf(fp_t l, fp_t r) { return tof(tob(l) ^ tob(r)); }
            INL static fp_t notf(fp_t l) { return tof(~tob(l)); }
            INL static fp_t andnotf(fp_t l, fp_t r) { return tof(tob(l) & ~tob(r)); }
        };

        // data
        union { mm_t mm; array_t f; };

        // constructor
        INL simd_base() {}
        INL simd_base(mm_t r) : mm(r) {}

        // assignment
        //INL simd_base& operator=(const simd_base& r) { mm = r.mm; return *this; }

        // iteration
        INL iterator begin() { return f.begin(); }
        INL iterator end() { return f.end(); }
        INL reverse_iterator rbegin() { return f.rbegin(); }
        INL reverse_iterator rend() { return f.rend(); }
        INL const_iterator cbegin() const { return f.cbegin(); }
        INL const_iterator cend() const { return f.cend(); }
        INL const_reverse_iterator crbegin() const { return f.crbegin(); }
        INL const_reverse_iterator crend() const { return f.crend(); }
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
    INL const T apply_mask(const T& in, const T& mask, typename T::fp_t neutral_value) {
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
        using fp_t = typename T::fp_t;
        using reduce_find_t = std::pair<fp_t, uint>;

        template <binary_op_t F>
        static INL fp_t reduce(const T& in) { return reduce_wide<F>(in).front(); }

        template <binary_op_t F>
        static INL const T reduce_wide(const T& in) { return horizontal<T>::template reduce_wide<F>(in); }

        template <binary_op_t F>
        static INL std::pair<fp_t, uint> reduce_find(const T& in) {
            auto in_reduced = reduce_wide<F>(in);
            auto selected = horizontal<T>::find(in == in_reduced);
            return std::make_pair(in_reduced.front(), selected);
        }
    };

}

#undef INL

#endif // SIMDIFY_COMMON
