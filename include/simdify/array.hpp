#ifndef SIMDIFY_ARRAY
#define SIMDIFY_ARRAY

#include "simdify.hpp"
#include <array>

namespace simd {

    //
    // Extension of std::array enabling .at<float>(...), etc.
    //
    template <typename T, std::size_t SZ>
    class basic_array : public std::array<T, SZ> {
    public:
        // base class
        using base_t = std::array<T, SZ>;
        using size_type = typename std::array<T, SZ>::size_type;

        // fake base class type that aliases this class (unless S == T, then the actual base class type is returned)
        template <typename S>
        //using base_cast_t = typename std::conditional<std::is_same<S, T>::value, base_t, std::array<S, (sizeof(T) / sizeof(S)) * SZ>>::type;
        using base_cast_t = std::array < S, (sizeof(T) / sizeof(S)) * SZ > ;

        using mm_t = typename T::mm_t;
        using fp_t = typename T::fp_t;
        using bitmask_t = typename T::bitmask_t;

        // determines whether base_cast<S> is valid for a given S
        template <typename S>
        struct cast_allowed : std::integral_constant<bool,
            std::is_same<S, T>::value
            || std::is_same<S, mm_t>::value
            || std::is_same<S, fp_t>::value
            || std::is_same<S, bitmask_t>::value> {};

        // cast this to the fake base
        template <typename S, typename = typename std::enable_if<cast_allowed<S>::value, void>::type>
        base_cast_t<S>* base_cast() {
            return reinterpret_cast<base_cast_t<S>*>(this);
        }
        template <typename S, typename = typename std::enable_if<cast_allowed<S>::value, void>::type>
        const base_cast_t<S>* base_cast() const {
            return reinterpret_cast<const base_cast_t<S>*>(this);
        }

        template <typename S = T> S& at(size_type i) { return base_cast<S>()->at(i); }
        template <typename S = T> const S& at(size_type i) const { return base_cast<S>()->at(i); }
        template <typename S = T> S& front() { return base_cast<S>()->front(); }
        template <typename S = T> const S& front() const { return base_cast<S>()->front(); }
        template <typename S = T> S& back() { return base_cast<S>()->back(); }
        template <typename S = T> const S& back() const { return base_cast<S>()->back(); }
        template <typename S = T> S* data() { return base_cast<S>()->data(); }
        template <typename S = T> const S* data() const { return base_cast<S>()->data(); }

        template <typename S = T> typename base_cast_t<S>::iterator begin() { return base_cast<S>()->begin(); }
        template <typename S = T> typename base_cast_t<S>::const_iterator begin() const { return base_cast<S>()->begin(); }
        template <typename S = T> typename base_cast_t<S>::const_iterator cbegin() const { return base_cast<S>()->cbegin(); }
        template <typename S = T> typename base_cast_t<S>::iterator end() { return base_cast<S>()->end(); }
        template <typename S = T> typename base_cast_t<S>::const_iterator end() const { return base_cast<S>()->end(); }
        template <typename S = T> typename base_cast_t<S>::const_iterator cend() const { return base_cast<S>()->cend(); }
        template <typename S = T> typename base_cast_t<S>::reverse_iterator rbegin() { return base_cast<S>()->rbegin(); }
        template <typename S = T> typename base_cast_t<S>::const_reverse_iterator rbegin() const { return base_cast<S>()->rbegin(); }
        template <typename S = T> typename base_cast_t<S>::const_reverse_iterator crbegin() const { return base_cast<S>()->crbegin(); }
        template <typename S = T> typename base_cast_t<S>::reverse_iterator rend() { return base_cast<S>()->rend(); }
        template <typename S = T> typename base_cast_t<S>::const_reverse_iterator rend() const { return base_cast<S>()->rend(); }
        template <typename S = T> typename base_cast_t<S>::const_reverse_iterator crend() const { return base_cast<S>()->crend(); }

        template <typename S = T> size_type size() const { return base_cast<S>()->size(); }
        template <typename S = T> size_type capacity() const { return base_cast<S>()->capacity(); }
        template <typename S = T> void fill(const S& val) { base_cast<S>()->fill(val); }
    };

    template <std::size_t SZ>
    using array = basic_array<simd_t, SZ>;
};

#endif // SIMDIFY_ARRAY
