# `sse` (type family)

Defined in header `<simdee/simd_types/sse.hpp>`

`sse` is an architecture-specific type family that employs the SSE2 instruction set and its extensions. If you use `sse` type family in your code, SSE2 support must be enabled. See guide on how to [enable instruction sets].

Avoid coding against architecture-specific types; prefer [architecture-agnostic types](vec4.md) instead.

type       | `width` | `scalar_t`      | satisfies concepts
-----------|---------|-----------------|----------------------------------------------------------------
`sd::sseb` | 4       | `sd::bool32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorB`](SIMDVectorB.md)
`sd::ssef` | 4       | `float`         | [`SIMDVector`](SIMDVector.md), [`SIMDVectorF`](SIMDVectorF.md)
`sd::sseu` | 4       | `std::uint32_t` | [`SIMDVector`](SIMDVector.md), [`SIMDVectorU`](SIMDVectorU.md)
`sd::sses` | 4       | `std::int32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorS`](SIMDVectorS.md)
