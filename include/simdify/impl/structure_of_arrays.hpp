#ifndef SIMDIFY_STRUCTURE_OF_ARRAYS
#define SIMDIFY_STRUCTURE_OF_ARRAYS

#include "../util/inline.hpp"
#include "../util/malloc.hpp"
#include "../util/integral.hpp"
#include "expr.hpp"
#include "named_array.hpp"
#include <array>
#include <vector>
#include <tuple>

namespace simd {

    template <typename Simd_t, typename Ids, typename Sequence>
    struct structure_of_arrays_impl;

    template <typename Simd_t, id... Ids, std::size_t... I>
    struct structure_of_arrays_impl<Simd_t, id_sequence<Ids...>, sequence<I...>> {
        static_assert(sizeof...(Ids) == sizeof...(I), "structure_of_arrays_impl: sequence size mismatch");

        using self_t = structure_of_arrays_impl;
        using simd_t = Simd_t;
        using f_t = typename simd_t::f_t;
        using mm_t = typename simd_t::mm_t;

        using value_type = named_array<f_t, Ids...>;
        using value_type_vector = named_array<mm_t, Ids...>;
        using reference = named_array<f_t&, Ids...>;
        using reference_vector = named_array<expr::aligned<f_t>, Ids...>;
        using const_reference = named_array<const f_t&, Ids...>;
        using const_reference_vector = named_array<expr::aligned<const f_t>, Ids...>;

        enum : std::size_t { N = sizeof...(Ids), W = simd_t::W };

        template <typename Ref, std::size_t Step>
        struct iterator_impl {
            iterator_impl(self_t& self, std::size_t idx) :
                m_buf(std::forward_as_tuple(*(std::get<I>(self.m_data).data() + idx)...)) {}

            iterator_impl& operator++() {
                detail::no_op(simd::get<I>(m_buf).ptr += Step...);
                return *this;
            }

            bool operator==(const iterator_impl& rhs) const { return m_buf.get().ptr == rhs.m_buf.get().ptr; }
            bool operator!=(const iterator_impl& rhs) const { return m_buf.get().ptr != rhs.m_buf.get().ptr; }
            Ref& operator*() { return m_buf; }
            Ref* operator->() { return &m_buf; }

        private:
            Ref m_buf;
        };

        template <typename Ref, std::size_t Step>
        struct iterator_vector_impl {
            iterator_vector_impl(self_t& self, std::size_t idx) :
                m_buf(std::forward_as_tuple(std::get<I>(self.m_data).data() + idx...)) {}

            iterator_vector_impl& operator++() {
                detail::no_op(simd::get<I>(m_buf).ptr += Step...);
                return *this;
            }

            bool operator==(const iterator_vector_impl& rhs) const { return m_buf.get().ptr == rhs.m_buf.get().ptr; }
            bool operator!=(const iterator_vector_impl& rhs) const { return m_buf.get().ptr != rhs.m_buf.get().ptr; }
            Ref& operator*() { return m_buf; }
            Ref* operator->() { return &m_buf; }

        private:
            Ref m_buf;
        };

        using iterator = iterator_impl<reference, 1>;
        using iterator_vector = iterator_vector_impl<reference_vector, W>;
        using const_iterator = iterator_impl<const_reference, 1>;
        using const_iterator_vector = iterator_vector_impl<const_reference_vector, W>;

        std::size_t size() { return m_data.get().size(); }
        std::size_t size_head() { return div_floor_shift<W>(size()); }
        std::size_t size_vector() { return div_ceil_shift<W>(size()); }

        iterator begin() { return iterator(*this, 0); }
        iterator end() { return iterator(*this, size()); }
        iterator_vector begin_vector() { return iterator_vector(*this, 0); }
        iterator_vector end_vector() { return iterator_vector(*this, size_vector()); }
        iterator_vector begin_head() { return iterator_vector(*this, 0); }
        iterator_vector end_head() { return iterator_vector(*this, size_head()); }
        iterator begin_tail() { return iterator(*this, size_head()); }
        iterator end_tail() { return iterator(*this, size()); }

        const_iterator begin() const { return cbegin(); }
        const_iterator end() const { return cend(); }
        const_iterator_vector begin_vector() const { return cbegin_vector(); }
        const_iterator_vector end_vector() const { return cend_vector(); }
        const_iterator_vector begin_head() const { return cbegin_head(); }
        const_iterator_vector end_head() const { return cend_head(); }
        const_iterator begin_tail() const { return cbegin_tail(); }
        const_iterator end_tail() const { return cend_tail(); }

        const_iterator cbegin() const { return const_iterator(*this, 0); }
        const_iterator cend() const { return const_iterator(*this, size()); }
        const_iterator_vector cbegin_vector() const { return const_iterator_vector(*this, 0); }
        const_iterator_vector cend_vector() const { return const_iterator_vector(*this, size_vector()); }
        const_iterator_vector cbegin_head() const { return const_iterator_vector(*this, 0); }
        const_iterator_vector cend_head() const { return const_iterator_vector(*this, size_head()); }
        const_iterator cbegin_tail() const { return const_iterator(*this, size_head()); }
        const_iterator cend_tail() const { return const_iterator(*this, size()); }
    private:
        std::array<std::vector<f_t, aligned_allocator<f_t, alignof(mm_t)>>, N> m_data;
        std::size_t m_cap;
    };

    template <typename Simd_t, id... Ids>
    struct structure_of_arrays :
        structure_of_arrays_impl<Simd_t, id_sequence<Ids...>, make_sequence_t<0, sizeof...(Ids)>> {};
}

#endif // SIMDIFY_STRUCTURE_OF_ARRAYS
