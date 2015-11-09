#ifndef SIMDIFY_VEC3_FLT
#define SIMDIFY_VEC3_FLT

#include "../vec3.hpp"
#include "../util/inline.hpp"
#include "expr.hpp"

#ifdef SIMDIFY_SSE

namespace simd {

    //
    // A specialization of basic_vec3 with a single element, a. k. a. vec3f
    //
    template <>
    struct basic_vec3<flt> {
        // typedefs and constants
        using simd_t = flt;
        using mm_t = flt::mm_t;
        using f_t = flt::f_t;
        using u_t = flt::u_t;
        using s_t = flt::s_t;
        static constexpr auto W = flt::W;

        // constructors
        SIMDIFY_FORCE_INLINE basic_vec3() = default;
        SIMDIFY_FORCE_INLINE basic_vec3(const basic_vec3& other) { as_sse() = other.as_sse(); }
        SIMDIFY_FORCE_INLINE explicit basic_vec3(const flt& t) : x(t), y(t), z(t) {}
        SIMDIFY_FORCE_INLINE explicit basic_vec3(f_t t) : x(t), y(t), z(t) {}
        SIMDIFY_FORCE_INLINE explicit basic_vec3(const sse& w) {}
        SIMDIFY_FORCE_INLINE explicit basic_vec3(const expr::bit_not<basic_vec3>& r) { as_sse() = sse(~r.neg.as_sse()); }
        SIMDIFY_FORCE_INLINE basic_vec3(const flt& tx, const flt& ty, const flt& tz) :
            x(tx), y(ty), z(tz) {}
        SIMDIFY_FORCE_INLINE basic_vec3(f_t tx, f_t ty, f_t tz) :
            x(tx), y(ty), z(tz) {}

        // operator=
        SIMDIFY_FORCE_INLINE basic_vec3& operator=(const basic_vec3& other) { as_sse() = other.as_sse(); return *this; }

        // generalized constructor and operator= for any RHS that has fields x, y, z
        template <typename S>
        SIMDIFY_FORCE_INLINE explicit basic_vec3(const S& other) : x(other.x), y(other.y), z(other.z) {}
        template <typename S>
        SIMDIFY_FORCE_INLINE basic_vec3& operator=(const S& other) {
            x = other.x; y = other.y; z = other.z;
            return *this;
        }

        // operator[] has no effect for vec3f, 0 is its only valid input, returning the vec3f itself
        SIMDIFY_FORCE_INLINE basic_vec3& operator[](std::size_t) { return *this; }
        SIMDIFY_FORCE_INLINE const basic_vec3& operator[](std::size_t) const { return *this; }

        // cast self to sse - specific to vec3f
        SIMDIFY_FORCE_INLINE sse& as_sse() { return reinterpret_cast<sse&>(x); }
        SIMDIFY_FORCE_INLINE const sse& as_sse() const { return reinterpret_cast<const sse&>(x); }

        // data
        flt x, y, z, extra;
    };

    using vec3f = basic_vec3<flt>;

    SIMDIFY_FORCE_INLINE const vec3f operator&(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() & r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator|(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() | r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator^(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() ^ r.as_sse()); }
    SIMDIFY_FORCE_INLINE const expr::bit_not<vec3f> operator~(const vec3f& l) { return expr::bit_not<vec3f>(l); }
    SIMDIFY_FORCE_INLINE const vec3f operator<(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() < r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator>(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() > r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator<=(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() <= r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator>=(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() >= r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator==(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() == r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator!=(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() != r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator+(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() + r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator-(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() - r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator*(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() * r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator/(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() / r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f andnot(const vec3f& l, const vec3f& r) { return vec3f(andnot(l.as_sse(), r.as_sse())); }
    SIMDIFY_FORCE_INLINE const vec3f min(const vec3f& l, const vec3f& r) { return vec3f(min(l.as_sse(), r.as_sse())); }
    SIMDIFY_FORCE_INLINE const vec3f max(const vec3f& l, const vec3f& r) { return vec3f(max(l.as_sse(), r.as_sse())); }
    SIMDIFY_FORCE_INLINE const vec3f cond(const vec3f& p, const vec3f& t, const vec3f& f) { return vec3f(cond(p.as_sse(), t.as_sse(), f.as_sse())); }
}

#endif // SIMDIFY_SSE

#endif // SIMDIFY_VEC3_FLT
