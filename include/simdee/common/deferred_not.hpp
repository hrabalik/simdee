// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_DEFERRED_NOT_HPP
#define SIMDEE_COMMON_DEFERRED_NOT_HPP

#include "../util/inline.hpp"
#include <type_traits>

namespace sd {
    namespace expr {
        template <typename T>
        struct dont_change_deferred_not {
            template <typename T2>
            struct false_when_instantiated : std::integral_constant<bool, false> {};

            static_assert(false_when_instantiated<T>::value,
                "Hello! You are trying to change a varible of type deferred_bitnot<T>. "
                "This is definitely not something you want to do. "
                "What probably happened is that you used *auto* to store a calculation result, "
                "but the type of the result is not what you were expecting. "
                "To solve the problem, you have to modify the calculation; "
                "either use the .eval() method to force evaluation to the desired type, "
                "or don't use *auto* on that particular line.");
        };

        template <typename T>
        struct deferred_bitnot {
            using vector_t = typename T::vector_t;
            using scalar_t = typename T::scalar_t;
            using mask_t = typename T::mask_t;
            using storage_t = typename T::storage_t;
            using binary_op_t = typename T::binary_op_t;
            enum : std::size_t { width = T::width };

            SIMDEE_INL constexpr explicit deferred_bitnot(const T& r) : neg(r) {}

            template <typename Rhs>
            void operator=(const Rhs& r) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL const T eval() const {
                T pos(~neg);
                return pos;
            }

            SIMDEE_INL const vector_t data() const {
                T pos(~neg);
                return pos.data();
            }

            template <typename Rhs>
            void aligned_load(const Rhs& r) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void aligned_store(scalar_t* r) const {
                T pos(~neg);
                pos.aligned_store(r);
            }

            template <typename Rhs>
            void unaligned_load(const Rhs& r) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void unaligned_store(scalar_t* r) const {
                T pos(~neg);
                pos.unaligned_store(r);
            }

            template <typename Rhs>
            void interleaved_load(const Rhs& r, std::size_t step) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void interleaved_store(scalar_t* r, std::size_t step) const {
                T pos(~neg);
                pos.interleaved_store(r, step);
            }

            SIMDEE_INL const T reduce(binary_op_t f) const {
                T pos(~neg);
                return pos.reduce(f);
            }

            SIMDEE_INL typename T::mask_t mask() const {
                return ~neg.mask();
            }

            SIMDEE_INL typename T::element_t first_element() const {
                return ~neg.first_element();
            }

            // data
            const T neg;
        };

        template<typename T>
        SIMDEE_INL constexpr const T operator~(const deferred_bitnot<T>& l) {
            return l.neg;
        }
        template<typename T>
        SIMDEE_INL const T operator&(const deferred_bitnot<T>& l, const T& r) {
            return andnot(r, l.neg);
        }
        template<typename T>
        SIMDEE_INL const T operator&(const T& l, const deferred_bitnot<T>& r) {
            return andnot(l, r.neg);
        }
        template<typename T>
        SIMDEE_INL const deferred_bitnot<T> operator&(const deferred_bitnot<T>& l, const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(l.neg | r.neg);
        }
        template<typename T>
        SIMDEE_INL const deferred_bitnot<T> operator|(const deferred_bitnot<T>& l, const T& r) {
            return deferred_bitnot<T>(andnot(l.neg, r));
        }
        template<typename T>
        SIMDEE_INL const deferred_bitnot<T> operator|(const T& l, const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(andnot(r.neg, l));
        }
        template<typename T>
        SIMDEE_INL const deferred_bitnot<T> operator|(const deferred_bitnot<T>& l, const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(l.neg & r.neg);
        }
        template<typename T>
        SIMDEE_INL const deferred_bitnot<T> operator^(const deferred_bitnot<T>& l, const T& r) {
            return deferred_bitnot<T>(l.neg ^ r);
        }
        template<typename T>
        SIMDEE_INL const deferred_bitnot<T> operator^(const T& l, const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(l ^ r.neg);
        }
        template<typename T>
        SIMDEE_INL const T operator^(const deferred_bitnot<T>& l, const deferred_bitnot<T>& r) {
            return l.neg ^ r.neg;
        }
        template<typename T>
        SIMDEE_INL const deferred_bitnot<T> andnot(const deferred_bitnot<T>& l, const T& r) {
            return deferred_bitnot<T>(l.neg | r);
        }
        template<typename T>
        SIMDEE_INL const T andnot(const T& l, const deferred_bitnot<T>& r) {
            return l & r.neg;
        }
        template<typename T>
        SIMDEE_INL const T andnot(const deferred_bitnot<T>& l, const deferred_bitnot<T>& r) {
            return andnot(r.neg, l.neg);
        }
        template <typename T, typename Rhs>
        SIMDEE_INL void operator&=(const deferred_bitnot<T>& l, const Rhs& r) {
            dont_change_deferred_not<Rhs> fail;
        }
        template <typename T, typename Rhs>
        SIMDEE_INL void operator|=(const deferred_bitnot<T>& l, const Rhs& r) {
            dont_change_deferred_not<Rhs> fail;
        }
        template <typename T, typename Rhs>
        SIMDEE_INL void operator^=(const deferred_bitnot<T>& l, const Rhs& r) {
            dont_change_deferred_not<Rhs> fail;
        }

