#ifndef SIMDIFY_STRUCTURE_OF_ARRAYS
#define SIMDIFY_STRUCTURE_OF_ARRAYS

#include "../simdify.hpp"
#include "../containers.hpp"
#include "../util/inline.hpp"
#include "../util/malloc.hpp"
#include "../util/integral.hpp"
#include "expr.hpp"
#include "named_array.hpp"
#include <array>
#include <vector>

namespace simd {

    template <typename Simd_t, typename Ids, typename Sequence>
    struct structure_of_arrays_impl;

    template <typename Simd_t, id... Ids, std::size_t... I>
    struct structure_of_arrays_impl<Simd_t, id_sequence<Ids...>, sequence<I...>> {
        static_assert(sizeof...(Ids) == sizeof...(I), "structure_of_arrays_impl: sequence size mismatch");
        using simd_t = Simd_t;
        using f_t = typename simd_t::f_t;
        using mm_t = typename simd_t::mm_t;
        enum : std::size_t { N = sizeof...(Ids), W = simd_t::W };

        struct iterator {
            using buf_t = named_array<expr::aligned<f_t>, Ids...>;

            iterator(structure_of_arrays_impl& self, std::size_t idx) :
                m_buf(std::get<I>(self.m_data).data() + idx...) {}

            iterator& operator++() {
                detail::no_op(simd::get<I>(m_buf).ptr += W...);
                return *this;
            }

            bool operator==(const iterator& rhs) const { return simd::get<0>(m_buf).ptr == simd::get<0>(rhs.m_buf).ptr; }
            bool operator!=(const iterator& rhs) const { return simd::get<0>(m_buf).ptr != simd::get<0>(rhs.m_buf).ptr; }
            buf_t& operator*() { return m_buf; }
            buf_t* operator->() { return &m_buf; }

        private:
            buf_t m_buf;
        };

        std::size_t size() { return std::get<0>(m_data).size(); }
        iterator begin() { return iterator(*this, 0); }
        iterator end() { return iterator(*this, div_ceil_shift<W>(size())); }
    private:
        std::array<std::vector<f_t, aligned_allocator<f_t, alignof(mm_t)>>, N> m_data;
    };

    template <typename Simd_t, id... Ids>
    struct structure_of_arrays :
        structure_of_arrays_impl<Simd_t, id_sequence<Ids...>, make_sequence_t<0, sizeof...(Ids)>> {};
}

#endif // SIMDIFY_STRUCTURE_OF_ARRAYS
