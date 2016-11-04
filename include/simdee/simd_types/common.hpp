// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_COMMON_HPP
#define SIMDEE_SIMD_TYPES_COMMON_HPP

#include "../common/init.hpp"
#include "../common/casts.hpp"
#include "../common/expr.hpp"
#include "../common/mask.hpp"
#include "../common/storage.hpp"
#include "../common/deferred_not.hpp"
#include "../util/inline.hpp"
#include "../util/bitfiddle.hpp"
#include "../util/macros.hpp"
#include <array>
#include <type_traits>

namespace sd {

    template <typename T>
    struct is_simd_type : std::integral_constant<bool, false> {};
    template <typename T>
    struct simd_type_traits;

    template <typename Crtp>
    struct simd_base {
        using traits_t = simd_type_traits<Crtp>;
        using vector_t = typename traits_t::vector_t;
        using scalar_t = typename traits_t::scalar_t;
        using vec_b = typename traits_t::vec_b;
        using vec_f = typename traits_t::vec_f;
        using vec_u = typename traits_t::vec_u;
        using vec_s = typename traits_t::vec_s;
        using mask_t = typename traits_t::mask_t;
        using storage_t = typename traits_t::storage_t;

        enum : std::size_t {
            width = sizeof(vector_t) / sizeof(scalar_t)
        };

        SIMDEE_INL Crtp& self() {
            return static_cast<Crtp&>(*this);
        }

        SIMDEE_INL constexpr const Crtp& self() const {
            return static_cast<const Crtp&>(*this);
        }

        SIMDEE_INL Crtp& eval() {
            return static_cast<Crtp&>(*this);
        }

        SIMDEE_INL constexpr const Crtp& eval() const {
            return static_cast<const Crtp&>(*this);
        }

        vector_t& data() { return mm; }
        const vector_t& data() const { return mm; }

    protected:
        // data
        vector_t mm;
    };

    template <typename Simd_t, typename Rhs_t>
    SIMDEE_INL Simd_t& operator&=(simd_base<Simd_t>& l, const Rhs_t& r) {
        l.self() = l.self() & r;
        return l.self();
    }
    template <typename Simd_t, typename Rhs_t>
    SIMDEE_INL Simd_t& operator|=(simd_base<Simd_t>& l, const Rhs_t& r) {
        l.self() = l.self() | r;
        return l.self();
    }
    template <typename Simd_t, typename Rhs_t>
    SIMDEE_INL Simd_t& operator^=(simd_base<Simd_t>& l, const Rhs_t& r) {
        l.self() = l.self() ^ r;
        return l.self();
    }
    template <typename Simd_t, typename Rhs_t>
    SIMDEE_INL Simd_t& operator+=(simd_base<Simd_t>& l, const Rhs_t& r) {
        l.self() = l.self() + r;
        return l.self();
    }
    template <typename Simd_t, typename Rhs_t>
    SIMDEE_INL Simd_t& operator-=(simd_base<Simd_t>& l, const Rhs_t& r) {
        l.self() = l.self() - r;
        return l.self();
    }
    template <typename Simd_t, typename Rhs_t>
    SIMDEE_INL Simd_t& operator*=(simd_base<Simd_t>& l, const Rhs_t& r) {
        l.self() = l.self() * r;
        return l.self();
    }
    template <typename Simd_t, typename Rhs_t>
    SIMDEE_INL Simd_t& operator/=(simd_base<Simd_t>& l, const Rhs_t& r) {
        l.self() = l.self() / r;
        return l.self();
    }
    template <typename Simd_t>
    SIMDEE_INL const Simd_t operator+(const simd_base<Simd_t>& l) {
        return l.self();
    }
    template <typename Simd_t>
    SIMDEE_INL bool any(const simd_base<Simd_t>& l) {
        return any(mask(l.self()));
    }
    template <typename Simd_t>
    SIMDEE_INL bool all(const simd_base<Simd_t>& l) {
        return all(mask(l.self()));
    }

    struct op_add {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(l + r) { return l + r; }
    };
    struct op_mul {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(l * r) { return l * r; }
    };
    struct op_min {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(min(l, r)) { return min(l, r); }
    };
    struct op_max {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(max(l, r)) { return max(l, r); }
    };
    struct op_bitand {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(l & r) { return l & r; }
    };
    struct op_bitor {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(l | r) { return l | r; }
    };
    struct op_bitxor {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(l ^ r) { return l ^ r; }
    };
    struct op_logand {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(l && r) { return l && r; }
    };
    struct op_logor {
        template <typename L, typename R>
        SIMDEE_INL auto operator()(const L& l, const R& r) -> decltype(l || r) { return l || r; }
    };

}

#endif // SIMDEE_SIMD_TYPES_COMMON_HPP
