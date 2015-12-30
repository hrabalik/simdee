#ifndef SIMDIFY_STORAGE_IMPL
#define SIMDIFY_STORAGE_IMPL

#include "common.hpp"
#include "../util/inline.hpp"

namespace simd {

    //
    // storage for SIMD or floating-point types
    //
    template <typename T, typename Enable = void>
    struct storage;

    //
    // specialized storage for SIMD types
    //
    template <typename Simd_t>
    struct alignas(Simd_t)storage<Simd_t, typename std::enable_if<is_simd_type<Simd_t>::value>::type> {
        using stored_t = Simd_t;
        using e_t = typename Simd_t::e_t;
        using data_t = std::array<e_t, Simd_t::W>;

        SIMDIFY_FORCE_INLINE constexpr storage() = default;
        SIMDIFY_FORCE_INLINE constexpr storage(const storage&) = default;
        SIMDIFY_FORCE_INLINE constexpr explicit storage(const data_t& rhs) : m_data(rhs) {}

        SIMDIFY_FORCE_INLINE explicit storage(const Simd_t& rhs) {
            rhs.store(data());
        }

        SIMDIFY_FORCE_INLINE storage& operator=(const storage&) = default;

        SIMDIFY_FORCE_INLINE storage& operator=(const Simd_t& rhs) {
            rhs.store(data());
            return *this;
        }

        SIMDIFY_FORCE_INLINE storage& operator=(const data_t& rhs) {
            m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE e_t* data() { return m_data.data(); }
        SIMDIFY_FORCE_INLINE const e_t* data() const { return m_data.data(); }
        SIMDIFY_FORCE_INLINE e_t& operator[](std::size_t i) { return m_data[i]; }
        SIMDIFY_FORCE_INLINE const e_t& operator[](std::size_t i) const { return m_data[i]; }

        // implicit conversion to Simd_t
        SIMDIFY_FORCE_INLINE operator Simd_t() const { Simd_t s; s.load(data()); return s; }

        // data
        data_t m_data;
    };

    //
    // specialized storage for arithmetic types
    //
    template <typename E_t>
    struct storage<E_t, typename std::enable_if<std::is_arithmetic<E_t>::value>::type> {
        using stored_t = E_t;

        SIMDIFY_FORCE_INLINE constexpr storage() = default;
        SIMDIFY_FORCE_INLINE constexpr storage(const storage&) = default;
        SIMDIFY_FORCE_INLINE explicit storage(const E_t& rhs) : m_data(rhs) {}

        SIMDIFY_FORCE_INLINE storage& operator=(const storage&) = default;

        SIMDIFY_FORCE_INLINE storage& operator=(const E_t& rhs) {
            m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE E_t* data() { return &m_data; }
        SIMDIFY_FORCE_INLINE const E_t* data() const { return &m_data; }

        // implicit conversion to E_t
        SIMDIFY_FORCE_INLINE operator E_t() const { return m_data; }

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

        SIMDIFY_FORCE_INLINE constexpr aos_storage() = default;

        SIMDIFY_FORCE_INLINE aos_storage(const aos_storage& rhs) {
            operator=(rhs);
        }

        SIMDIFY_FORCE_INLINE explicit aos_storage(const Simd_t& rhs) {
            rhs.interleaved_store(data(), N);
        }

        SIMDIFY_FORCE_INLINE aos_storage& operator=(const aos_storage& rhs) {
            auto lp = data();
            auto rp = rhs.data();
            for (std::size_t i = 0; i < W; ++i, lp += N, rp += N) {
                *lp = *rp;
            }
        }

        SIMDIFY_FORCE_INLINE aos_storage& operator=(const Simd_t& rhs) {
            rhs.interleaved_store(data(), N);
            return *this;
        }

        SIMDIFY_FORCE_INLINE e_t* data() { return m_data.data(); }
        SIMDIFY_FORCE_INLINE const e_t* data() const { return m_data.data(); }
        SIMDIFY_FORCE_INLINE e_t& operator[](std::size_t i) { return m_data[i * N]; }
        SIMDIFY_FORCE_INLINE const e_t& operator[](std::size_t i) const { return m_data[i * N]; }

        // implicit conversion to Simd_t
        SIMDIFY_FORCE_INLINE operator Simd_t() const {
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

        SIMDIFY_FORCE_INLINE constexpr reference() = default;
        SIMDIFY_FORCE_INLINE constexpr reference(const reference&) = default;

        SIMDIFY_FORCE_INLINE reference& operator=(const reference& rhs) {
            *m_data = *rhs.m_data;
            return *this;
        }

        SIMDIFY_FORCE_INLINE reference& operator=(const Storage& rhs) {
            *m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE reference& operator=(const referred_t& rhs) {
            *m_data = rhs;
            return *this;
        }

        SIMDIFY_FORCE_INLINE Storage* reset(void* ptr) { m_data = static_cast<Storage*>(ptr); return m_data; }
        SIMDIFY_FORCE_INLINE Storage*& ptr() { return m_data; }
        SIMDIFY_FORCE_INLINE Storage* ptr() const { return m_data; }

        // implicit conversion to const referred_t
        SIMDIFY_FORCE_INLINE operator const referred_t() const { return referred_t(*m_data); }

        // data
        Storage* m_data;
    };

    //
    // const reference to storage (proxy object)
    //
    template <typename Storage>
    struct const_reference {
        using referred_t = typename Storage::stored_t;

        SIMDIFY_FORCE_INLINE constexpr const_reference() = default;
        SIMDIFY_FORCE_INLINE constexpr const_reference(const const_reference&) = default;

        // no assignment operations

        SIMDIFY_FORCE_INLINE Storage* reset(void* ptr) { m_data = static_cast<Storage*>(ptr); return m_data; }
        SIMDIFY_FORCE_INLINE Storage*& ptr() { return m_data; }
        SIMDIFY_FORCE_INLINE Storage* ptr() const { return m_data; }

        // implicit conversion to const referred_t
        SIMDIFY_FORCE_INLINE operator const referred_t() const { return referred_t(*m_data); }

        // data
        Storage* m_data;
    };

}

#endif // SIMDIFY_STORAGE_IMPL
