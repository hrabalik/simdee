#ifndef SIMDIFY_ARRAY_OF_VECTORS
#define SIMDIFY_ARRAY_OF_VECTORS

#include "common.hpp"

namespace simd {

    template <typename Simd_t, typename Ids, typename Sequence>
    struct array_of_vectors_impl;

    template <typename Simd_t, typename... Ids, std::size_t... I>
    struct array_of_vectors_impl<Simd_t, detail::group<Ids...>, sequence<I...>> {
        using self_t = array_of_vectors_impl;
        using simd_t = Simd_t;
        using e_t = typename simd_t::e_t;
        using mm_t = typename simd_t::mm_t;
        using vec_u = typename simd_type_traits<Simd_t>::vec_u;

        enum : std::size_t { N = detail::group<Ids...>::size, W = simd_t::W };

        static_assert(N == sizeof...(I), "array_of_vectors_impl: sequence size mismatch");
        static_assert(std::is_trivial<e_t>::value, "array_of_vectors_impl: e_t not trivial");

        using value_type = named_array<e_t, Ids...>;
        using value_type_vector = named_array<simd::storage<simd_t>, Ids...>;
        using reference = named_array<simd::reference<simd::storage<e_t>>, Ids...>;
        using const_reference = named_array<simd::const_reference<simd::storage<e_t>>, Ids...>;
        using reference_vector = value_type_vector&;
        using const_reference_vector = const value_type_vector&;

        SIMDIFY_CONTAINERS_COMMON_CONSTRUCTION(array_of_vectors_impl);

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
            auto first = begin();
            auto last = end();
            first += from;

            for (; first < last; ++first) {
                detail::no_op(simd::get<I>(*first) = simd::get<I>(val)...);
            }
        }

        SIMIDFY_CONTAINERS_COMMON_SIZE_MANAGEMENT;

        reference operator[](std::size_t i) {
            reference res;
            auto base = m_data.get() + (N*W)*div_floor<W>(i) + mod<W>(i);
            detail::no_op(simd::get<I>(res).reset(base + I*W)...);
            return res;
        }

        const_reference operator[](std::size_t i) const {
            const_reference res;
            auto base = m_data.get() + (N*W)*div_floor<W>(i) + mod<W>(i);
            detail::no_op(simd::get<I>(res).reset(base + I*W)...);
            return res;
        }

        named_array<simd_t, Ids...> operator[](const vec_u& idx) const {
            return operator[](storage<vec_u>(idx));
        }

        named_array<simd_t, Ids...> operator[](const storage<vec_u>& idx) const {
            e_t* ptr[W];

            for (int i = 0; i < W; ++i) {
                ptr[i] = m_data.get() + (N*W)*div_floor<W>(idx[i]) + mod<W>(idx[i]);
            }

            named_array<simd_t, Ids...> out;

            for (int j = 0; j < N; ++j) {
                storage<simd_t> stor;
                for (int i = 0; i < W; ++i) {
                    stor[i] = *(ptr[i] + j*W);
                }
                out[j] = stor;
            }

            return out;
        }

        SIMDIFY_CONTAINERS_COMMON_ACCESS("structure_of_arrays");

        void push_back(const value_type& val) {
            reserve(m_sz + 1);
            auto base = m_data.get() + (N*W)*div_floor<W>(m_sz) + mod<W>(m_sz);
            detail::no_op(*(base + I*W) = simd::get<I>(val)...);
            ++m_sz;
        }

        SIMDIFY_CONTAINERS_COMMON_POP_BACK("structure_of_arrays");

        template <typename Val, typename Ref>
        struct scalar_iterator : std::iterator<std::random_access_iterator_tag, Val, std::ptrdiff_t, scalar_iterator<Val, Ref>, Ref> {
            scalar_iterator(const self_t& self, std::size_t idx) {
                auto base = self.m_data.get() + (N*W)*div_floor<W>(idx) + mod<W>(idx);
                detail::no_op(simd::get<I>(m_ref).reset(base + I*W)...);
                pos = idx;
            }