        template <typename T>
        struct deferred_lognot {
            using vector_t = typename T::vector_t;
            using scalar_t = typename T::scalar_t;
            using mask_t = typename T::mask_t;
            using storage_t = typename T::storage_t;
            using binary_op_t = typename T::binary_op_t;
            enum : std::size_t { width = T::width };

            SIMDEE_INL constexpr explicit deferred_lognot(const T& r) : neg(r) {}

            template <typename Rhs>
            void operator=(const Rhs& r) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL const T eval() const {
                T pos(!neg);
                return pos;
            }

            SIMDEE_INL const vector_t data() const {
                T pos(!neg);
                return pos.data();
            }

            template <typename Rhs>
            void aligned_load(const Rhs& r) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void aligned_store(scalar_t* r) const {
                T pos(!neg);
                pos.aligned_store(r);
            }

            template <typename Rhs>
            void unaligned_load(const Rhs& r) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void unaligned_store(scalar_t* r) const {
                T pos(!neg);
                pos.unaligned_store(r);
            }

            template <typename Rhs>
            void interleaved_load(const Rhs& r, std::size_t step) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void interleaved_store(scalar_t* r, std::size_t step) const {
                T pos(!neg);
                pos.interleaved_store(r, step);
            }

            SIMDEE_INL const T reduce(binary_op_t f) const {
                T pos(!neg);
                return pos.reduce(f);
            }

            SIMDEE_INL typename T::mask_t mask() const {
                return ~neg.mask();
            }

            SIMDEE_INL typename T::element_t first_element() const {
                return !neg.first_element();
            }

            // data
            const T neg;
        };

        template<typename T>
        SIMDEE_INL bool any(const deferred_lognot<T>& l) {
            return !l.neg.mask().all();
        }
        template<typename T>
        SIMDEE_INL bool all(const deferred_lognot<T>& l) {
            return !l.neg.mask().any();
        }
        template<typename T>
        SIMDEE_INL constexpr const T operator!(const deferred_lognot<T>& l) {
            return l.neg;
        }
        template<typename T>
        SIMDEE_INL const T operator&&(const deferred_lognot<T>& l, const T& r) {
            return andnot(r, l.neg);
        }
        template<typename T>
        SIMDEE_INL const T operator&&(const T& l, const deferred_lognot<T>& r) {
            return andnot(l, r.neg);
        }
        template<typename T>
        SIMDEE_INL const deferred_lognot<T> operator&&(const deferred_lognot<T>& l, const deferred_lognot<T>& r) {
            return deferred_lognot<T>(l.neg || r.neg);
        }
        template<typename T>
        SIMDEE_INL const deferred_lognot<T> operator||(const deferred_lognot<T>& l, const T& r) {
            return deferred_lognot<T>(andnot(l.neg, r));
        }
        template<typename T>
        SIMDEE_INL const deferred_lognot<T> operator||(const T& l, const deferred_lognot<T>& r) {
            return deferred_lognot<T>(andnot(r.neg, l));
        }
        template<typename T>
        SIMDEE_INL const deferred_lognot<T> operator||(const deferred_lognot<T>& l, const deferred_lognot<T>& r) {
            return deferred_lognot<T>(l.neg && r.neg);
        }
        template<typename T>
        SIMDEE_INL const deferred_lognot<T> andnot(const deferred_lognot<T>& l, const T& r) {
            return deferred_lognot<T>(l.neg || r);
        }
        template<typename T>
        SIMDEE_INL const T andnot(const T& l, const deferred_lognot<T>& r) {
            return l && r.neg;
        }
        template<typename T>
        SIMDEE_INL const T andnot(const deferred_lognot<T>& l, const deferred_lognot<T>& r) {
            return andnot(r.neg, l.neg);
        }
    }
}

#endif // SIMDEE_COMMON_DEFERRED_NOT_HPP
