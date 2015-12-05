#ifndef SIMDIFY_NAMED_ARRAY
#define SIMDIFY_NAMED_ARRAY

#include "expr.hpp"
#include "../util/inline.hpp"
#include <tuple>
#include <array>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////////////////////////
#define SIMDIFY_ID_PACK_DECLARATION(IDENTIFIER)                                                   \
                                                                                                  \
template <typename T, id... Ids>                                                                  \
struct id_pack<T, id::IDENTIFIER, Ids...> : id_pack<T, Ids...> {                                  \
    SIMDIFY_FORCE_INLINE constexpr id_pack() = default;                                           \
    SIMDIFY_FORCE_INLINE constexpr id_pack(const id_pack&) = default;                             \
    SIMDIFY_FORCE_INLINE constexpr id_pack(id_pack&&) = default;                                  \
    SIMDIFY_FORCE_INLINE id_pack& operator=(const id_pack&) = default;                            \
    SIMDIFY_FORCE_INLINE id_pack& operator=(id_pack&&) = default;                                 \
                                                                                                  \
    template <typename... Args>                                                                   \
    SIMDIFY_FORCE_INLINE constexpr id_pack(const std::tuple<Args...>& t) :                        \
        id_pack<T, Ids...>(t),                                                                    \
        IDENTIFIER(std::get<sizeof...(Args) - sizeof...(Ids) - 1>(t)) {}                          \
                                                                                                  \
    SIMDIFY_FORCE_INLINE T& get() { return IDENTIFIER; }                                          \
    SIMDIFY_FORCE_INLINE constexpr const T& get() const { return IDENTIFIER; }                    \
                                                                                                  \
    T IDENTIFIER;                                                                                 \
};                                                                                                \
                                                                                                  \
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace simd {
    enum class id {
        a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
    };

    template <id... Ids>
    struct id_sequence {};

    namespace detail {
        template <typename... Args>
        SIMDIFY_FORCE_INLINE constexpr int no_op(Args&&...) { return 0; }

        template <typename T, id... Ids>
        struct id_pack;

        template <typename T>
        struct id_pack<T> {
            SIMDIFY_FORCE_INLINE constexpr id_pack() = default;
            SIMDIFY_FORCE_INLINE constexpr id_pack(const id_pack&) = default;
            SIMDIFY_FORCE_INLINE constexpr id_pack(id_pack&&) = default;
            SIMDIFY_FORCE_INLINE id_pack& operator=(const id_pack&) = default;
            SIMDIFY_FORCE_INLINE id_pack& operator=(id_pack&&) = default;

            template <typename... Args>
            SIMDIFY_FORCE_INLINE constexpr id_pack(const std::tuple<Args...>&) {}
        };

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

        template <std::size_t I, typename T, typename Enable = void>
        struct Get;

        template <std::size_t I, typename T, id FirstId, id... Ids>
        struct Get<I, id_pack<T, FirstId, Ids...>, typename std::enable_if<I != 0>::type> {
            static_assert(sizeof...(Ids) >= I, "retrieving element of id_pack: index out of bounds");

            SIMDIFY_FORCE_INLINE static constexpr T& get(id_pack<T, FirstId, Ids...>& pack) {
                return Get<I - 1, id_pack<T, Ids...>>::get(pack);
            }
            SIMDIFY_FORCE_INLINE static constexpr const T& get(const id_pack<T, FirstId, Ids...>& pack) {
                return Get<I - 1, id_pack<T, Ids...>>::get(pack);
            }
            SIMDIFY_FORCE_INLINE static constexpr T&& get(id_pack<T, FirstId, Ids...>&& pack) {
                return Get<I - 1, id_pack<T, Ids...>>::get(pack);
            }
            SIMDIFY_FORCE_INLINE static constexpr const T&& get(const id_pack<T, FirstId, Ids...>&& pack) {
                return Get<I - 1, id_pack<T, Ids...>>::get(pack);
            }
        };

        template <typename T, id... Ids>
        struct Get<0, id_pack<T, Ids...>> {
            static_assert(sizeof...(Ids) > 0, "retrieving element of id_pack: pack is empty");

            SIMDIFY_FORCE_INLINE static constexpr T& get(id_pack<T, Ids...>& pack) {
                return pack.get();
            }
            SIMDIFY_FORCE_INLINE static constexpr const T& get(const id_pack<T, Ids...>& pack) {
                return pack.get();
            }
            SIMDIFY_FORCE_INLINE static constexpr T&& get(id_pack<T, Ids...>&& pack) {
                return pack.get();
            }
            SIMDIFY_FORCE_INLINE static constexpr const T&& get(const id_pack<T, Ids...>&& pack) {
                return pack.get();
            }
        };
    }

    template <typename T, id... Ids>
    struct named_array : detail::id_pack<T, Ids...> {
        enum : std::size_t { N = sizeof...(Ids) };
        using all_elements = make_sequence_t<0, N>;

        constexpr named_array() = default;
        constexpr named_array(const named_array&) = default;
        constexpr named_array(named_array&&) = default;
        named_array& operator=(const named_array&) = default;
        named_array& operator=(named_array&&) = default;

        template <typename... Args>
        SIMDIFY_FORCE_INLINE constexpr named_array(const std::tuple<Args...>& t) :
            detail::id_pack<T, Ids...>(t) {
            static_assert(sizeof...(Args) == sizeof...(Ids), "named_array: incorrect number of parameters");
        }

        T& operator[](std::size_t i) {
            auto& as_array = reinterpret_cast<std::array<T, N>&>(*this);
            return as_array[(N - 1) - i];
        }

        const T& operator[](std::size_t i) const {
            auto& as_array = reinterpret_cast<const std::array<const T, N>&>(*this);
            return as_array[(N - 1) - i];
        }
    };

    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr T& get(simd::named_array<T, Ids...>& a) {
        return simd::detail::Get<I, simd::detail::id_pack<T, Ids...>>::get(a);
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr T&& get(simd::named_array<T, Ids...>&& a) {
        return simd::detail::Get<I, simd::detail::id_pack<T, Ids...>>::get(a);
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T& get(const simd::named_array<T, Ids...>& a) {
        return simd::detail::Get<I, simd::detail::id_pack<T, Ids...>>::get(a);
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T&& get(const simd::named_array<T, Ids...>&& a) {
        return simd::detail::Get<I, simd::detail::id_pack<T, Ids...>>::get(a);
    }
}

#undef SIMDIFY_ID_PACK_DECLARATION

#endif // SIMDIFY_NAMED_ARRAY
