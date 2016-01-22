#ifndef SIMDIFY_ARRAY_OF_STRUCTURES
#define SIMDIFY_ARRAY_OF_STRUCTURES

#include "containers_common.hpp"

namespace simd {

    template <typename Simd_t, typename Ids, typename Sequence>
    struct array_of_structures_impl;

    template <typename Simd_t, typename... Ids, std::size_t... I>
    struct array_of_structures_impl<Simd_t, detail::group<Ids...>, sequence<I...>> {
        using self_t = array_of_structures_impl;
        using simd_t = Simd_t;
        using e_t = typename simd_t::e_t;
        using mm_t = typename simd_t::mm_t;
        using vec_u = typename simd_type_traits<Simd_t>::vec_u;

        enum : std::size_t { N = detail::group<Ids...>::size, W = simd_t::W };

        static_assert(N == sizeof...(I), "array_of_structures_impl: sequence size mismatch");
        static_assert(std::is_trivial<e_t>::value, "array_of_structures_impl: e_t not trivial");

        using value_type = named_array<e_t, Ids...>;
        using value_type_vector = named_array<simd_t, Ids...>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using reference_vector = named_array<simd::reference<simd::aos_storage<Simd_t, N>>, Ids...>;
        using const_reference_vector = named_array<simd::const_reference<simd::aos_storage<Simd_t, N>>, Ids...>;

        SIMDIFY_CONTAINERS_COMMON_CONSTRUCTION(array_of_structures_impl);

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
                std::memcpy(new_data.get(), m_data.get(), N*sizeof(e_t)*m_sz);
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

        named_array<simd_t, Ids...> operator[](const vec_u& idx) const {
            return operator[](storage<vec_u>(idx));
        }

        named_array<simd_t, Ids...> operator[](const storage<vec_u>& idx) const {
            value_type* ptr[W];

            for (int i = 0; i < W; ++i) {
                ptr[i] = &data_as_value_type_ptr()[idx[i]];
            }

            named_array<simd_t, Ids...> out;

            for (int j = 0; j < N; ++j) {
                storage<simd_t> stor;
                for (int i = 0; i < W; ++i) {
                    stor[i] = (*(ptr[i]))[j];
                }
                out[j] = stor;
            }

            return out;
        }

        SIMDIFY_CONTAINERS_COMMON_ACCESS("array_of_structures");

        void push_back(const value_type& val) {
            reserve(m_sz + 1);
            *(data_as_value_type_ptr() + m_sz) = val;
            ++m_sz;
        }

        SIMDIFY_CONTAINERS_COMMON_POP_BACK("array_of_structures");

        template <typename Val, typename Ref>
        struct vector_iterator : std::iterator<std::random_access_iterator_tag, Val, std::ptrdiff_t, vector_iterator<Val, Ref>, Ref> {
            vector_iterator(const self_t& self, std::size_t idx) {
                auto base = self.m_data.get() + N * idx;
                detail::no_op(simd::get<I>(m_ref).reset(base + I)...);
            }

            vector_iterator& operator=(const vector_iterator& rhs) {
                detail::no_op(simd::get<I>(m_ref).reset(simd::get<I>(rhs.m_ref).ptr())...);
                return *this;
            }

            vector_iterator& operator++() { detail::no_op(++simd::get<I>(m_ref).ptr()...); return *this; }
            vector_iterator& operator--() { detail::no_op(--simd::get<I>(m_ref).ptr()...); return *this; }
            vector_iterator& operator+=(std::ptrdiff_t add) { detail::no_op(simd::get<I>(m_ref).ptr() += add...); return *this; }
            std::ptrdiff_t operator-(const vector_iterator& rhs) { return m_ref.get().ptr() - rhs.m_ref.get().ptr(); }
            bool operator<(const vector_iterator& rhs) const { return m_ref.get().ptr() < rhs.m_ref.get().ptr(); }
            bool operator<=(const vector_iterator& rhs) const { return m_ref.get().ptr() <= rhs.m_ref.get().ptr(); }
            bool operator==(const vector_iterator& rhs) const { return m_ref.get().ptr() == rhs.m_ref.get().ptr(); }
            Ref& operator*() { return m_ref; }
            Ref* operator->() { return &m_ref; }

            SIMDIFY_CONTAINERS_COMMON_ITERATOR_FACILITIES(vector_iterator);

        private:
            Ref m_ref;
        };

        using iterator = value_type*;
        using const_iterator = const value_type*;
        using iterator_vector = vector_iterator<value_type_vector, reference_vector>;
        using const_iterator_vector = vector_iterator<const value_type_vector, const_reference_vector>;

        SIMDIFY_INL iterator begin() { return data_as_value_type_ptr() + 0; }
        SIMDIFY_INL iterator end() { return data_as_value_type_ptr() + size(); }
        SIMDIFY_INL iterator begin_tail() { return data_as_value_type_ptr() + size_body(); }
        SIMDIFY_INL iterator end_tail() { return data_as_value_type_ptr() + size(); }

        SIMDIFY_INL const_iterator begin() const { return cbegin(); }
        SIMDIFY_INL const_iterator end() const { return cend(); }
        SIMDIFY_INL const_iterator begin_tail() const { return cbegin_tail(); }
        SIMDIFY_INL const_iterator end_tail() const { return cend_tail(); }

        SIMDIFY_INL const_iterator cbegin() const { return data_as_value_type_ptr() + 0; }
        SIMDIFY_INL const_iterator cend() const { return data_as_value_type_ptr() + size(); }
        SIMDIFY_INL const_iterator cbegin_tail() const { return data_as_value_type_ptr() + size_body(); }
        SIMDIFY_INL const_iterator cend_tail() const { return data_as_value_type_ptr() + size(); }

        SIMDIFY_CONTAINERS_COMMON_ITERATION_VECTOR;

    private:
        value_type* data_as_value_type_ptr() const {
            return static_cast<value_type*>(static_cast<void*>(m_data.get()));
        }

        SIMDIFY_CONTAINERS_COMMON_DATA;
    };

    template <typename Simd_t, typename... Ids>
    using array_of_structures = array_of_structures_impl<Simd_t, detail::group<Ids...>, make_sequence_t<0, detail::group<Ids...>::size>>;

}

#endif // SIMDIFY_ARRAY_OF_STRUCTURES
