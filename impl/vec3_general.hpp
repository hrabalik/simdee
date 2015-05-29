#ifndef SIMDIFY_VEC3_GENERAL
#define SIMDIFY_VEC3_GENERAL

#include "../vec3.hpp"
#include "../util/inline.hpp"

namespace simd {

    template <typename T>
    struct basic_vec3 {
        using simd_t = T;
        using mm_t = typename T::mm_t;
        using fp_t = typename T::fp_t;
        using bitmask_t = typename T::bitmask_t;

        T x, y, z;

        SIMDIFY_FORCE_INLINE basic_vec3() = default;
        SIMDIFY_FORCE_INLINE basic_vec3(const basic_vec3& other) = default;
        SIMDIFY_FORCE_INLINE basic_vec3& operator=(const basic_vec3& other) = default;

        SIMDIFY_FORCE_INLINE explicit basic_vec3(const T& t) : x(t), y(t), z(t) {}

        SIMDIFY_FORCE_INLINE basic_vec3(const T& tx, const T& ty, const T& tz) :
            x(tx), y(ty), z(tz) {}

        template <typename S>
        SIMDIFY_FORCE_INLINE explicit basic_vec3(const S& other) : x(other.x), y(other.y), z(other.z) {}

        template <typename S>
        SIMDIFY_FORCE_INLINE basic_vec3& operator=(const S& other) {
            x = T(other.x); y = T(other.y); z = T(other.z);
            return *this;
        }

        // the value received by subscript behaves as a triple of floats x, y, z
        struct view {
            fp_t& x; fp_t& y; fp_t& z;

            SIMDIFY_FORCE_INLINE view(basic_vec3& v, std::size_t i) : x(v.x.f[i]), y(v.y.f[i]), z(v.z.f[i]) {}

            template <typename S>
            SIMDIFY_FORCE_INLINE view& operator=(const S& other) {
                x = other.x; y = other.y; z = other.z;
                return *this;
            }

            template <typename S>
            SIMDIFY_FORCE_INLINE explicit operator S() const {
                return S(x, y, z);
            }
        };

        SIMDIFY_FORCE_INLINE view operator[](std::size_t i) { return view(*this, i); }
        SIMDIFY_FORCE_INLINE const view operator[](std::size_t i) const { return view(*this, i); }
    };

    template <typename T>
    SIMDIFY_FORCE_INLINE const basic_vec3<T> cross(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return{ l.y*r.z - l.z*r.y, l.z*r.x - l.x*r.z, l.x*r.y - l.y*r.x };
    }

    template <typename T>
    SIMDIFY_FORCE_INLINE const T dot(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return l.x*r.x + l.y*r.y + l.z*r.z;
    }

    // bitwise not with lazy evaluation
    template <typename T>
    struct bitwise_not<basic_vec3<T>> {
        using tp = basic_vec3<T>;
        tp neg;
        SIMDIFY_FORCE_INLINE explicit bitwise_not(const tp& r) : neg(r) {}
        SIMDIFY_FORCE_INLINE explicit operator const tp() const { return{ ~neg.x, ~neg.y, ~neg.z }; }
    };

    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator&(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x & r.x, l.y & r.y, l.z & r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator|(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x | r.x, l.y | r.y, l.z | r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator^(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x ^ r.x, l.y ^ r.y, l.z ^ r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const bitwise_not<basic_vec3<T>> operator~(const basic_vec3<T>& l) { return bitwise_not<basic_vec3<T>>(l); }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator<(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x < r.x, l.y < r.y, l.z < r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator>(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x > r.x, l.y > r.y, l.z > r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator<=(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x <= r.x, l.y <= r.y, l.z <= r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator>=(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x >= r.x, l.y >= r.y, l.z >= r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator==(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x == r.x, l.y == r.y, l.z == r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator!=(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x != r.x, l.y != r.y, l.z != r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator+(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x + r.x, l.y + r.y, l.z + r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator-(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x - r.x, l.y - r.y, l.z - r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator*(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x * r.x, l.y * r.y, l.z * r.z }; }
    template <typename T> SIMDIFY_FORCE_INLINE const basic_vec3<T> operator/(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x / r.x, l.y / r.y, l.z / r.z }; }

    template <typename T>
    SIMDIFY_FORCE_INLINE const basic_vec3<T> andnot(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return{ andnot(l.x, r.x), andnot(l.y, r.y), andnot(l.z, r.z) };
    }

    template <typename T>
    SIMDIFY_FORCE_INLINE const basic_vec3<T> min(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return{ min(l.x, r.x), min(l.y, r.y), min(l.z, r.z) };
    }

    template <typename T>
    SIMDIFY_FORCE_INLINE const basic_vec3<T> max(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return{ max(l.x, r.x), max(l.y, r.y), max(l.z, r.z) };
    }

    template <typename T>
    SIMDIFY_FORCE_INLINE const basic_vec3<T> cond(const basic_vec3<T>& p, const basic_vec3<T>& t, const basic_vec3<T>& f) {
        return{ cond(p.x, t.x, f.x), cond(p.y, t.y, f.y), cond(p.z, t.z, f.z) };
    }
}

#endif // SIMDIFY_VEC3_GENERAL
