#ifndef SIMDIFY_VEC3_FLT
#define SIMDIFY_VEC3_FLT

#include "../vec3.hpp"
#include "../util/inline.hpp"

#ifdef SIMDIFY_SSE

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    template <>
    struct basic_vec3<flt> {
        using mm_t = flt::mm_t;
        using fp_t = flt::fp_t;
        using bitmask_t = flt::bitmask_t;

        union {
            struct { fp_t x, y, z, extra; };
            sse::mm_t mm;
        };

        INL basic_vec3() = default;
        INL basic_vec3(const basic_vec3& other) : mm(other.mm) {}
        INL basic_vec3& operator=(const basic_vec3& other) { mm = other.mm; return *this; }

        INL explicit basic_vec3(const flt& t) : x(t.mm), y(t.mm), z(t.mm) {}
        INL explicit basic_vec3(fp_t t) : x(t), y(t), z(t) {}
        INL explicit basic_vec3(const sse& w) : mm(w.mm) {}
        INL explicit basic_vec3(sse::mm_t& w) : mm(w) {}

        INL basic_vec3(const flt& tx, const flt& ty, const flt& tz) :
            x(tx.mm), y(ty.mm), z(tz.mm) {}
        INL basic_vec3(fp_t tx, fp_t ty, fp_t tz) :
            x(tx), y(ty), z(tz) {}

        template <typename S>
        INL explicit basic_vec3(const S& other) : x(other.x), y(other.y), z(other.z) {}

        template <typename S>
        INL basic_vec3& operator=(const S& other) {
            x = other.x; y = other.y; z = other.z;
            return *this;
        }

        INL basic_vec3& operator[](std::size_t) { return *this; }
        INL const basic_vec3& operator[](std::size_t) const { return *this; }
    };

    using vec3f = basic_vec3<flt>;

    template <>
    struct bitwise_not<vec3f> {
        vec3f neg;
        INL explicit bitwise_not(const vec3f& r) : neg(r) {}
        INL explicit operator const vec3f() const { return vec3f(sse(~neg.mm)); }
    };

    INL const vec3f operator&(const vec3f& l, const vec3f& r) { return vec3f(l.mm & r.mm); }
    INL const vec3f operator|(const vec3f& l, const vec3f& r) { return vec3f(l.mm | r.mm); }
    INL const vec3f operator^(const vec3f& l, const vec3f& r) { return vec3f(l.mm ^ r.mm); }
    INL const bitwise_not<vec3f> operator~(const vec3f& l) { return bitwise_not<vec3f>(l); }
    INL const vec3f operator<(const vec3f& l, const vec3f& r) { return vec3f(l.mm < r.mm); }
    INL const vec3f operator>(const vec3f& l, const vec3f& r) { return vec3f(l.mm > r.mm); }
    INL const vec3f operator<=(const vec3f& l, const vec3f& r) { return vec3f(l.mm <= r.mm); }
    INL const vec3f operator>=(const vec3f& l, const vec3f& r) { return vec3f(l.mm >= r.mm); }
    INL const vec3f operator==(const vec3f& l, const vec3f& r) { return vec3f(l.mm == r.mm); }
    INL const vec3f operator!=(const vec3f& l, const vec3f& r) { return vec3f(l.mm != r.mm); }
    INL const vec3f operator+(const vec3f& l, const vec3f& r) { return vec3f(l.mm + r.mm); }
    INL const vec3f operator-(const vec3f& l, const vec3f& r) { return vec3f(l.mm - r.mm); }
    INL const vec3f operator*(const vec3f& l, const vec3f& r) { return vec3f(l.mm * r.mm); }
    INL const vec3f operator/(const vec3f& l, const vec3f& r) { return vec3f(l.mm / r.mm); }
    INL const vec3f andnot(const vec3f& l, const vec3f& r) { return vec3f(andnot(l.mm, r.mm)); }
    INL const vec3f min(const vec3f& l, const vec3f& r) { return vec3f(min(l.mm, r.mm)); }
    INL const vec3f max(const vec3f& l, const vec3f& r) { return vec3f(max(l.mm, r.mm)); }
    INL const vec3f cond(const vec3f& p, const vec3f& t, const vec3f& f) { return vec3f(cond(p.mm, t.mm, f.mm)); }
}

#undef INL

#endif // SIMDIFY_SSE

#endif // SIMDIFY_VEC3_FLT
