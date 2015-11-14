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
    template <typename Simd_t, id... Ids>
    struct structure_of_arrays;

    template <typename Mm_t, typename F_t, typename Simd_t, id...Ids>
    struct structure_of_arrays<simd_base<Mm_t, F_t, Simd_t>, Ids...> {
        using simd_t = Simd_t;
        using f_t = F_t;
        using mm_t = Mm_t;
        enum : std::size_t { N = sizeof...(Ids), W = simd_t::W };

        struct iterator {
            using buffer_t = named_array<expr::aligned<f_t>, Ids...>;

            template <std::size_t... I>
            SIMDIFY_FORCE_INLINE iterator(structure_of_arrays& from, std::size_t idx, sequence<I...>) :
                m_buf(simd::get<I>(from.m_data)[idx]...) {}

            iterator(structure_of_arrays& from, std::size_t idx) :
                iterator(from, idx, buffer_t::all_elements{}) {}

            template <std::size_t... I>
            SIMDIFY_FORCE_INLINE void incr_impl(sequence<I...>) {
                detail::no_op(simd::get<I>(m_buf).ptr += W...);
            }

            iterator& operator++() {
                incr_impl(buffer_t::all_elements{});
                return *this;
            }

            bool operator==(const iterator& rhs) { return simd::get<0>(m_buf).ptr == simd::get<0>(rhs.m_buf).ptr; }
            bool operator!=(const iterator& rhs) { return simd::get<0>(m_buf).ptr != simd::get<0>(rhs.m_buf).ptr; }
            buffer_t& operator*() const { return m_buf; }
            buffer_t& operator->() const { return m_buf; }

        private:
            buffer_t m_buf;
        };

        std::size_t size() { return std::get<0>(m_data).size(); }
        iterator begin() { return iterator(*this, 0); }
        iterator end() { return iterator(*this, div_ceil_shift<W>(size())); }
        //const_iterator begin() const;
        //const_iterator end() const;
        //const_iterator cbegin() const;
        //const_iterator cend() const;
        //iterator bodybegin();
        //iterator bodyend();
        //const_iterator bodybegin() const;
        //const_iterator bodyend() const;
        //const_iterator bodycbegin() const;
        //const_iterator bodycend() const;
        //tail_iterator tailbegin();
        //tail_iterator tailend();
        //const_tail_iterator tailbegin() const;
        //const_tail_iterator tailend() const;
        //const_tail_iterator tailcbegin() const;
        //const_tail_iterator tailcend() const;
    private:
        std::array<std::vector<f_t, aligned_allocator<f_t, alignof(mm_t)>>, N> m_data;
    };
}

#endif // SIMDIFY_STRUCTURE_OF_ARRAYS
