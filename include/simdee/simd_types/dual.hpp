// This file is a part of Simdee, see homepage at http://github.com/tufak/simdee
// This file is distributed under the MIT license.

#ifndef SIMDEE_SIMD_TYPES_DUAL_HPP
#define SIMDEE_SIMD_TYPES_DUAL_HPP

#include "common.hpp"
#include <utility>

namespace sd {
    namespace impl {
        template <typename T>
        struct pair {
            using l_t = T;
            using r_t = T;
            T l;
            T r;
        };

        template <typename Mask_t>
        using dual_mask_t = impl::mask<((Mask_t::all_bits + 1) * (Mask_t::all_bits + 1)) - 1>;
    }

    template <typename T, typename Enable = void>
    struct dual;

    template <typename T>
    struct is_simd_type<dual<T>> : std::integral_constant<bool, true> {};

    template <typename T>
    struct simd_type_traits<dual<T>> {
        using simd_t = dual<T>;
        using vector_t = impl::pair<T>;
        using scalar_t = typename simd_type_traits<T>::scalar_t;
        using element_t = typename simd_type_traits<T>::element_t;
        using vec_b = dual<typename simd_type_traits<T>::vec_b>;
        using vec_f = dual<typename simd_type_traits<T>::vec_f>;
        using vec_u = dual<typename simd_type_traits<T>::vec_u>;
        using vec_s = dual<typename simd_type_traits<T>::vec_s>;
        using mask_t = impl::dual_mask_t<typename simd_type_traits<T>::mask_t>;
        using storage_t = impl::storage<simd_t, scalar_t, alignof(vector_t)>;
    };

    template <typename Crtp>
    struct dual_base : simd_base<Crtp> {
    protected:
        using T = typename simd_base<Crtp>::vector_t::l_t;
        using simd_base<Crtp>::mm;

    public:
        using vector_t = typename simd_base<Crtp>::vector_t;
        using scalar_t = typename simd_base<Crtp>::scalar_t;
        using element_t = typename simd_base<Crtp>::element_t;
        using storage_t = typename simd_base<Crtp>::storage_t;
        using simd_base<Crtp>::width;
        using simd_base<Crtp>::self;

        SIMDEE_TRIVIAL_TYPE(dual_base);

