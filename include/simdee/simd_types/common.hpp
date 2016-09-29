// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_COMMON_HPP
#define SIMDEE_SIMD_TYPES_COMMON_HPP

#include "../util/inline.hpp"
#include "../util/bitfiddle.hpp"
#include "../util/macros.hpp"
#include "../common/expr.hpp"
#include "../common/mask.hpp"
#include "../common/storage.hpp"
#include "../common/deferred_not.hpp"
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
        using mask_t = typename traits_t::mask_t;
        using storage_t = typename traits_t::storage_t;
        using binary_op_t = const Crtp(*)(const Crtp& l, const Crtp& r);

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
    SIMDEE_INL const Simd_t signum(const simd_base<Simd_t>& l) {
        return cond(l.self() > zero(), Simd_t(1), Simd_t(-1));
    }
    template <typename Simd_t>
    SIMDEE_INL bool any(const simd_base<Simd_t>& l) {
        return l.self().mask().any();
    }
    template <typename Simd_t>
    SIMDEE_INL bool all(const simd_base<Simd_t>& l) {
        return l.self().mask().all();
    }

}

#endif // SIMDEE_SIMD_TYPES_COMMON_HPP
