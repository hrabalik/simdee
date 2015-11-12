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

        template <std::size_t I, typename T, typename Enable = void>
        struct Get;

        template <std::size_t I, typename T, id FirstId, id... Ids>
        struct Get<I, id_pack<T, FirstId, Ids...>, typename std::enable_if<I != 0>::type> {
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
        //void swap(named_array& rhs) {}
    };
}

namespace std {
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

    //template <typename T, simd::id... Ids>
    //void swap(simd::named_array<T, Ids...>& lhs, simd::named_array<T, Ids...>& rhs) { lhs.swap(rhs); }
    template <typename T, simd::id... Ids>
    class tuple_size<simd::named_array<T, Ids...>> : public integral_constant<std::size_t, sizeof...(Ids)> {};
    template <std::size_t I, typename T, simd::id... Ids>
    class tuple_element<I, simd::named_array<T, Ids...>> { using type = T; };
}

#undef SIMDIFY_ID_PACK_DECLARATION

#endif // SIMDIFY_NAMED_ARRAY
