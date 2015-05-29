#ifndef SIMDIFY_VEC3_FLT
#define SIMDIFY_VEC3_FLT

#include "../vec3.hpp"
#include "../util/inline.hpp"

#ifdef SIMDIFY_SSE

namespace simd {

    template <>
    struct basic_vec3<flt> {
        using mm_t = flt::mm_t;
        using fp_t = flt::fp_t;
        using bitmask_t = flt::bitmask_t;

        flt x, y, z, extra;

        SIMDIFY_FORCE_INLINE basic_vec3() = default;
        SIMDIFY_FORCE_INLINE basic_vec3(const basic_vec3& other) { as_sse() = other.as_sse(); }
        SIMDIFY_FORCE_INLINE basic_vec3& operator=(const basic_vec3& other) { as_sse() = other.as_sse(); return *this; }

        SIMDIFY_FORCE_INLINE explicit basic_vec3(const flt& t) : x(t), y(t), z(t) {}
        SIMDIFY_FORCE_INLINE explicit basic_vec3(fp_t t) : x(t), y(t), z(t) {}
        SIMDIFY_FORCE_INLINE explicit basic_vec3(const sse& w) {}

        SIMDIFY_FORCE_INLINE basic_vec3(const flt& tx, const flt& ty, const flt& tz) :
            x(tx), y(ty), z(tz) {}
        SIMDIFY_FORCE_INLINE basic_vec3(fp_t tx, fp_t ty, fp_t tz) :
            x(tx), y(ty), z(tz) {}

        template <typename S>
        SIMDIFY_FORCE_INLINE explicit basic_vec3(const S& other) : x(other.x), y(other.y), z(other.z) {}

        template <typename S>
        SIMDIFY_FORCE_INLINE basic_vec3& operator=(const S& other) {
            x = other.x; y = other.y; z = other.z;
            return *this;
        }

        SIMDIFY_FORCE_INLINE basic_vec3& operator[](std::size_t) { return *this; }
        SIMDIFY_FORCE_INLINE const basic_vec3& operator[](std::size_t) const { return *this; }

        SIMDIFY_FORCE_INLINE sse& as_sse() { return reinterpret_cast<sse&>(x); }
        SIMDIFY_FORCE_INLINE const sse& as_sse() const { return reinterpret_cast<const sse&>(x); }
    };

    using vec3f = basic_vec3<flt>;

    template <>
    struct bitwise_not<vec3f> {
        vec3f neg;
        SIMDIFY_FORCE_INLINE explicit bitwise_not(const vec3f& r) : neg(r) {}
        SIMDIFY_FORCE_INLINE explicit operator const vec3f() const { return vec3f(sse(~neg.as_sse())); }
    };

    SIMDIFY_FORCE_INLINE const vec3f operator&(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() & r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator|(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() | r.as_sse()); }
    SIMDIFY_FORCE_INLINE const vec3f operator^(const vec3f& l, const vec3f& r) { return vec3f(l.as_sse() ^ r.as_sse()); }
    SIMDIFY_FORCE_INLINE const bitwise_not<vec3f> operator~(const vec3f& l) { return bitwise_not<vec3f>(l); }
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
