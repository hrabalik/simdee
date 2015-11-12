#ifndef SIMDIFY_NAMED_ARRAY
#define SIMDIFY_NAMED_ARRAY

#include "../containers.hpp"
#include "../util/inline.hpp"
#include <array>
#include <type_traits>

namespace simd {
    enum class id {
        a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
    };

    namespace detail {
        template <std::size_t N, id... Ids>
        struct find_nth_id;
        template <std::size_t N, id FirstId, id... Ids>
        struct find_nth_id<N, FirstId, Ids...> : find_nth_id<N - 1, Ids...> {};
        template <id FirstId, id... Ids>
        struct find_nth_id<0, FirstId, Ids...> { static constexpr id value = FirstId; };

        template <id... Ids>
        struct find_last_id : find_nth_id<sizeof...(Ids)-1, Ids...> {};

        template <typename T, id... Ids>
        struct id_pack;

        template <typename T>
        struct id_pack<T> {};

        template <typename T, id... Ids> struct id_pack<T, id::a, Ids...> : id_pack<T, Ids...> { T a; SIMDIFY_FORCE_INLINE T& get() { return a; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return a; } };
        template <typename T, id... Ids> struct id_pack<T, id::b, Ids...> : id_pack<T, Ids...> { T b; SIMDIFY_FORCE_INLINE T& get() { return b; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return b; } };
        template <typename T, id... Ids> struct id_pack<T, id::c, Ids...> : id_pack<T, Ids...> { T c; SIMDIFY_FORCE_INLINE T& get() { return c; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return c; } };
        template <typename T, id... Ids> struct id_pack<T, id::d, Ids...> : id_pack<T, Ids...> { T d; SIMDIFY_FORCE_INLINE T& get() { return d; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return d; } };
        template <typename T, id... Ids> struct id_pack<T, id::e, Ids...> : id_pack<T, Ids...> { T e; SIMDIFY_FORCE_INLINE T& get() { return e; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return e; } };
        template <typename T, id... Ids> struct id_pack<T, id::f, Ids...> : id_pack<T, Ids...> { T f; SIMDIFY_FORCE_INLINE T& get() { return f; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return f; } };
        template <typename T, id... Ids> struct id_pack<T, id::g, Ids...> : id_pack<T, Ids...> { T g; SIMDIFY_FORCE_INLINE T& get() { return g; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return g; } };
        template <typename T, id... Ids> struct id_pack<T, id::h, Ids...> : id_pack<T, Ids...> { T h; SIMDIFY_FORCE_INLINE T& get() { return h; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return h; } };
        template <typename T, id... Ids> struct id_pack<T, id::i, Ids...> : id_pack<T, Ids...> { T i; SIMDIFY_FORCE_INLINE T& get() { return i; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return i; } };
        template <typename T, id... Ids> struct id_pack<T, id::j, Ids...> : id_pack<T, Ids...> { T j; SIMDIFY_FORCE_INLINE T& get() { return j; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return j; } };
        template <typename T, id... Ids> struct id_pack<T, id::k, Ids...> : id_pack<T, Ids...> { T k; SIMDIFY_FORCE_INLINE T& get() { return k; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return k; } };
        template <typename T, id... Ids> struct id_pack<T, id::l, Ids...> : id_pack<T, Ids...> { T l; SIMDIFY_FORCE_INLINE T& get() { return l; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return l; } };
        template <typename T, id... Ids> struct id_pack<T, id::m, Ids...> : id_pack<T, Ids...> { T m; SIMDIFY_FORCE_INLINE T& get() { return m; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return m; } };
        template <typename T, id... Ids> struct id_pack<T, id::n, Ids...> : id_pack<T, Ids...> { T n; SIMDIFY_FORCE_INLINE T& get() { return n; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return n; } };
        template <typename T, id... Ids> struct id_pack<T, id::o, Ids...> : id_pack<T, Ids...> { T o; SIMDIFY_FORCE_INLINE T& get() { return o; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return o; } };
        template <typename T, id... Ids> struct id_pack<T, id::p, Ids...> : id_pack<T, Ids...> { T p; SIMDIFY_FORCE_INLINE T& get() { return p; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return p; } };
        template <typename T, id... Ids> struct id_pack<T, id::q, Ids...> : id_pack<T, Ids...> { T q; SIMDIFY_FORCE_INLINE T& get() { return q; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return q; } };
        template <typename T, id... Ids> struct id_pack<T, id::r, Ids...> : id_pack<T, Ids...> { T r; SIMDIFY_FORCE_INLINE T& get() { return r; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return r; } };
        template <typename T, id... Ids> struct id_pack<T, id::s, Ids...> : id_pack<T, Ids...> { T s; SIMDIFY_FORCE_INLINE T& get() { return s; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return s; } };
        template <typename T, id... Ids> struct id_pack<T, id::t, Ids...> : id_pack<T, Ids...> { T t; SIMDIFY_FORCE_INLINE T& get() { return t; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return t; } };
        template <typename T, id... Ids> struct id_pack<T, id::u, Ids...> : id_pack<T, Ids...> { T u; SIMDIFY_FORCE_INLINE T& get() { return u; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return u; } };
        template <typename T, id... Ids> struct id_pack<T, id::v, Ids...> : id_pack<T, Ids...> { T v; SIMDIFY_FORCE_INLINE T& get() { return v; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return v; } };
        template <typename T, id... Ids> struct id_pack<T, id::w, Ids...> : id_pack<T, Ids...> { T w; SIMDIFY_FORCE_INLINE T& get() { return w; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return w; } };
        template <typename T, id... Ids> struct id_pack<T, id::x, Ids...> : id_pack<T, Ids...> { T x; SIMDIFY_FORCE_INLINE T& get() { return x; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return x; } };
        template <typename T, id... Ids> struct id_pack<T, id::y, Ids...> : id_pack<T, Ids...> { T y; SIMDIFY_FORCE_INLINE T& get() { return y; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return y; } };
        template <typename T, id... Ids> struct id_pack<T, id::z, Ids...> : id_pack<T, Ids...> { T z; SIMDIFY_FORCE_INLINE T& get() { return z; } SIMDIFY_FORCE_INLINE constexpr const T& get() const { return z; } };

