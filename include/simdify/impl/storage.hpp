#ifndef SIMDIFY_STORAGE_IMPL
#define SIMDIFY_STORAGE_IMPL

#include "common.hpp"
#include "expr.hpp"
#include "../util/inline.hpp"

namespace simd {

    //
    // storage for SIMD or floating-point types
    //
    template <typename T, typename Enable = void>
    struct storage;

    //
    // storage replacement for loading SIMD vectors from interleaved (AOS) data
    //
    template <typename Simd_t, std::size_t N>
    struct aos_storage;

    //
    // reference to storage (proxy object)
    //
    template <typename Storage>
    struct reference;

    //
    // const reference to storage (proxy object)
    //
    template <typename Storage>
    struct const_reference;

    //
    // specialized storage for SIMD types
    //
    template <typename Simd_t>
    struct alignas(Simd_t)storage<Simd_t, typename std::enable_if<is_simd_type<Simd_t>::value>::type> {
        using stored_t = Simd_t;
        using e_t = typename Simd_t::e_t;
        using data_t = std::array<e_t, Simd_t::W>;

        SIMDIFY_INL constexpr storage() = default;
        SIMDIFY_INL constexpr storage(const storage&) = default;
        SIMDIFY_INL constexpr storage(reference<storage> rhs) : storage(*rhs.ptr()) {}
        SIMDIFY_INL constexpr storage(const_reference<storage> rhs) : storage(*rhs.ptr()) {}

        SIMDIFY_INL explicit storage(const Simd_t& rhs) { rhs.aligned_store(data()); }

        SIMDIFY_INL constexpr explicit storage(const data_t& rhs) : m_data(rhs) {}

        template <std::size_t N>
        SIMDIFY_INL storage(const aos_storage<Simd_t, N>& rhs) { operator=(rhs); }
        template <std::size_t N>
        SIMDIFY_INL storage(reference<aos_storage<Simd_t, N>> rhs) { operator=(*rhs.ptr()); }
        template <std::size_t N>
        SIMDIFY_INL storage(const_reference<aos_storage<Simd_t, N>> rhs) { operator=(*rhs.ptr()); }

        SIMDIFY_INL storage& operator=(const storage&) = default;
        SIMDIFY_INL storage& operator=(reference<storage> rhs) { return operator=(*rhs.ptr()); }
        SIMDIFY_INL storage& operator=(const_reference<storage> rhs) { return operator=(*rhs.ptr()); }

        SIMDIFY_INL storage& operator=(const Simd_t& rhs) {
            rhs.aligned_store(data());
            return *this;
        }

        SIMDIFY_INL storage& operator=(const data_t& rhs) {
            m_data = rhs;
            return *this;
        }

        template <std::size_t N>
        SIMDIFY_INL storage& operator=(const aos_storage<Simd_t, N>& rhs) {
            auto lp = data();
            auto rp = rhs.data();
            for (std::size_t i = 0; i < Simd_t::W; ++i, ++lp, rp += N) {
                *lp = *rp;
            }
            return *this;
        }

        template <std::size_t N>
        SIMDIFY_INL storage& operator=(reference<aos_storage<Simd_t, N>> rhs) {
            return operator=(*rhs.ptr());
        }

        template <std::size_t N>
        SIMDIFY_INL storage& operator=(const_reference<aos_storage<Simd_t, N>> rhs) {
            return operator=(*rhs.ptr());
        }

        SIMDIFY_INL e_t* data() { return m_data.data(); }
        SIMDIFY_INL const e_t* data() const { return m_data.data(); }
        SIMDIFY_INL e_t& operator[](std::size_t i) { return m_data[i]; }
        SIMDIFY_INL const e_t& operator[](std::size_t i) const { return m_data[i]; }

        // implicit conversion to Simd_t
        SIMDIFY_INL operator Simd_t() const { Simd_t s; s.aligned_load(data()); return s; }

        // data
        data_t m_data;
    };

