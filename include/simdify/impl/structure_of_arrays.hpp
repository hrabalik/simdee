#ifndef SIMDIFY_STRUCTURE_OF_ARRAYS
#define SIMDIFY_STRUCTURE_OF_ARRAYS

#include "../util/inline.hpp"
#include "../util/malloc.hpp"
#include "../util/integral.hpp"
#include "expr.hpp"
#include "named_array.hpp"
#include <tuple>
#include <memory>
#include <exception>
#include <cstring>
#include <iterator>

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

        static_assert(std::is_trivial<f_t>::value, "structure_of_arrays_impl: f_t not trivial");

        using value_type = named_array<f_t, Ids...>;
        using value_type_vector = named_array<mm_t, Ids...>;
        using reference = named_array<simd::reference<f_t>, Ids...>;
        using reference_vector = named_array<simd::reference<Simd_t>, Ids...>;
        using const_reference = named_array<simd::const_reference<f_t>, Ids...>;
        using const_reference_vector = named_array<simd::const_reference<Simd_t>, Ids...>;

        enum : std::size_t { N = sizeof...(Ids), W = simd_t::W };

        template <typename Ref>
        struct iterator_impl : std::iterator<std::forward_iterator_tag, Ref> {
            iterator_impl(const self_t& self, std::size_t idx) {
                detail::no_op(simd::get<I>(m_ref).reset(self.m_data.get() + I*self.m_cap + idx)...);
            }

            iterator_impl& operator++() {
                detail::no_op(simd::get<I>(m_ref).ptr()++...);
                return *this;
            }

            bool operator==(const iterator_impl& rhs) const { return m_ref.get().ptr() == rhs.m_ref.get().ptr(); }
            bool operator!=(const iterator_impl& rhs) const { return m_ref.get().ptr() != rhs.m_ref.get().ptr(); }
            Ref& operator*() { return m_ref; }
            Ref* operator->() { return &m_ref; }

        private:
            Ref m_ref;
        };

        using iterator = iterator_impl<reference>;
        using iterator_vector = iterator_impl<reference_vector>;
        using const_iterator = iterator_impl<const_reference>;
        using const_iterator_vector = iterator_impl<const_reference_vector>;

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

        std::size_t capacity() const { return m_cap; }
        std::size_t size() const { return m_sz; }
        std::size_t size_head() const { return div_floor_shift<W>(m_sz); }
        std::size_t size_vector() const { return div_ceil_shift<W>(m_sz); }
        std::size_t size_tail() const { return size() - size_head(); }

        void reserve(std::size_t count) {
            if (count <= m_cap) return;

            std::size_t new_cap = m_cap;
            if (new_cap == 0) {
                new_cap = div_ceil_shift<W>(count);
            }
            else do { new_cap *= 2; } while (new_cap < count);

            decltype(m_data) new_data(aligned_malloc<f_t, alignof(mm_t)>(N * new_cap), aligned_deleter{});
            if (!new_data) throw std::bad_alloc{};

            if (m_sz != 0) {
                detail::no_op(std::memcpy(new_data.get() + I*new_cap, m_data.get() + I*m_cap, sizeof(f_t)*m_sz)...);
            }

            std::swap(m_data, new_data);
            m_cap = new_cap;
        }

        void fill(const value_type& val, std::size_t from = 0) {
            f_t* first = m_data.get() + from;
            f_t* last = first + m_sz;
            for (int n = 0; n < N; ++n, first += m_cap, last += m_cap) {
                f_t f = val[n];
                for (f_t* curr = first; curr != last; ++curr) {
                    *curr = f;
                }
            }
        }

        void resize(std::size_t count) {
            reserve(count);
            m_sz = count;
        }

        void resize(std::size_t count, const value_type& val) {
            auto sz = m_sz;
            resize(count);
            if (count > sz) fill(val, sz);
        }

        void clear() { m_sz = 0; }

        void push_back(const value_type& val) {
            reserve(m_sz + 1);
            f_t* base = m_data.get() + m_sz;
            detail::no_op(*(base + I*m_cap) = simd::get<I>(val)...);
            ++m_sz;
        }

        reference back() {
            f_t* base = m_data.get() + m_sz;
            return std::forward_as_tuple(*(base + I*m_cap)...);
        }

        const_reference back() const {
            f_t* base = m_data.get() + m_sz;
            return std::forward_as_tuple(*(base + I*m_cap)...);
        }

        void pop_back() {
            if (m_sz == 0) throw std::runtime_error("structure_of_arrays: pop_back()");
            --m_sz;
        }

        structure_of_arrays_impl() : m_data(nullptr, aligned_deleter{}), m_sz(0), m_cap(0) {}

        structure_of_arrays_impl(std::size_t count) : structure_of_arrays_impl() {
            reserve(div_ceil_shift<W>(count));
            m_sz = count;
        }

        structure_of_arrays_impl(std::size_t count, const value_type& val)
            : structure_of_arrays_impl(count) {
            fill(val);
        }

        structure_of_arrays_impl(structure_of_arrays_impl&& rhs) :
            m_data(std::move(rhs.m_data)), m_sz(rhs.m_sz), m_cap(rhs.m_cap) {
            rhs.m_sz = 0; rhs.m_cap = 0;
        }


    private:
        std::unique_ptr<f_t, aligned_deleter> m_data;
        std::size_t m_sz;
        std::size_t m_cap;
    };

    template <typename Simd_t, id... Ids>
    using structure_of_arrays = structure_of_arrays_impl<Simd_t, id_sequence<Ids...>, make_sequence_t<0, sizeof...(Ids)>>;
}

#endif // SIMDIFY_STRUCTURE_OF_ARRAYS