        template <std::size_t I, typename T, id FirstId, id... Ids, typename = typename std::enable_if<I != 0, void>::type>
        SIMDIFY_FORCE_INLINE T& get(id_pack<T, FirstId, Ids...>& pack) {
            return get<I - 1>(static_cast<id_pack<T, Ids...>&>(pack));
        }

        template <std::size_t I, typename T, id... Ids, typename = typename std::enable_if<I == 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr T& get(id_pack<T, Ids...>& pack) {
            return pack.get();
        }

        template <std::size_t I, typename T, id FirstId, id... Ids, typename = typename std::enable_if<I != 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr const T& get(const id_pack<T, FirstId, Ids...>& pack) {
            return get<I - 1>(static_cast<const id_pack<T, Ids...>&>(pack));
        }

        template <std::size_t I, typename T, id... Ids, typename = typename std::enable_if<I == 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr const T& get(const id_pack<T, Ids...>& pack) {
            return pack.get();
        }
    }

    template <typename T, id... Ids>
    struct named_array : detail::id_pack<T, Ids...> {
        using std_array_t = std::array<T, sizeof...(Ids)>;
        using iterator = typename std_array_t::iterator;
        using const_iterator = typename std_array_t::const_iterator;
        using reverse_iterator = typename std_array_t::reverse_iterator;
        using const_reverse_iterator = typename std_array_t::const_reverse_iterator;

        SIMDIFY_FORCE_INLINE constexpr named_array() = default;
        SIMDIFY_FORCE_INLINE constexpr named_array(const named_array&) = default;
        SIMDIFY_FORCE_INLINE constexpr named_array(named_array&&) = default;

