#ifndef SIMDIFY_CONTAINERS_COMMON
#define SIMDIFY_CONTAINERS_COMMON

#include "../util/malloc.hpp"
#include "../util/integral.hpp"
#include "expr.hpp"
#include "named_array.hpp"
#include "storage.hpp"
#include <memory>
#include <exception>
#include <cstring>
#include <iterator>

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_CONTAINERS_COMMON_CONSTRUCTION( CLASS )                                                  \
                                                                                                         \
    CLASS () : m_data(nullptr, aligned_deleter{}), m_sz(0), m_cap(0) {}                                  \
                                                                                                         \
    CLASS (std::size_t count) : CLASS () {                                                               \
        resize(count);                                                                                   \
    }                                                                                                    \
                                                                                                         \
    CLASS (std::size_t count, const value_type& val)                                                     \
        : CLASS (count) {                                                                                \
        fill(val);                                                                                       \
    }                                                                                                    \
                                                                                                         \
    CLASS ( CLASS && rhs) :                                                                              \
        m_data(std::move(rhs.m_data)), m_sz(rhs.m_sz), m_cap(rhs.m_cap) {                                \
        rhs.m_sz = 0; rhs.m_cap = 0;                                                                     \
    }                                                                                                    \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMIDFY_CONTAINERS_COMMON_SIZE_MANAGEMENT                                                        \
                                                                                                         \
    std::size_t capacity() const { return m_cap; }                                                       \
    std::size_t size() const { return m_sz; }                                                            \
    std::size_t size_body() const { return div_floor_shift<W>(m_sz); }                                   \
    std::size_t size_overspan() const { return div_ceil_shift<W>(m_sz); }                                \
    std::size_t size_tail() const { return size() - size_body(); }                                       \
                                                                                                         \
    void resize(std::size_t count) {                                                                     \
        reserve(count);                                                                                  \
        m_sz = count;                                                                                    \
    }                                                                                                    \
                                                                                                         \
    void resize(std::size_t count, const value_type& val) {                                              \
        auto sz = m_sz;                                                                                  \
        resize(count);                                                                                   \
        if (count > sz) fill(val, sz);                                                                   \
    }                                                                                                    \
                                                                                                         \
    void clear() { m_sz = 0; }                                                                           \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_CONTAINERS_COMMON_ITERATION                                                              \
                                                                                                         \
    iterator begin() { return iterator(*this, 0); }                                                      \
    iterator end() { return iterator(*this, size()); }                                                   \
    iterator_vector begin_overspan() { return iterator_vector(*this, 0); }                               \
    iterator_vector end_vector() { return iterator_vector(*this, size_overspan()); }                     \
    iterator_vector begin_body() { return iterator_vector(*this, 0); }                                   \
    iterator_vector end_body() { return iterator_vector(*this, size_body()); }                           \
    iterator begin_tail() { return iterator(*this, size_body()); }                                       \
    iterator end_tail() { return iterator(*this, size()); }                                              \
                                                                                                         \
    const_iterator begin() const { return cbegin(); }                                                    \
    const_iterator end() const { return cend(); }                                                        \
    const_iterator_vector begin_overspan() const { return cbegin_overspan(); }                           \
    const_iterator_vector end_vector() const { return cend_vector(); }                                   \
    const_iterator_vector begin_body() const { return cbegin_body(); }                                   \
    const_iterator_vector end_body() const { return cend_body(); }                                       \
    const_iterator begin_tail() const { return cbegin_tail(); }                                          \
    const_iterator end_tail() const { return cend_tail(); }                                              \
                                                                                                         \
    const_iterator cbegin() const { return const_iterator(*this, 0); }                                   \
    const_iterator cend() const { return const_iterator(*this, size()); }                                \
    const_iterator_vector cbegin_overspan() const { return const_iterator_vector(*this, 0); }            \
    const_iterator_vector cend_vector() const { return const_iterator_vector(*this, size_overspan()); }  \
    const_iterator_vector cbegin_body() const { return const_iterator_vector(*this, 0); }                \
    const_iterator_vector cend_body() const { return const_iterator_vector(*this, size_body()); }        \
    const_iterator cbegin_tail() const { return const_iterator(*this, size_body()); }                    \
    const_iterator cend_tail() const { return const_iterator(*this, size()); }                           \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_CONTAINERS_COMMON_ACCESS( CLASS_STRING )                                                 \
                                                                                                         \
    reference at(std::size_t i) {                                                                        \
        if (i >= m_sz) throw std::runtime_error( CLASS_STRING ": out of bounds in at()");                \
        return operator[](i);                                                                            \
    }                                                                                                    \
                                                                                                         \
    const_reference at(std::size_t i) const {                                                            \
        if (i >= m_sz) throw std::runtime_error( CLASS_STRING ": out of bounds in at()");                \
        return operator[](i);                                                                            \
    }                                                                                                    \
                                                                                                         \
    reference back() {                                                                                   \
        if (m_sz == 0) throw std::runtime_error( CLASS_STRING ": container empty in back()");            \
        return operator[](m_sz - 1);                                                                     \
    }                                                                                                    \
                                                                                                         \
    const_reference back() const {                                                                       \
        if (m_sz == 0) throw std::runtime_error( CLASS_STRING ": container empty in back()");            \
        return operator[](m_sz - 1);                                                                     \
    }                                                                                                    \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_CONTAINERS_COMMON_POP_BACK( CLASS_STRING )                                               \
                                                                                                         \
    void pop_back() {                                                                                    \
        if (m_sz == 0) throw std::runtime_error( CLASS_STRING ": container empty in pop_back()");        \
        --m_sz;                                                                                          \
    }                                                                                                    \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_CONTAINERS_COMMON_DATA                                                                   \
                                                                                                         \
    std::unique_ptr<e_t, aligned_deleter> m_data;                                                        \
    std::size_t m_sz;                                                                                    \
    std::size_t m_cap;                                                                                   \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_CONTAINERS_COMMON_ITERATOR_FACILITIES( CLASS )                                           \
                                                                                                         \
    CLASS () = default;                                                                                  \
                                                                                                         \
    CLASS (const CLASS & rhs) {                                                                          \
        operator=(rhs);                                                                                  \
    }                                                                                                    \
                                                                                                         \
    CLASS & operator-=(std::ptrdiff_t sub) {                                                             \
        return operator+=(-sub);                                                                         \
    }                                                                                                    \
                                                                                                         \
    CLASS operator+(std::ptrdiff_t add) const {                                                          \
        CLASS res(*this);                                                                                \
        res += add;                                                                                      \
        return res;                                                                                      \
    }                                                                                                    \
                                                                                                         \
    CLASS operator-(std::ptrdiff_t sub) const {                                                          \
        CLASS res(*this);                                                                                \
        res -= sub;                                                                                      \
        return res;                                                                                      \
    }                                                                                                    \
                                                                                                         \
    friend CLASS operator+(std::ptrdiff_t add, const CLASS & rhs) {                                      \
        return rhs + add;                                                                                \
    }                                                                                                    \
                                                                                                         \
    CLASS operator++(int) {                                                                              \
        CLASS res(*this);                                                                                \
        operator++();                                                                                    \
        return res;                                                                                      \
    }                                                                                                    \
                                                                                                         \
    CLASS operator--(int) {                                                                              \
        CLASS res(*this);                                                                                \
        operator--();                                                                                    \
        return res;                                                                                      \
    }                                                                                                    \
                                                                                                         \
    bool operator>=(const CLASS & rhs) const { return !operator<(rhs); }                                 \
    bool operator>(const CLASS & rhs) const { return !operator<=(rhs); }                                 \
    bool operator!=(const CLASS & rhs) const { return !operator==(rhs); }                                \
                                                                                                         \
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // SIMDIFY_CONTAINERS_COMMON
