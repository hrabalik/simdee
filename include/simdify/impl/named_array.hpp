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
struct id_pack<T, id_sequence<id::IDENTIFIER, Ids...>> : id_pack<T, id_sequence<Ids...>> {        \
    using base_t = id_pack<T, id_sequence<Ids...>>;                                               \
                                                                                                  \
    SIMDIFY_FORCE_INLINE constexpr id_pack() = default;                                           \
    SIMDIFY_FORCE_INLINE constexpr id_pack(const id_pack&) = default;                             \
    SIMDIFY_FORCE_INLINE constexpr id_pack(id_pack&&) = default;                                  \
    SIMDIFY_FORCE_INLINE id_pack& operator=(const id_pack&) = default;                            \
    SIMDIFY_FORCE_INLINE id_pack& operator=(id_pack&&) = default;                                 \
                                                                                                  \
    template <typename... Args>                                                                   \
    SIMDIFY_FORCE_INLINE constexpr id_pack(const std::tuple<Args...>& t) :                        \
        base_t(t),                                                                                \
        IDENTIFIER(std::get<sizeof...(Ids)>(t)) {}                                                \
                                                                                                  \
    template <typename... Args>                                                                   \
    SIMDIFY_FORCE_INLINE id_pack& operator=(const std::tuple<Args...>& t) {                       \
        base_t::operator=(t);                                                                     \
        IDENTIFIER = std::get<sizeof...(Ids)>(t);                                                 \
        return *this;                                                                             \
    }                                                                                             \
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

        template <typename T, typename IDSequence>
        struct id_pack;

        template <typename T>
        struct id_pack<T, id_sequence<>> {
            SIMDIFY_FORCE_INLINE constexpr id_pack() = default;
            SIMDIFY_FORCE_INLINE constexpr id_pack(const id_pack&) = default;
            SIMDIFY_FORCE_INLINE constexpr id_pack(id_pack&&) = default;
            SIMDIFY_FORCE_INLINE id_pack& operator=(const id_pack&) = default;
            SIMDIFY_FORCE_INLINE id_pack& operator=(id_pack&&) = default;

            template <typename... Args>
            SIMDIFY_FORCE_INLINE constexpr id_pack(const std::tuple<Args...>&) {}

            template <typename... Args>
            SIMDIFY_FORCE_INLINE id_pack& operator=(const std::tuple<Args...>&) { return *this; }
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

        template <typename In, typename Out>
        struct reverse_impl;

        template <id... Out>
        struct reverse_impl<id_sequence<>, id_sequence<Out...>> {
            using type = id_sequence<Out...>;
        };

        template <id First, id... In, id... Out>
        struct reverse_impl<id_sequence<First, In...>, id_sequence<Out...>> :
            reverse_impl<id_sequence<In...>, id_sequence<First, Out...>> {};

        template <id... Ids>
        struct reverse : reverse_impl<id_sequence<Ids...>, id_sequence<>> {};

        template <id... Ids>
        using reverse_t = typename reverse<Ids...>::type;
    }

    template <typename T, id... Ids>
    struct named_array : detail::id_pack<T, detail::reverse_t<Ids...>> {
        enum : std::size_t { N = sizeof...(Ids) };
        using base_t = detail::id_pack<T, detail::reverse_t<Ids...>>;

        constexpr named_array() = default;
        constexpr named_array(const named_array&) = default;
        constexpr named_array(named_array&&) = default;
        named_array& operator=(const named_array&) = default;
        named_array& operator=(named_array&&) = default;

        template <typename... Args>
        SIMDIFY_FORCE_INLINE constexpr named_array(const std::tuple<Args...>& t) : base_t(t) {
            static_assert(sizeof...(Args) == sizeof...(Ids), "named_array: incorrect number of parameters");
        }

        template <typename... Args>
        SIMDIFY_FORCE_INLINE named_array& operator=(const std::tuple<Args...>& t) {
            static_assert(sizeof...(Args) == sizeof...(Ids), "named_array operator=: incorrect number of parameters");
            base_t::operator=(t);
            return *this;
        }

        T& operator[](std::size_t i) {
            auto& as_array = reinterpret_cast<std::array<T, N>&>(*this);
            return as_array[i];
        }

        const T& operator[](std::size_t i) const {
            auto& as_array = reinterpret_cast<const std::array<const T, N>&>(*this);
            return as_array[i];
        }

        template <std::size_t I = 0>
        T& get() {
            auto& as_array = reinterpret_cast<std::array<T, N>&>(*this);
            return std::get<I>(as_array);
        }

        template <std::size_t I = 0>
        const T& get() const {
            auto& as_array = reinterpret_cast<const std::array<const T, N>&>(*this);
            return std::get<I>(as_array);
        }
    };

    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr T& get(simd::named_array<T, Ids...>& a) {
        return a.template get<I>();
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr T&& get(simd::named_array<T, Ids...>&& a) {
        return std::move(a.template get<I>());
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T& get(const simd::named_array<T, Ids...>& a) {
        return a.template get<I>();
    }
    template <size_t I, typename T, simd::id... Ids>
    SIMDIFY_FORCE_INLINE constexpr const T&& get(const simd::named_array<T, Ids...>&& a) {
        return std::move(a.template get<I>());
    }
}

#undef SIMDIFY_ID_PACK_DECLARATION

#endif // SIMDIFY_NAMED_ARRAY