        SIMDIFY_FORCE_INLINE std_array_t& as_std_array() & { return reinterpret_cast<std_array_t&>(*this); }
        SIMDIFY_FORCE_INLINE constexpr const std_array_t& as_std_array() const& { return reinterpret_cast<const std_array_t&>(*this); }
        SIMDIFY_FORCE_INLINE std_array_t&& as_std_array() && { return reinterpret_cast<std_array_t &&>(*this); }
        SIMDIFY_FORCE_INLINE constexpr const std_array_t&& as_std_array() const&& { return reinterpret_cast<const std_array_t&&>(*this); }
        SIMDIFY_FORCE_INLINE T& at(std::size_t i) { return as_std_array().at(i); }
        SIMDIFY_FORCE_INLINE constexpr const T& at(std::size_t i) const { return as_std_array().at(i); }
        SIMDIFY_FORCE_INLINE T& operator[](std::size_t i) { return as_std_array()[i]; }
        SIMDIFY_FORCE_INLINE constexpr const T& operator[](std::size_t i) const { return as_std_array()[i]; }
        SIMDIFY_FORCE_INLINE constexpr const bool empty() const { return sizeof...(Ids) != 0; }
        SIMDIFY_FORCE_INLINE constexpr const std::size_t size() const { return sizeof...(Ids); }
        SIMDIFY_FORCE_INLINE void fill(const T& val) { as_std_array().fill(val); }
        SIMDIFY_FORCE_INLINE void swap(named_array& rhs) { as_std_array().swap(rhs.as_std_array()); }

        SIMDIFY_FORCE_INLINE iterator begin() { return as_std_array().begin(); }
        SIMDIFY_FORCE_INLINE iterator end() { return as_std_array().end(); }
        SIMDIFY_FORCE_INLINE constexpr const_iterator begin() const { return as_std_array().begin(); }
        SIMDIFY_FORCE_INLINE constexpr const_iterator end() const { return as_std_array().end(); }
        SIMDIFY_FORCE_INLINE constexpr const_iterator cbegin() const { return as_std_array().cbegin(); }
        SIMDIFY_FORCE_INLINE constexpr const_iterator cend() const { return as_std_array().cend(); }
        SIMDIFY_FORCE_INLINE reverse_iterator rbegin() { return as_std_array().rbegin(); }
        SIMDIFY_FORCE_INLINE reverse_iterator rend() { return as_std_array().rend(); }
        SIMDIFY_FORCE_INLINE constexpr const_reverse_iterator rbegin() const { return as_std_array().rbegin(); }
        SIMDIFY_FORCE_INLINE constexpr const_reverse_iterator rend() const { return as_std_array().rend(); }
        SIMDIFY_FORCE_INLINE constexpr const_reverse_iterator crbegin() const { return as_std_array().crbegin(); }
        SIMDIFY_FORCE_INLINE constexpr const_reverse_iterator crend() const { return as_std_array().crend(); }
    };

    template <typename T, id... Ids>
    bool operator==(const named_array<T, Ids...>& lhs, const named_array<T, Ids...>& rhs) {
        return lhs.as_std_array() == rhs.as_std_array();
    }
    template <typename T, id... Ids>
    bool operator!=(const named_array<T, Ids...>& lhs, const named_array<T, Ids...>& rhs) {
        return lhs.as_std_array() != rhs.as_std_array();
    }
}

namespace std {
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr T& get(simd::named_array<T, Ids...>& a) { return get<I>(a.as_std_array()); }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr T&& get(simd::named_array<T, Ids...>&& a) { return get<I>(a.as_std_array()); }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T& get(const simd::named_array<T, Ids...>& a) { return get<I>(a.as_std_array()); }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T&& get(const simd::named_array<T, Ids...>&& a) { return get<I>(a.as_std_array()); }

    template <typename T, simd::id... Ids>
    void swap(simd::named_array<T, Ids...>& lhs, simd::named_array<T, Ids...>& rhs) { lhs.swap(rhs); }
    template <typename T, simd::id... Ids>
    class tuple_size<simd::named_array<T, Ids...>> : integral_constant<std::size_t, sizeof...(Ids)> {};
    template <std::size_t I, typename T, simd::id... Ids>
    class tuple_element<I, simd::named_array<T, Ids...>> { using type = T; };
}

#endif // SIMDIFY_NAMED_ARRAY
