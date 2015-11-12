#ifndef SIMDIFY_STRUCTURE_OF_ARRAYS
#define SIMDIFY_STRUCTURE_OF_ARRAYS

#include "../simdify.hpp"
#include "../containers.hpp"
#include "../util/inline.hpp"
#include "named_array.hpp"

namespace simd {
    template <typename Simd_t, id... Ids>
    struct structure_of_arrays;

    template <typename Mm_t, typename F_t, typename Simd_t, id...Ids>
    struct structure_of_arrays<simd_base<Mm_t, F_t, Simd_t>, Ids...> {
        static constexpr std::size_t N = sizeof...(Ids);
        using simd_t = Simd_t;
        using f_t = F_t;
        using mm_t = Mm_t;

        template <typename T>
        struct iterator_base : named_array<T, Ids...> {};

        struct iterator : iterator_base<simd_t> {};

        struct const_iterator : iterator_base<const simd_t> {};

        template <typename T>
        struct tail_iterator_base : named_array<T, Ids...> {};

        struct tail_iterator : tail_iterator_base<f_t&> {};

        struct const_tail_iterator : tail_iterator_base<const f_t&> {};

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
        iterator bodybegin();
        iterator bodyend();
        const_iterator bodybegin() const;
        const_iterator bodyend() const;
        const_iterator bodycbegin() const;
        const_iterator bodycend() const;
        tail_iterator tailbegin();
        tail_iterator tailend();
        const_tail_iterator tailbegin() const;
        const_tail_iterator tailend() const;
        const_tail_iterator tailcbegin() const;
        const_tail_iterator tailcend() const;
    };
}

#endif // SIMDIFY_STRUCTURE_OF_ARRAYS
