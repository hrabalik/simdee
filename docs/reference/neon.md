# `sd::neon_` (type family)

Defined in header `<simdee/simd_vectors/neon.hpp>`

`sd::neon_` is an architecture-specific type family that employs the NEON instruction set. If you use `sd::neon_` type family in your code, NEON support must be enabled. See guide on how to [enable instruction sets](../guides/config.md).

Avoid coding against architecture-specific types; prefer [architecture-independent types](vec4.md) instead.

type        | `width` | `scalar_t`      | satisfies concepts
------------|---------|-----------------|----------------------------------------------------------------
`sd::neonb` | 4       | `sd::bool32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorB`](SIMDVectorB.md)
`sd::neonf` | 4       | `float`         | [`SIMDVector`](SIMDVector.md), [`SIMDVectorF`](SIMDVectorF.md)
`sd::neonu` | 4       | `std::uint32_t` | [`SIMDVector`](SIMDVector.md), [`SIMDVectorU`](SIMDVectorU.md)
`sd::neons` | 4       | `std::int32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorS`](SIMDVectorS.md)
