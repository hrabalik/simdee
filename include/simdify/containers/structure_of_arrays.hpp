#ifndef SIMDIFY_STRUCTURE_OF_ARRAYS
#define SIMDIFY_STRUCTURE_OF_ARRAYS

#include "common.hpp"

namespace simd {

    template <typename Simd_t, typename Ids, typename Sequence>
    struct structure_of_arrays_impl;

    template <typename Simd_t, typename... Ids, std::size_t... I>
    struct structure_of_arrays_impl<Simd_t, detail::group<Ids...>, sequence<I...>> {
        using self_t = structure_of_arrays_impl;
        using simd_t = Simd_t;
        using e_t = typename simd_t::e_t;
        using mm_t = typename simd_t::mm_t;
        using vec_u = typename simd_type_traits<Simd_t>::vec_u;

        enum : std::size_t { N = detail::group<Ids...>::size, W = simd_t::W };

        static_assert(N == sizeof...(I), "structure_of_arrays_impl: sequence size mismatch");
        static_assert(std::is_trivial<e_t>::value, "structure_of_arrays_impl: e_t not trivial");

        using value_type = named_array<e_t, Ids...>;
        using value_type_vector = named_array<simd_t, Ids...>;
        using reference = named_array<simd::reference<simd::storage<e_t>>, Ids...>;
        using const_reference = named_array<simd::const_reference<simd::storage<e_t>>, Ids...>;
        using reference_vector = named_array<simd::reference<simd::storage<Simd_t>>, Ids...>;
        using const_reference_vector = named_array<simd::const_reference<simd::storage<Simd_t>>, Ids...>;

        SIMDIFY_CONTAINERS_COMMON_CONSTRUCTION(structure_of_arrays_impl);

        void reserve(std::size_t count) {
            if (count <= m_cap) return;

            std::size_t new_cap = m_cap;
            if (new_cap == 0) {
                new_cap = div_ceil_mult<W>(count);
            }
            else do { new_cap *= 2; } while (new_cap < count);

            decltype(m_data) new_data(aligned_malloc<e_t, alignof(mm_t)>(N * new_cap), aligned_deleter{});

            if (!new_data) throw std::bad_alloc{};

            if (m_sz != 0) {
                detail::no_op(std::memcpy(new_data.get() + I*new_cap, m_data.get() + I*m_cap, sizeof(e_t)*m_sz)...);
            }

            std::swap(m_data, new_data);
            m_cap = new_cap;
        }

        void fill(const value_type& val, std::size_t from = 0) {
            e_t* first = m_data.get() + from;
            e_t* last = m_data.get() + m_sz;
            for (int n = 0; n < N; ++n, first += m_cap, last += m_cap) {
                e_t f = val[n];
                for (e_t* curr = first; curr != last; ++curr) {
                    *curr = f;
                }
            }
        }

        SIMIDFY_CONTAINERS_COMMON_SIZE_MANAGEMENT;

        reference operator[](std::size_t i) {
            reference res;
            auto base = m_data.get() + i;
            detail::no_op(simd::get<I>(res).reset(base + I*m_cap)...);
            return res;
        }

        const_reference operator[](std::size_t i) const {
            const_reference res;
            auto base = m_data.get() + i;
            detail::no_op(simd::get<I>(res).reset(base + I*m_cap)...);
            return res;
        }

        named_array<simd_t, Ids...> operator[](const vec_u& idx) const {
            return operator[](storage<vec_u>(idx));
        }

        named_array<simd_t, Ids...> operator[](const storage<vec_u>& idx) const {
            named_array<simd_t, Ids...> out;

            for (int j = 0; j < N; ++j) {
                e_t* base = m_data.get() + j*m_cap;
                storage<simd_t> stor;
                for (int i = 0; i < W; ++i) {
                    stor[i] = *(base + idx[i]);
                }
                out[j] = stor;
            }

            return out;
        }

        SIMDIFY_CONTAINERS_COMMON_ACCESS("structure_of_arrays");

        void push_back(const value_type& val) {
            reserve(m_sz + 1);
            e_t* base = m_data.get() + m_sz;
            detail::no_op(*(base + I*m_cap) = simd::get<I>(val)...);
            ++m_sz;
        }

        SIMDIFY_CONTAINERS_COMMON_POP_BACK("structure_of_arrays");

        template <typename Val, typename Ref>
        struct common_iterator : std::iterator<std::random_access_iterator_tag, Val, std::ptrdiff_t, common_iterator<Val, Ref>, Ref> {
            common_iterator(const self_t& self, std::size_t idx) {
                auto base = self.m_data.get() + idx;
                detail::no_op(simd::get<I>(m_ref).reset(base + I*self.m_cap)...);
            }

            common_iterator& operator=(const common_iterator& rhs) {
                detail::no_op(simd::get<I>(m_ref).reset(simd::get<I>(rhs.m_ref).ptr())...);
                return *this;
            }

            common_iterator& operator++() { detail::no_op(++simd::get<I>(m_ref).ptr()...); return *this; }
            common_iterator& operator--() { detail::no_op(--simd::get<I>(m_ref).ptr()...); return *this; }
            common_iterator& operator+=(std::ptrdiff_t add) { detail::no_op(simd::get<I>(m_ref).ptr() += add...); return *this; }
            std::ptrdiff_t operator-(const common_iterator& rhs) { return m_ref.get().ptr() - rhs.m_ref.get().ptr(); }
            bool operator<(const common_iterator& rhs) const { return m_ref.get().ptr() < rhs.m_ref.get().ptr(); }
            bool operator<=(const common_iterator& rhs) const { return m_ref.get().ptr() <= rhs.m_ref.get().ptr(); }
            bool operator==(const common_iterator& rhs) const { return m_ref.get().ptr() == rhs.m_ref.get().ptr(); }
            Ref& operator*() { return m_ref; }
            Ref* operator->() { return &m_ref; }

            SIMDIFY_CONTAINERS_COMMON_ITERATOR_FACILITIES(common_iterator);

        private:
            Ref m_ref;
        };

        using iterator = common_iterator<value_type, reference>;
        using const_iterator = common_iterator<const value_type, const_reference>;
        using iterator_vector = common_iterator<value_type_vector, reference_vector>;
        using const_iterator_vector = common_iterator<const value_type_vector, const_reference_vector>;

        SIMDIFY_CONTAINERS_COMMON_ITERATION_SCALAR;
        SIMDIFY_CONTAINERS_COMMON_ITERATION_VECTOR;

    private:
        SIMDIFY_CONTAINERS_COMMON_DATA;
    };

    template <typename Simd_t, typename... Ids>
    using structure_of_arrays = structure_of_arrays_impl<Simd_t, detail::group<Ids...>, make_sequence_t<0, detail::group<Ids...>::size>>;
}

#endif // SIMDIFY_STRUCTURE_OF_ARRAYS