        SIMDEE_BASE_CTOR(dual_base, vector_t, mm = r);
        SIMDEE_BASE_CTOR(dual_base, scalar_t, mm.l = r; mm.r = r);
        SIMDEE_BASE_CTOR_TPL(dual_base, expr::aligned<T>, aligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(dual_base, expr::unaligned<T>, unaligned_load(r.ptr));
        SIMDEE_BASE_CTOR_TPL(dual_base, expr::init<T>, mm.l = r; mm.r = r);
        SIMDEE_BASE_CTOR(dual_base, storage_t, aligned_load(r.data()));

        SIMDEE_INL void aligned_load(const scalar_t* r) {
            mm.l.aligned_load(r);
            mm.r.aligned_load(r + T::width);
        }

        SIMDEE_INL void aligned_store(scalar_t* r) const {
            mm.l.aligned_store(r);
            mm.r.aligned_store(r + T::width);
        }

        SIMDEE_INL void unaligned_load(const scalar_t* r) {
            mm.l.unaligned_load(r);
            mm.r.unaligned_load(r + T::width);
        }

        SIMDEE_INL void unaligned_store(scalar_t* r) const {
            mm.l.unaligned_store(r);
            mm.r.unaligned_store(r + T::width);
        }

        SIMDEE_INL void interleaved_load(const scalar_t* r, std::size_t step) {
            mm.l.interleaved_load(r);
            mm.r.interleaved_load(r + (T::width * step));
        }

        SIMDEE_INL void interleaved_store(scalar_t* r, std::size_t step) const {
            mm.l.interleaved_store(r);
            mm.r.interleaved_store(r + (T::width * step));
        }

        template <typename Op_t>
        const Crtp reduce(Op_t f) const {
            vector_t res;
            res.l = mm.l.reduce(f);
            res.r = mm.r.reduce(f);
            res.l = f(res.l, res.r);
            res.r = res.l;
            return res;
        }

        SIMDEE_INL element_t first_element() const { return mm.l.first_element(); }
    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_b>::value>::type> : dual_base<dual<T>> {
    private:
        using simd_base<dual<T>>::mm;

    public:
        using vector_t = typename simd_base<dual<T>>::vector_t;
        using element_t = typename simd_base<dual<T>>::element_t;
        using vec_b = typename simd_base<dual<T>>::vec_b;
        using vec_f = typename simd_base<dual<T>>::vec_f;
        using vec_u = typename simd_base<dual<T>>::vec_u;
        using vec_s = typename simd_base<dual<T>>::vec_s;
        using mask_t = typename simd_base<dual<T>>::mask_t;
        using dual_base<dual<T>>::dual_base;

        SIMDEE_TRIVIAL_TYPE(dual);
        SIMDEE_CTOR(dual, element_t, mm.l = r; mm.r = r);

        SIMDEE_INL mask_t mask() const {
            return mask_t(mm.l.mask().value | (mm.r.mask().value << T::width));
        }

        SIMDEE_BINOP(vec_b, vec_b, operator==, (vector_t{ l.mm.l == r.mm.l, l.mm.r == r.mm.r }));
        SIMDEE_BINOP(vec_b, vec_b, operator!=, (vector_t{ l.mm.l != r.mm.l, l.mm.r != r.mm.r }));
        SIMDEE_BINOP(vec_b, vec_b, operator&&, (vector_t{ l.mm.l && r.mm.l, l.mm.r && r.mm.r }));
        SIMDEE_BINOP(vec_b, vec_b, operator||, (vector_t{ l.mm.l || r.mm.l, l.mm.r || r.mm.r }));
        SIMDEE_UNOP(vec_b, vec_b, operator!, (vector_t{!l.mm.l, !l.mm.r}));
        SIMDEE_BINOP(vec_b, vec_b, andnot, (vector_t{ andnot(l.mm.l, r.mm.l), andnot(l.mm.r, r.mm.r) }));
    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_f>::value>::type> : dual_base<dual<T>> {
    private:
        using simd_base<dual<T>>::mm;
        using b_vector_t = typename simd_base<dual<T>>::vec_b::vector_t;

    public:
        using vector_t = typename simd_base<dual<T>>::vector_t;
        using vec_b = typename simd_base<dual<T>>::vec_b;
        using vec_f = typename simd_base<dual<T>>::vec_f;
        using vec_u = typename simd_base<dual<T>>::vec_u;
        using vec_s = typename simd_base<dual<T>>::vec_s;
        using dual_base<dual<T>>::dual_base;

        SIMDEE_TRIVIAL_TYPE(dual);

        SIMDEE_INL explicit dual(const vec_s& r) {
            mm.l = decltype(mm.l)(r.data().l);
            mm.r = decltype(mm.r)(r.data().r);
        }

        SIMDEE_BINOP(vec_f, vec_b, operator<, (b_vector_t{ l.mm.l < r.mm.l, l.mm.r < r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_b, operator>, (b_vector_t{ l.mm.l > r.mm.l, l.mm.r > r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_b, operator<=, (b_vector_t{ l.mm.l <= r.mm.l, l.mm.r <= r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_b, operator>=, (b_vector_t{ l.mm.l >= r.mm.l, l.mm.r >= r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_b, operator==, (b_vector_t{ l.mm.l == r.mm.l, l.mm.r == r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_b, operator!=, (b_vector_t{ l.mm.l != r.mm.l, l.mm.r != r.mm.r }));
        SIMDEE_UNOP(vec_f, vec_f, operator-, (vector_t{-l.mm.l, -l.mm.r}));
        SIMDEE_BINOP(vec_f, vec_f, operator+, (vector_t{ l.mm.l + r.mm.l, l.mm.r + r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_f, operator-, (vector_t{ l.mm.l - r.mm.l, l.mm.r - r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_f, operator*, (vector_t{ l.mm.l * r.mm.l, l.mm.r * r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_f, operator/, (vector_t{ l.mm.l / r.mm.l, l.mm.r / r.mm.r }));
        SIMDEE_BINOP(vec_f, vec_f, min, (vector_t{ min(l.mm.l, r.mm.l), min(l.mm.r, r.mm.r) }));
        SIMDEE_BINOP(vec_f, vec_f, max, (vector_t{ max(l.mm.l, r.mm.l), max(l.mm.r, r.mm.r) }));
        SIMDEE_UNOP(vec_f, vec_f, signum, (vector_t{ signum(l.mm.l), signum(l.mm.r) }));
        SIMDEE_UNOP(vec_f, vec_f, sqrt, (vector_t{ sqrt(l.mm.l), sqrt(l.mm.r) }));
        SIMDEE_UNOP(vec_f, vec_f, rsqrt, (vector_t{ rsqrt(l.mm.l), rsqrt(l.mm.r) }));
        SIMDEE_UNOP(vec_f, vec_f, rcp, (vector_t{ rcp(l.mm.l), rcp(l.mm.r) }));
        SIMDEE_UNOP(vec_f, vec_f, abs, (vector_t{ abs(l.mm.l), abs(l.mm.r) }));
    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_u>::value>::type> : dual_base<dual<T>> {
    private:
        using simd_base<dual<T>>::mm;

    public:
        using vec_b = typename simd_base<dual<T>>::vec_b;
        using vec_s = typename simd_base<dual<T>>::vec_s;

        SIMDEE_TRIVIAL_TYPE(dual);

        SIMDEE_INL explicit dual(const vec_b& r) {
            mm.l = decltype(mm.l)(r.data().l);
            mm.r = decltype(mm.r)(r.data().r);
        }

        SIMDEE_INL explicit dual(const vec_s& r) {
            mm.l = decltype(mm.l)(r.data().l);
            mm.r = decltype(mm.r)(r.data().r);
        }

        using dual_base<dual<T>>::dual_base;
    };

    template <typename T>
    struct dual<T, typename std::enable_if<std::is_same<T, typename T::vec_s>::value>::type> : dual_base<dual<T>> {
    private:
        using simd_base<dual<T>>::mm;

    public:
        using vec_f = typename simd_base<dual<T>>::vec_f;
        using vec_u = typename simd_base<dual<T>>::vec_u;

        SIMDEE_TRIVIAL_TYPE(dual);

        SIMDEE_INL explicit dual(const vec_f& r) {
            mm.l = decltype(mm.l)(r.data().l);
            mm.r = decltype(mm.r)(r.data().r);
        }

        SIMDEE_INL explicit dual(const vec_u& r) {
            mm.l = decltype(mm.l)(r.data().l);
            mm.r = decltype(mm.r)(r.data().r);
        }

        using dual_base<dual<T>>::dual_base;
    };
}

#endif // SIMDEE_SIMD_TYPES_DUAL_HPP
