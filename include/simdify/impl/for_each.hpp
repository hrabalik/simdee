#ifndef SIMDIFY_FOR_EACH
#define SIMDIFY_FOR_EACH

#include "../util/inline.hpp"
#include <type_traits>

namespace simd {

    namespace detail {

        template <std::size_t I>
        struct ForEach {
            template <typename Func>
            SIMDIFY_FORCE_INLINE static constexpr void perform(Func& f) {
                ForEach<I - 1>::perform(f);
                f.template perform<I - 1>();
            }
        };

        template <>
        struct ForEach<0> {
            template <typename Func>
            SIMDIFY_FORCE_INLINE static constexpr void perform(Func&) {}
        };

    }

}

#endif // SIMDIFY_FOR_EACH
