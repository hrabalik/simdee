#ifndef SIMDIFY_ID
#define SIMDIFY_ID

#include "../containers.hpp"
#include <array>
#include <type_traits>

namespace simd {
    enum class id {
        a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
    };

    namespace detail {
        template <typename T, id... Ids>
        struct id_pack;

        template <typename T>
        struct id_pack<T> {};

        template <typename T, id... Ids> struct id_pack<T, id::a, Ids...> : id_pack<T, Ids...> { T a; };
        template <typename T, id... Ids> struct id_pack<T, id::b, Ids...> : id_pack<T, Ids...> { T b; };
        template <typename T, id... Ids> struct id_pack<T, id::c, Ids...> : id_pack<T, Ids...> { T c; };
        template <typename T, id... Ids> struct id_pack<T, id::d, Ids...> : id_pack<T, Ids...> { T d; };
        template <typename T, id... Ids> struct id_pack<T, id::e, Ids...> : id_pack<T, Ids...> { T e; };
        template <typename T, id... Ids> struct id_pack<T, id::f, Ids...> : id_pack<T, Ids...> { T f; };
        template <typename T, id... Ids> struct id_pack<T, id::g, Ids...> : id_pack<T, Ids...> { T g; };
        template <typename T, id... Ids> struct id_pack<T, id::h, Ids...> : id_pack<T, Ids...> { T h; };
        template <typename T, id... Ids> struct id_pack<T, id::i, Ids...> : id_pack<T, Ids...> { T i; };
        template <typename T, id... Ids> struct id_pack<T, id::j, Ids...> : id_pack<T, Ids...> { T j; };
        template <typename T, id... Ids> struct id_pack<T, id::k, Ids...> : id_pack<T, Ids...> { T k; };
        template <typename T, id... Ids> struct id_pack<T, id::l, Ids...> : id_pack<T, Ids...> { T l; };
        template <typename T, id... Ids> struct id_pack<T, id::m, Ids...> : id_pack<T, Ids...> { T m; };
        template <typename T, id... Ids> struct id_pack<T, id::n, Ids...> : id_pack<T, Ids...> { T n; };
        template <typename T, id... Ids> struct id_pack<T, id::o, Ids...> : id_pack<T, Ids...> { T o; };
        template <typename T, id... Ids> struct id_pack<T, id::p, Ids...> : id_pack<T, Ids...> { T p; };
        template <typename T, id... Ids> struct id_pack<T, id::q, Ids...> : id_pack<T, Ids...> { T q; };
        template <typename T, id... Ids> struct id_pack<T, id::r, Ids...> : id_pack<T, Ids...> { T r; };
        template <typename T, id... Ids> struct id_pack<T, id::s, Ids...> : id_pack<T, Ids...> { T s; };
        template <typename T, id... Ids> struct id_pack<T, id::t, Ids...> : id_pack<T, Ids...> { T t; };
        template <typename T, id... Ids> struct id_pack<T, id::u, Ids...> : id_pack<T, Ids...> { T u; };
        template <typename T, id... Ids> struct id_pack<T, id::v, Ids...> : id_pack<T, Ids...> { T v; };
        template <typename T, id... Ids> struct id_pack<T, id::w, Ids...> : id_pack<T, Ids...> { T w; };
        template <typename T, id... Ids> struct id_pack<T, id::x, Ids...> : id_pack<T, Ids...> { T x; };
        template <typename T, id... Ids> struct id_pack<T, id::y, Ids...> : id_pack<T, Ids...> { T y; };
        template <typename T, id... Ids> struct id_pack<T, id::z, Ids...> : id_pack<T, Ids...> { T z; };
    }

    template <typename T, id... Ids>
    struct named_array : detail::id_pack<T, Ids...> {
        using std_array_t = std::array<T, sizeof...(Ids)>;
        static_assert(std::is_trivial<T>::value, "named_array: element type not trivial");
        static_assert(sizeof(detail::id_pack<T, Ids...>) == sizeof(std_array_t), "named_array: bad size");

        std_array_t& as_std_array() { return reinterpret_cast<std_array_t&>(*this); }
        const std_array_t& as_std_array() const { return reinterpret_cast<const std_array_t&>(*this); }
    };
    
}

#endif // SIMDIFY_ID
