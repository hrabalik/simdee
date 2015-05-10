#ifndef SIMDIFY_VEC3
#define SIMDIFY_VEC3

#include "simdify.hpp"
#include "util/inline.hpp"

#define INL SIMDIFY_FORCE_INLINE

namespace simd {

    template <typename T>
    struct basic_vec3 {
        using mm_t = typename T::mm_t;
        using fp_t = typename T::fp_t;
        using bitmask_t = typename T::bitmask_t;

        INL basic_vec3() = default;
        INL basic_vec3(const basic_vec3& other) = default;
        INL basic_vec3& operator=(const basic_vec3& other) = default;
        
        INL basic_vec3(const T& tx, const T& ty, const T& tz) :
            x(tx), y(ty), z(tz) {}

        template <typename S>
        INL basic_vec3(const S& other) : x(other.x), y(other.y), z(other.z) {}

        template <typename S>
        INL basic_vec3& operator=(const S& other) {
            x = T(other.x); y = T(other.y); z = T(other.z);
            return *this;
        }

        T x, y, z;
    };

    template <typename T>
    INL const basic_vec3<T> cross(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return{ l.y*r.z - l.z*r.y, l.z*r.x - l.x*r.z, l.x*r.y - l.y*r.x };
    }

    template <typename T>
    INL const T dot(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return l.x*r.x + l.y*r.y + l.z*r.z;
    }

    // bitwise not with lazy evaluation
    template <typename T>
    struct bitwise_not<basic_vec3<T>> {
        using tp = basic_vec3<T>;
        tp neg;
        INL explicit bitwise_not(const tp& r) : neg(r) {}
        INL operator const tp() const { return{ ~neg.x, ~neg.y, ~neg.z }; }
    };

    template <typename T>
    INL const basic_vec3<T> operator&(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x & r.x, l.y & r.y, l.z & r.z }; }
    INL const basic_vec3<T> operator|(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x | r.x, l.y | r.y, l.z | r.z }; }
    INL const basic_vec3<T> operator^(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x ^ r.x, l.y ^ r.y, l.z ^ r.z }; }
    INL const bitwise_not<basic_vec3<T>> operator~(const basic_vec3<T>& l) { return bitwise_not<basic_vec3<T>>(l); }
    INL const basic_vec3<T> operator<(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x < r.x, l.y < r.y, l.z < r.z }; }
    INL const basic_vec3<T> operator>(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x > r.x, l.y > r.y, l.z > r.z }; }
    INL const basic_vec3<T> operator<=(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x <= r.x, l.y <= r.y, l.z <= r.z }; }
    INL const basic_vec3<T> operator>=(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x >= r.x, l.y >= r.y, l.z >= r.z }; }
    INL const basic_vec3<T> operator==(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x == r.x, l.y == r.y, l.z == r.z }; }
    INL const basic_vec3<T> operator!=(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x != r.x, l.y != r.y, l.z != r.z }; }
    INL const basic_vec3<T> operator+(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x + r.x, l.y + r.y, l.z + r.z }; }
    INL const basic_vec3<T> operator-(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x - r.x, l.y - r.y, l.z - r.z }; }
    INL const basic_vec3<T> operator*(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x * r.x, l.y * r.y, l.z * r.z }; }
    INL const basic_vec3<T> operator/(const basic_vec3<T>& l, const basic_vec3<T>& r) { return{ l.x / r.x, l.y / r.y, l.z / r.z }; }

    INL const basic_vec3<T> min(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return{ min(l.x, r.x), min(l.y, r.y), min(l.z, r.z) };
    }

    INL const basic_vec3<T> max(const basic_vec3<T>& l, const basic_vec3<T>& r) {
        return{ max(l.x, r.x), max(l.y, r.y), max(l.z, r.z) };
    }

    using vec3 = basic_vec3<simd_t>;
}

#undef INL

#endif // SIMDIFY_VEC3