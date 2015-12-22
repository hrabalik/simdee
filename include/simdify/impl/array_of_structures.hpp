#ifndef SIMDIFY_ARRAY_OF_STRUCTURES
#define SIMDIFY_ARRAY_OF_STRUCTURES

#include "containers_common.hpp"

namespace simd {

    template <typename Simd_t, typename Names, typename Sequence>
    struct array_of_structures_impl;

    template <typename Simd_t, typename... Names, std::size_t... I>
    struct array_of_structures_impl<Simd_t, detail::group<Names...>, sequence<I...>> {
        using self_t = array_of_structures_impl;
        using simd_t = Simd_t;
        using f_t = typename simd_t::f_t;
        using mm_t = typename simd_t::mm_t;

        enum : std::size_t { N = detail::group<Names...>::size, W = simd_t::W };

        static_assert(N == sizeof...(I), "array_of_structures_impl: sequence size mismatch");
        static_assert(std::is_trivial<f_t>::value, "array_of_structures_impl: f_t not trivial");

        using value_type = named_array<f_t, Names...>;
        using value_type_vector = named_array<simd_t, Names...>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using reference_vector = named_array<simd::reference<simd::aos_storage<Simd_t, N>>, Names...>;
        using const_reference_vector = named_array<simd::const_reference<simd::aos_storage<Simd_t, N>>, Names...>;

        SIMDIFY_CONTAINERS_COMMON_CONSTRUCTION(array_of_structures_impl);

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
                std::memcpy(new_data.get(), m_data.get(), N*sizeof(f_t)*m_sz);
            }

            std::swap(m_data, new_data);
            m_cap = new_cap;
        }

        void fill(const value_type& val, std::size_t from = 0) {
            auto first = data_as_value_type_ptr() + from;
            auto last = data_as_value_type_ptr() + m_sz;
            for (; first != last; ++first) {
                *first = val;
            }
        }

        SIMIDFY_CONTAINERS_COMMON_SIZE_MANAGEMENT;

        reference operator[](std::size_t i) {
            return data_as_value_type_ptr()[i];
        }

        const_reference operator[](std::size_t i) const {
            return data_as_value_type_ptr()[i];
        }

        SIMDIFY_CONTAINERS_COMMON_ACCESS("array_of_structures");

        void push_back(const value_type& val) {
            reserve(m_sz + 1);
            *(data_as_value_type_ptr() + m_sz) = val;
            ++m_sz;
        }

        SIMDIFY_CONTAINERS_COMMON_POP_BACK("array_of_structures");

        template <typename Val>
        struct value_iterator : std::iterator<std::random_access_iterator_tag, Val> {
            value_iterator(const self_t& self, std::size_t idx) :
                m_ptr(self.data_as_value_type_ptr() + idx) {}

            value_iterator& operator=(const value_iterator& rhs) = default;

            value_iterator& operator++() { ++m_ptr; return *this; }
            value_iterator& operator--() { --m_ptr; return *this; }
            value_iterator& operator+=(std::ptrdiff_t add) { m_ptr += add; return *this; }
            std::ptrdiff_t operator-(const value_iterator& rhs) { return m_ptr - rhs.m_ptr; }
            bool operator<(const value_iterator& rhs) const { return m_ptr < rhs.m_ptr; }
            bool operator<=(const value_iterator& rhs) const { return m_ptr <= rhs.m_ptr; }
            bool operator==(const value_iterator& rhs) const { return m_ptr == rhs.m_ptr; }
            Val& operator*() { return *m_ptr; }
            Val* operator->() { return m_ptr; }

            SIMDIFY_CONTAINERS_COMMON_ITERATOR_FACILITIES(value_iterator);

        private:
            Val* m_ptr;
        };

        template <typename Ref>
        struct reference_iterator : std::iterator<std::random_access_iterator_tag, Ref> {
            reference_iterator(const self_t& self, std::size_t idx) {
                auto base = self.m_data.get() + N * idx;
                detail::no_op(simd::get<I>(m_ref).reset(base + I)...);
            }

            reference_iterator& operator=(const reference_iterator& rhs) {
                detail::no_op(simd::get<I>(m_ref).reset(simd::get<I>(rhs.m_ref).ptr())...);
                return *this;
            }

            reference_iterator& operator++() { detail::no_op(++simd::get<I>(m_ref).ptr()...); return *this; }
            reference_iterator& operator--() { detail::no_op(--simd::get<I>(m_ref).ptr()...); return *this; }
            reference_iterator& operator+=(std::ptrdiff_t add) { detail::no_op(simd::get<I>(m_ref).ptr() += add...); return *this; }
            std::ptrdiff_t operator-(const reference_iterator& rhs) { return m_ref.get().ptr() - rhs.m_ref.get().ptr(); }
            bool operator<(const reference_iterator& rhs) const { return m_ref.get().ptr() < rhs.m_ref.get().ptr(); }
            bool operator<=(const reference_iterator& rhs) const { return m_ref.get().ptr() <= rhs.m_ref.get().ptr(); }
            bool operator==(const reference_iterator& rhs) const { return m_ref.get().ptr() == rhs.m_ref.get().ptr(); }
            Ref& operator*() { return m_ref; }
            Ref* operator->() { return &m_ref; }

            SIMDIFY_CONTAINERS_COMMON_ITERATOR_FACILITIES(reference_iterator);

        private:
            Ref m_ref;
        };

        using iterator = value_iterator<value_type>;
        using const_iterator = value_iterator<const value_type>;
        using iterator_vector = reference_iterator<reference_vector>;
        using const_iterator_vector = reference_iterator<const_reference_vector>;

        SIMDIFY_CONTAINERS_COMMON_ITERATION;

    private:
        value_type* data_as_value_type_ptr() const {
            return static_cast<value_type*>(static_cast<void*>(m_data.get()));
        }

        SIMDIFY_CONTAINERS_COMMON_DATA;
    };

    template <typename Simd_t, typename... Names>
    using array_of_structures = array_of_structures_impl<Simd_t, detail::group<Names...>, make_sequence_t<0, detail::group<Names...>::size>>;

}

#endif // SIMDIFY_ARRAY_OF_STRUCTURES
