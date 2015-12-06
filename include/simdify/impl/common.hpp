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

    //
    // SIMD type base class (with derived class as Crtp, CRTP-style)
    //
    template <typename Mm_t_, typename F_t_, typename Crtp>
    struct simd_base {
        using mm_t = Mm_t_;
        using f_t = F_t_;
        using u_t = select_uint_t<sizeof(f_t)>;
        using s_t = select_sint_t<sizeof(f_t)>;

        enum : std::size_t {
            W = sizeof(mm_t) / sizeof(f_t)
        };
        using array_f = std::array<f_t, W>;
        using array_u = std::array<u_t, W>;
        using array_s = std::array<s_t, W>;
        using horizontal = horizontal_impl<Crtp>;

        // data
        union { mm_t mm; array_f f; array_u u; array_s i; };

        // constructor
        SIMDIFY_FORCE_INLINE simd_base() {}
        SIMDIFY_FORCE_INLINE simd_base(mm_t r) : mm(r) {}
    };

    //
    // storage for SIMD or floating-point types
    //
    template <typename T, typename Enable = void>
    struct storage;

    //
    // specialized storage for SIMD types
    //
    template <typename Simd_t>
    struct alignas(Simd_t)storage<Simd_t, typename std::enable_if<is_simd_type<Simd_t>::value>::type> {
        using f_t = typename Simd_t::f_t;
        using data_t = std::array<f_t, Simd_t::W>;

        SIMDIFY_FORCE_INLINE constexpr storage() = default;
        SIMDIFY_FORCE_INLINE constexpr storage(const storage&) = default;
        SIMDIFY_FORCE_INLINE constexpr explicit storage(const data_t& rhs) : m_data(rhs) {}

        SIMDIFY_FORCE_INLINE explicit storage(const Simd_t& rhs) {
            rhs.store(data());
        }

        SIMDIFY_FORCE_INLINE storage& operator=(const storage&) = default;

        SIMDIFY_FORCE_INLINE storage& operator=(const Simd_t& rhs) {
            rhs.store(data());
            return *this;
        }

        SIMDIFY_FORCE_INLINE storage& operator=(const data_t& rhs) {
            m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE f_t* data() { return m_data.data(); }
        SIMDIFY_FORCE_INLINE const f_t* data() const { return m_data.data(); }
        SIMDIFY_FORCE_INLINE f_t& operator[](std::size_t i) { return m_data[i]; }
        SIMDIFY_FORCE_INLINE const f_t& operator[](std::size_t i) const { return m_data[i]; }

        // implicit conversion to Simd_t
        SIMDIFY_FORCE_INLINE operator Simd_t() const { Simd_t s; s.load(data()); return s; }

        // data
        data_t m_data;
    };

    //
    // specialized storage for floating point types
    //
    template <typename F_t>
    struct storage<F_t, typename std::enable_if<std::is_floating_point<F_t>::value>::type> {
        SIMDIFY_FORCE_INLINE constexpr storage() = default;
        SIMDIFY_FORCE_INLINE constexpr storage(const storage&) = default;
        SIMDIFY_FORCE_INLINE explicit storage(const F_t& rhs) : m_data(rhs) {}

        SIMDIFY_FORCE_INLINE storage& operator=(const storage&) = default;

        SIMDIFY_FORCE_INLINE storage& operator=(const F_t& rhs) {
            m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE F_t* data() { return &m_data; }
        SIMDIFY_FORCE_INLINE const F_t* data() const { return &m_data; }

        // implicit conversion to F_t
        SIMDIFY_FORCE_INLINE operator F_t() const { return m_data; }

        // data
        F_t m_data;
    };

    //
    // reference to storage (proxy object)
    //
    template <typename T>
    struct reference {
        using storage_t = storage<T>;

        SIMDIFY_FORCE_INLINE constexpr reference() = default;
        SIMDIFY_FORCE_INLINE constexpr reference(const reference&) = default;

        SIMDIFY_FORCE_INLINE reference& operator=(const reference& rhs) {
            *m_data = *rhs.m_data;
            return *this;
        }

        SIMDIFY_FORCE_INLINE reference& operator=(const storage<T>& rhs) {
            *m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE reference& operator=(const T& rhs) {
            *m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE storage_t* reset(void* ptr) { m_data = static_cast<storage_t*>(ptr); return m_data; }
        SIMDIFY_FORCE_INLINE storage_t*& ptr() { return m_data; }
        SIMDIFY_FORCE_INLINE storage_t* ptr() const { return m_data; }

        // implicit conversion to const T
        SIMDIFY_FORCE_INLINE operator const T() const { return T(*m_data); }

        // data
        storage_t* m_data;
    };

    //
    // const reference to storage (proxy object)
    //
    template <typename T>
    struct const_reference {
        using storage_t = const storage<T>;

        SIMDIFY_FORCE_INLINE constexpr const_reference() = default;
        SIMDIFY_FORCE_INLINE constexpr const_reference(const const_reference&) = default;

        // no assignment operations

        SIMDIFY_FORCE_INLINE storage_t* reset(void* ptr) { m_data = static_cast<storage_t*>(ptr); return m_data; }
        SIMDIFY_FORCE_INLINE storage_t*& ptr() { return m_data; }
        SIMDIFY_FORCE_INLINE storage_t* ptr() const { return m_data; }

        // implicit conversion to const T
        SIMDIFY_FORCE_INLINE operator const T() const { return T(*m_data); }

        // data
        storage_t* m_data;
    };

    //
    // meta operations - apply to all SIMD types
    //
    template <typename T> SIMDIFY_FORCE_INLINE const T operator+(const T& in) { return in; }
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