            scalar_iterator& operator=(const scalar_iterator& rhs) {
                detail::no_op(simd::get<I>(m_ref).reset(simd::get<I>(rhs.m_ref).ptr())...);
                pos = rhs.pos;
                return *this;
            }

            scalar_iterator& operator++() {
                auto incr = (mod<W>(pos) == W - 1) ? ((N - 1)*W + 1) : 1;
                detail::no_op(simd::get<I>(m_ref).ptr() += incr...);
                ++pos;
                return *this;
            }

            scalar_iterator& operator--() {
                auto decr = (mod<W>(pos) == 0) ? ((N - 1)*W + 1) : 1;
                detail::no_op(simd::get<I>(m_ref).ptr() -= decr...);
                --pos;
                return *this;
            }

            scalar_iterator& operator+=(std::ptrdiff_t add) {
                enum : std::ptrdiff_t { sigW = W, sigN = N };
                auto tail = static_cast<std::ptrdiff_t>(mod<W>(pos));
                tail = (add >= 0) ? tail : (tail - sigW + 1);
                auto ta = (tail + add) / sigW;
                auto nw = ((sigN - 1)*sigW);
                auto incr = add + ta*nw;
                detail::no_op(simd::get<I>(m_ref).ptr() += incr...);
                pos += add;
                return *this;
            }

            std::ptrdiff_t operator-(const scalar_iterator& rhs) {
                return static_cast<std::ptrdiff_t>(pos - rhs.pos);
            }

            bool operator<(const scalar_iterator& rhs) const { return pos < rhs.pos; }
            bool operator<=(const scalar_iterator& rhs) const { return pos <= rhs.pos; }
            bool operator==(const scalar_iterator& rhs) const { return pos == rhs.pos; }
            Ref& operator*() { return m_ref; }
            Ref* operator->() { return &m_ref; }

            SIMDIFY_CONTAINERS_COMMON_ITERATOR_FACILITIES(scalar_iterator);

        private:
            Ref m_ref;
            std::size_t pos;
        };

        using iterator = scalar_iterator<value_type, reference>;
        using const_iterator = scalar_iterator<const value_type, const_reference>;
        using iterator_vector = value_type_vector*;
        using const_iterator_vector = const value_type_vector*;

        SIMDIFY_INL iterator_vector begin_overspan() { return data_as_value_vector_type_ptr(); }
        SIMDIFY_INL iterator_vector end_overspan() { return data_as_value_vector_type_ptr() + (size_overspan() / W); }
        SIMDIFY_INL iterator_vector begin_body() { return data_as_value_vector_type_ptr(); }
        SIMDIFY_INL iterator_vector end_body() { return data_as_value_vector_type_ptr() + (size_body() / W); }

        SIMDIFY_INL const_iterator_vector begin_overspan() const { return cbegin_overspan(); }
        SIMDIFY_INL const_iterator_vector end_overspan() const { return cend_overspan(); }
        SIMDIFY_INL const_iterator_vector begin_body() const { return cbegin_body(); }
        SIMDIFY_INL const_iterator_vector end_body() const { return cend_body(); }

        SIMDIFY_INL const_iterator_vector cbegin_overspan() const { return data_as_value_vector_type_ptr(); }
        SIMDIFY_INL const_iterator_vector cend_overspan() const { return data_as_value_vector_type_ptr() + (size_overspan() / W); }
        SIMDIFY_INL const_iterator_vector cbegin_body() const { return data_as_value_vector_type_ptr(); }
        SIMDIFY_INL const_iterator_vector cend_body() const { return data_as_value_vector_type_ptr() + (size_body() / W); }

        SIMDIFY_CONTAINERS_COMMON_ITERATION_SCALAR;

    private:
        value_type_vector* data_as_value_vector_type_ptr() const {
            return static_cast<value_type_vector*>(static_cast<void*>(m_data.get()));
        }

        SIMDIFY_CONTAINERS_COMMON_DATA;
    };

    template <typename Simd_t, typename... Ids>
    using array_of_vectors = array_of_vectors_impl<Simd_t, detail::group<Ids...>, make_sequence_t<0, detail::group<Ids...>::size>>;

}

#endif // SIMDIFY_ARRAY_OF_VECTORS