    //
    // specialized storage for arithmetic types
    //
    template <typename E_t>
    struct storage<E_t, typename std::enable_if<std::is_arithmetic<E_t>::value>::type> {
        using stored_t = E_t;

        SIMDIFY_INL constexpr storage() = default;
        SIMDIFY_INL constexpr storage(const storage&) = default;
        SIMDIFY_INL explicit storage(const E_t& rhs) : m_data(rhs) {}

        SIMDIFY_INL storage& operator=(const storage&) = default;

        SIMDIFY_INL storage& operator=(const E_t& rhs) {
            m_data = rhs;
            return *this;
        }

        SIMDIFY_INL E_t* data() { return &m_data; }
        SIMDIFY_INL const E_t* data() const { return &m_data; }

        // implicit conversion to E_t
        SIMDIFY_INL operator E_t() const { return m_data; }

        // data
        E_t m_data;
    };

    //
    // storage replacement for loading SIMD vectors from interleaved (AOS) data
    //
    template <typename Simd_t, std::size_t N>
    struct aos_storage {
        using stored_t = Simd_t;
        enum : std::size_t { W = Simd_t::W };
        using e_t = typename Simd_t::e_t;
        using data_t = std::array<e_t, N * W>;

        SIMDIFY_INL constexpr aos_storage() = default;
        SIMDIFY_INL aos_storage(const aos_storage& rhs) { operator=(rhs); }
        SIMDIFY_INL explicit aos_storage(const Simd_t& rhs) { operator=(rhs); }
        SIMDIFY_INL explicit aos_storage(const storage<Simd_t>& rhs) { operator=(rhs); }

        SIMDIFY_INL aos_storage& operator=(const aos_storage& rhs) {
            auto lp = data();
            auto rp = rhs.data();
            for (std::size_t i = 0; i < W; ++i, lp += N, rp += N) {
                *lp = *rp;
            }
            return *this;
        }

        SIMDIFY_INL aos_storage& operator=(const Simd_t& rhs) {
            rhs.interleaved_store(data(), N);
            return *this;
        }

        SIMDIFY_INL aos_storage& operator=(const storage<Simd_t>& rhs) {
            auto lp = data();
            auto rp = rhs.data();
            for (std::size_t i = 0; i < W; ++i, lp += N, ++rp) {
                *lp = *rp;
            }
            return *this;
        }

        SIMDIFY_INL e_t* data() { return m_data.data(); }
        SIMDIFY_INL const e_t* data() const { return m_data.data(); }
        SIMDIFY_INL e_t& operator[](std::size_t i) { return m_data[i * N]; }
        SIMDIFY_INL const e_t& operator[](std::size_t i) const { return m_data[i * N]; }

        // implicit conversion to Simd_t
        SIMDIFY_INL operator Simd_t() const {
            Simd_t s;
            s.interleaved_load(data(), N);
            return s;
        }

        // data
        data_t m_data;
    };

    //
    // reference to storage (proxy object)
    //
    template <typename Storage>
    struct reference {
        using referred_t = typename Storage::stored_t;

        SIMDIFY_INL constexpr reference() = default;
        SIMDIFY_INL constexpr reference(const reference&) = default;

        SIMDIFY_INL reference(Storage& rhs) {
            m_data = &rhs;
        }

        template <typename U = int>
        SIMDIFY_INL reference(referred_t& rhs, typename std::enable_if<std::is_arithmetic<referred_t>::value, U>::type = 0) {
            m_data = reinterpret_cast<Storage*>(&rhs);
        }

        SIMDIFY_INL reference& operator=(const reference& rhs) {
            *m_data = *rhs.m_data;
            return *this;
        }

        SIMDIFY_INL reference& operator=(const Storage& rhs) {
            *m_data = rhs;
            return *this;
        }

        SIMDIFY_INL reference& operator=(const referred_t& rhs) {
            *m_data = rhs;
            return *this;
        }

