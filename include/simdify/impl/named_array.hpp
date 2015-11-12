#ifndef SIMDIFY_NAMED_ARRAY
#define SIMDIFY_NAMED_ARRAY

#include "../containers.hpp"
#include "../util/inline.hpp"
#include <array>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_ID_PACK_DECLARATION(IDENTIFIER)                                                   \
template <typename T, id... Ids>                                                                  \
struct id_pack<T, id::IDENTIFIER, Ids...> : id_pack<T, Ids...> {                                  \
    T IDENTIFIER;                                                                                 \
    SIMDIFY_FORCE_INLINE T& get() { return IDENTIFIER; }                                          \
    SIMDIFY_FORCE_INLINE constexpr const T& get() const { return IDENTIFIER; }                    \
};                                                                                                \
///////////////////////////////////////////////////////////////////////////////////////////////////

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

        SIMDIFY_ID_PACK_DECLARATION(a);
        SIMDIFY_ID_PACK_DECLARATION(b);
        SIMDIFY_ID_PACK_DECLARATION(c);
        SIMDIFY_ID_PACK_DECLARATION(d);
        SIMDIFY_ID_PACK_DECLARATION(e);
        SIMDIFY_ID_PACK_DECLARATION(f);
        SIMDIFY_ID_PACK_DECLARATION(g);
        SIMDIFY_ID_PACK_DECLARATION(h);
        SIMDIFY_ID_PACK_DECLARATION(i);
        SIMDIFY_ID_PACK_DECLARATION(j);
        SIMDIFY_ID_PACK_DECLARATION(k);
        SIMDIFY_ID_PACK_DECLARATION(l);
        SIMDIFY_ID_PACK_DECLARATION(m);
        SIMDIFY_ID_PACK_DECLARATION(n);
        SIMDIFY_ID_PACK_DECLARATION(o);
        SIMDIFY_ID_PACK_DECLARATION(p);
        SIMDIFY_ID_PACK_DECLARATION(q);
        SIMDIFY_ID_PACK_DECLARATION(r);
        SIMDIFY_ID_PACK_DECLARATION(s);
        SIMDIFY_ID_PACK_DECLARATION(t);
        SIMDIFY_ID_PACK_DECLARATION(u);
        SIMDIFY_ID_PACK_DECLARATION(v);
        SIMDIFY_ID_PACK_DECLARATION(w);
        SIMDIFY_ID_PACK_DECLARATION(x);
        SIMDIFY_ID_PACK_DECLARATION(y);
        SIMDIFY_ID_PACK_DECLARATION(z);

        template <std::size_t I, typename T, id FirstId, id... Ids, typename = typename std::enable_if<I != 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr T& get(id_pack<T, FirstId, Ids...>& pack) {
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

        template <std::size_t I, typename T, id FirstId, id... Ids, typename = typename std::enable_if<I != 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr T&& get(id_pack<T, FirstId, Ids...>&& pack) {
            return get<I - 1>(static_cast<id_pack<T, Ids...>&&>(pack));
        }

        template <std::size_t I, typename T, id... Ids, typename = typename std::enable_if<I == 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr T&& get(id_pack<T, Ids...>&& pack) {
            return pack.get();
        }

        template <std::size_t I, typename T, id FirstId, id... Ids, typename = typename std::enable_if<I != 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr const T&& get(const id_pack<T, FirstId, Ids...>&& pack) {
            return get<I - 1>(static_cast<const id_pack<T, Ids...>&&>(pack));
        }

        template <std::size_t I, typename T, id... Ids, typename = typename std::enable_if<I == 0, void>::type>
        SIMDIFY_FORCE_INLINE constexpr const T&& get(const id_pack<T, Ids...>&& pack) {
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
    SIMDIFY_FORCE_INLINE constexpr T& get(simd::named_array<T, Ids...>& a) {
        return simd::detail::get<I>(static_cast<simd::detail::id_pack<T, Ids...>&>(a));
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr T&& get(simd::named_array<T, Ids...>&& a) {
        return simd::detail::get<I>(static_cast<simd::detail::id_pack<T, Ids...>&&>(a));
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T& get(const simd::named_array<T, Ids...>& a) {
        return simd::detail::get<I>(static_cast<const simd::detail::id_pack<T, Ids...>&>(a));
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T&& get(const simd::named_array<T, Ids...>&& a) {
        return simd::detail::get<I>(static_cast<const simd::detail::id_pack<T, Ids...>&&>(a));
    }

    template <typename T, simd::id... Ids>
    void swap(simd::named_array<T, Ids...>& lhs, simd::named_array<T, Ids...>& rhs) { lhs.swap(rhs); }
    template <typename T, simd::id... Ids>
    class tuple_size<simd::named_array<T, Ids...>> : integral_constant<std::size_t, sizeof...(Ids)> {};
    template <std::size_t I, typename T, simd::id... Ids>
    class tuple_element<I, simd::named_array<T, Ids...>> { using type = T; };
}

#undef SIMDIFY_ID_PACK_DECLARATION

#endif // SIMDIFY_NAMED_ARRAY
