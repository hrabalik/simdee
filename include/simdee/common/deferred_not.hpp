// This file is a part of Simdee, see homepage at http://github.com/hrabalik/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_COMMON_DEFERRED_NOT_HPP
#define SIMDEE_COMMON_DEFERRED_NOT_HPP

#include "../util/inline.hpp"
#include <type_traits>

namespace sd {
    namespace impl {
        template <typename T>
        struct special_traits;
    }

    namespace expr {
        template <typename T>
        struct dont_change_deferred_not {
            template <typename T2>
            struct false_when_instantiated : std::integral_constant<bool, false> {};

            static_assert(
                false_when_instantiated<T>::value,
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
            void interleaved_load(const Rhs& r, int step) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void interleaved_store(scalar_t* r, int step) const {
                T pos(~neg);
                pos.interleaved_store(r, step);
            }

            SIMDEE_INL friend typename T::scalar_t first_scalar(const deferred_bitnot& l) {
                return ~first_scalar(l.neg);
            }

            // data
            const T neg;
        };

        template <typename T>
        SIMDEE_INL constexpr const T operator~(const deferred_bitnot<T>& l) {
            return l.neg;
        }
        template <typename T>
        SIMDEE_INL const T operator&(const deferred_bitnot<T>& l, const T& r) {
            return impl::special_traits<T>::andnot(r, l.neg);
        }
        template <typename T>
        SIMDEE_INL const T operator&(const T& l, const deferred_bitnot<T>& r) {
            return impl::special_traits<T>::andnot(l, r.neg);
        }
        template <typename T>
        SIMDEE_INL const deferred_bitnot<T> operator&(const deferred_bitnot<T>& l,
                                                      const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(l.neg | r.neg);
        }
        template <typename T>
        SIMDEE_INL const deferred_bitnot<T> operator|(const deferred_bitnot<T>& l, const T& r) {
            return deferred_bitnot<T>(impl::special_traits<T>::andnot(l.neg, r));
        }
        template <typename T>
        SIMDEE_INL const deferred_bitnot<T> operator|(const T& l, const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(impl::special_traits<T>::andnot(r.neg, l));
        }
        template <typename T>
        SIMDEE_INL const deferred_bitnot<T> operator|(const deferred_bitnot<T>& l,
                                                      const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(l.neg & r.neg);
        }
        template <typename T>
        SIMDEE_INL const deferred_bitnot<T> operator^(const deferred_bitnot<T>& l, const T& r) {
            return deferred_bitnot<T>(l.neg ^ r);
        }
        template <typename T>
        SIMDEE_INL const deferred_bitnot<T> operator^(const T& l, const deferred_bitnot<T>& r) {
            return deferred_bitnot<T>(l ^ r.neg);
        }
        template <typename T>
        SIMDEE_INL const T operator^(const deferred_bitnot<T>& l, const deferred_bitnot<T>& r) {
            return l.neg ^ r.neg;
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
            void interleaved_load(const Rhs& r, int step) {
                dont_change_deferred_not<Rhs> fail;
            }

            SIMDEE_INL void interleaved_store(scalar_t* r, int step) const {
                T pos(!neg);
                pos.interleaved_store(r, step);
            }

            SIMDEE_INL friend typename T::mask_t mask(const deferred_lognot& l) {
                return ~mask(l.neg);
            }

            SIMDEE_INL friend typename T::scalar_t first_scalar(const deferred_lognot& l) {
                return !first_scalar(l.neg);
            }

            // data
            const T neg;
        };

        template <typename T>
        SIMDEE_INL bool any(const deferred_lognot<T>& l) {
            return !all(mask(l.neg));
        }
        template <typename T>
        SIMDEE_INL bool all(const deferred_lognot<T>& l) {
            return !any(mask(l.neg));
        }
        template <typename T>
        SIMDEE_INL constexpr const T operator!(const deferred_lognot<T>& l) {
            return l.neg;
        }
        template <typename T>
        SIMDEE_INL const T operator&&(const deferred_lognot<T>& l, const T& r) {
            return impl::special_traits<T>::andnot(r, l.neg);
        }
        template <typename T>
        SIMDEE_INL const T operator&&(const T& l, const deferred_lognot<T>& r) {
            return impl::special_traits<T>::andnot(l, r.neg);
        }
        template <typename T>
        SIMDEE_INL const deferred_lognot<T> operator&&(const deferred_lognot<T>& l,
                                                       const deferred_lognot<T>& r) {
            return deferred_lognot<T>(l.neg || r.neg);
        }
        template <typename T>
        SIMDEE_INL const deferred_lognot<T> operator||(const deferred_lognot<T>& l, const T& r) {
            return deferred_lognot<T>(impl::special_traits<T>::andnot(l.neg, r));
        }
        template <typename T>
        SIMDEE_INL const deferred_lognot<T> operator||(const T& l, const deferred_lognot<T>& r) {
            return deferred_lognot<T>(impl::special_traits<T>::andnot(r.neg, l));
        }
        template <typename T>
        SIMDEE_INL const deferred_lognot<T> operator||(const deferred_lognot<T>& l,
                                                       const deferred_lognot<T>& r) {
            return deferred_lognot<T>(l.neg && r.neg);
        }
        template <typename T, typename True, typename False>
        SIMDEE_INL auto cond(const deferred_lognot<T>& l, True t, False f)
            -> decltype(cond(l.neg, f, t)) {
            return cond(l.neg, f, t);
        }
    }
}

#endif // SIMDEE_COMMON_DEFERRED_NOT_HPP
