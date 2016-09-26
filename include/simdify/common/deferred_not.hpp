#ifndef SIMDIFY_COMMON_DEFERRED_NOT_HPP
#define SIMDIFY_COMMON_DEFERRED_NOT_HPP

#include "../util/inline.hpp"
#include <type_traits>

namespace simd {
    namespace expr {
        template <typename T>
        struct dont_assign_to_deferred_not {
            template <typename T2>
            struct false_when_instantiated : std::integral_constant<bool, false> {};

            static_assert(false_when_instantiated<T>::value,
                "Hello! You are trying to assign to a varible of type deferred_not<T>. "
                "This is definitely not something you want to do. "
                "What probably happened is that you used *auto* to store a calculation result, "
                "but the type of the result is not what you expect. "
                "To solve the problem, don't use *auto* on that particular line.");
        };

        template <typename T>
        struct deferred_not {
            SIMDIFY_INL constexpr explicit deferred_not(const T& r) : neg(r) {}

            template <typename Rhs>
            SIMDIFY_INL void operator=(const Rhs& r) {
                dont_assign_to_deferred_not<Rhs> fail;
            }

            SIMDIFY_INL typename T::mask_t mask() const { return ~neg.mask(); }
            SIMDIFY_INL typename T::scalar_t first_element() const { return ~neg.first_element(); }

            SIMDIFY_INL const T reduce(typename T::binary_op_t f) {
                T pos = ~neg;
                return pos.reduce(f);
            }

            // data
            const T neg;
        };

        template<typename T>
        SIMDIFY_INL constexpr const T operator~(const deferred_not<T>& l) {
            return l.neg;
        }
        template<typename T>
        SIMDIFY_INL const T operator&(const deferred_not<T>& l, const T& r) {
            return andnot(r, l.neg);
        }
        template<typename T>
        SIMDIFY_INL const T operator&(const T& l, const deferred_not<T>& r) {
            return andnot(l, r.neg);
        }
        template<typename T>
        SIMDIFY_INL const deferred_not<T> operator&(const deferred_not<T>& l, const deferred_not<T>& r) {
            return deferred_not<T>(l.neg | r.neg);
        }
        template<typename T>
        SIMDIFY_INL const deferred_not<T> operator|(const deferred_not<T>& l, const T& r) {
            return deferred_not<T>(andnot(l.neg, r));
        }
        template<typename T>
        SIMDIFY_INL const deferred_not<T> operator|(const T& l, const deferred_not<T>& r) {
            return deferred_not<T>(andnot(r.neg, l));
        }
        template<typename T>
        SIMDIFY_INL const deferred_not<T> operator|(const deferred_not<T>& l, const deferred_not<T>& r) {
            return deferred_not<T>(l.neg & r.neg);
        }
        template<typename T>
        SIMDIFY_INL const deferred_not<T> operator^(const deferred_not<T>& l, const T& r) {
            return deferred_not<T>(l.neg ^ r);
        }
        template<typename T>
        SIMDIFY_INL const deferred_not<T> operator^(const T& l, const deferred_not<T>& r) {
            return deferred_not<T>(l ^ r.neg);
        }
        template<typename T>
        SIMDIFY_INL const T operator^(const deferred_not<T>& l, const deferred_not<T>& r) {
            return l.neg ^ r.neg;
        }
        template<typename T>
        SIMDIFY_INL bool any(const deferred_not<T>& l) {
            return !l.neg.mask().all();
        }
        template<typename T>
        SIMDIFY_INL bool all(const deferred_not<T>& l) {
            return !l.neg.mask().any();
        }
        template <typename T, typename Rhs>
        SIMDIFY_INL void operator&=(const deferred_not<T>& l, const Rhs& r) {
            dont_assign_to_deferred_not<Rhs> fail;
        }
        template <typename T, typename Rhs>
        SIMDIFY_INL void operator|=(const deferred_not<T>& l, const Rhs& r) {
            dont_assign_to_deferred_not<Rhs> fail;
        }
        template <typename T, typename Rhs>
        SIMDIFY_INL void operator^=(const deferred_not<T>& l, const Rhs& r) {
            dont_assign_to_deferred_not<Rhs> fail;
        }
    }
}

#endif // SIMDIFY_COMMON_DEFERRED_NOT_HPP