        SIMDIFY_INL Storage* reset(void* ptr) { m_data = static_cast<Storage*>(ptr); return m_data; }
        SIMDIFY_INL Storage*& ptr() { return m_data; }
        SIMDIFY_INL Storage* ptr() const { return m_data; }

        SIMDIFY_INL void swap(reference& rhs) {
            using std::swap;
            swap(*m_data, *rhs.m_data);
        }

        // implicit conversion to const referred_t
        SIMDIFY_INL operator const referred_t() const { return referred_t(*m_data); }

        // data
        Storage* m_data;
    };

    template <typename Storage>
    SIMDIFY_INL void swap(reference<Storage>& lhs, reference<Storage>& rhs) {
        lhs.swap(rhs);
    }

    //
    // const reference to storage (proxy object)
    //
    template <typename Storage>
    struct const_reference {
        using referred_t = typename Storage::stored_t;

        SIMDIFY_INL constexpr const_reference() = default;
        SIMDIFY_INL constexpr const_reference(const const_reference&) = default;

        SIMDIFY_INL const_reference(const reference<Storage>& rhs) {
            m_data = rhs.ptr();
        }

        SIMDIFY_INL const_reference(const Storage& rhs) {
            m_data = &rhs;
        }

        template <typename U = int>
        SIMDIFY_INL const_reference(const referred_t& rhs, typename std::enable_if<std::is_arithmetic<referred_t>::value, U>::type = 0) {
            m_data = reinterpret_cast<const Storage*>(&rhs);
        }

        // no assignment operations

        SIMDIFY_INL const Storage* reset(const void* ptr) { m_data = static_cast<const Storage*>(ptr); return m_data; }
        SIMDIFY_INL const Storage*& ptr() { return m_data; }
        SIMDIFY_INL const Storage* ptr() const { return m_data; }

        // implicit conversion to const referred_t
        SIMDIFY_INL operator const referred_t() const { return referred_t(*m_data); }

        // data
        const Storage* m_data;
    };

    template <typename T>
    SIMDIFY_INL constexpr auto tof(storage<T> r) -> decltype(tof(T(r))) { return tof(T(r)); }
    template <typename T, std::size_t N>
    SIMDIFY_INL constexpr auto tof(aos_storage<T, N> r) -> decltype(tof(T(r))) { return tof(T(r)); }
    template <typename T>
    SIMDIFY_INL constexpr auto tof(reference<T> r) -> decltype(tof(*r.ptr())) { return tof(*r.ptr()); }
    template <typename T>
    SIMDIFY_INL constexpr auto tof(const_reference<T> r) -> decltype(tof(*r.ptr())) { return tof(*r.ptr()); }

    template <typename T>
    SIMDIFY_INL constexpr auto tou(storage<T> r) -> decltype(tou(T(r))) { return tou(T(r)); }
    template <typename T, std::size_t N>
    SIMDIFY_INL constexpr auto tou(aos_storage<T, N> r) -> decltype(tou(T(r))) { return tou(T(r)); }
    template <typename T>
    SIMDIFY_INL constexpr auto tou(reference<T> r) -> decltype(tou(*r.ptr())) { return tou(*r.ptr()); }
    template <typename T>
    SIMDIFY_INL constexpr auto tou(const_reference<T> r) -> decltype(tou(*r.ptr())) { return tou(*r.ptr()); }

    template <typename T>
    SIMDIFY_INL constexpr auto tos(storage<T> r) -> decltype(tos(T(r))) { return tos(T(r)); }
    template <typename T, std::size_t N>
    SIMDIFY_INL constexpr auto tos(aos_storage<T, N> r) -> decltype(tos(T(r))) { return tos(T(r)); }
    template <typename T>
    SIMDIFY_INL constexpr auto tos(reference<T> r) -> decltype(tos(*r.ptr())) { return tos(*r.ptr()); }
    template <typename T>
    SIMDIFY_INL constexpr auto tos(const_reference<T> r) -> decltype(tos(*r.ptr())) { return tos(*r.ptr()); }
}

#endif // SIMDIFY_STORAGE_IMPL
