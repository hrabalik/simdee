#ifndef SIMDIFY_VECTOR
#define SIMDIFY_VECTOR

#include "simdify.hpp"
#include "util/malloc.hpp"
#include <vector>

namespace simd {

    template <typename T>
    inline T ceildiv(T a, T b) { return (a + b - 1) / b; }

    //
    // Extension of std::vector enabling .at<float>(...), etc.
    //
    template <typename T>
    class basic_vector : public std::vector<T, aligned_allocator<T, sizeof T>> {
    public:
        // base class
        using base_t = std::vector<T, aligned_allocator<T, sizeof T>>;

        // fake base class type that is an alias of the actual base class with an allocator that throws when used
        // (or the actual base class type, if S == T)
        template <typename S>
        using base_cast_t = typename std::conditional<std::is_same<S, T>::value, base_t, std::vector<S, dummy_allocator<S>>>::type;

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

        // the methods below would be nice, but they don't work under MSVC due to the mysterious error C1001

        //template <typename S = T> typename base_cast_t<S>::iterator begin() { return base_cast<S>()->begin(); }
        //template <typename S = T> typename base_cast_t<S>::const_iterator begin() const { return base_cast<S>()->begin(); }
        //template <typename S = T> typename base_cast_t<S>::const_iterator cbegin() const { return base_cast<S>()->cbegin(); }
        //template <typename S = T> typename base_cast_t<S>::iterator end() { return base_cast<S>()->end(); }
        //template <typename S = T> typename base_cast_t<S>::const_iterator end() const { return base_cast<S>()->end(); }
        //template <typename S = T> typename base_cast_t<S>::const_iterator cend() const { return base_cast<S>()->cend(); }
        //template <typename S = T> typename base_cast_t<S>::reverse_iterator rbegin() { return base_cast<S>()->rbegin(); }
        //template <typename S = T> typename base_cast_t<S>::const_reverse_iterator rbegin() const { return base_cast<S>()->rbegin(); }
        //template <typename S = T> typename base_cast_t<S>::const_reverse_iterator crbegin() const { return base_cast<S>()->crbegin(); }
        //template <typename S = T> typename base_cast_t<S>::reverse_iterator rend() { return base_cast<S>()->rend(); }
        //template <typename S = T> typename base_cast_t<S>::const_reverse_iterator rend() const { return base_cast<S>()->rend(); }
        //template <typename S = T> typename base_cast_t<S>::const_reverse_iterator crend() const { return base_cast<S>()->crend(); }

        template <typename S = T> size_type size() const { return base_cast<S>()->size(); }
        template <typename S = T> size_type capacity() const { return base_cast<S>()->capacity(); }
        template <typename S = T> void reserve(size_type i) { base_t::reserve(ceildiv(i, sizeof(T) / sizeof(S))); }
        template <typename S = T> void resize(size_type i) { base_t::resize(ceildiv(i, sizeof(T) / sizeof(S))); }
        template <typename S = T> void resize(size_type i, const value_type& v) { base_t::resize(ceildiv(i, sizeof(T) / sizeof(S)), v); }
    };

    using vector = basic_vector<simd_t>;
};

#endif