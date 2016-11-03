# `avx` (type family)

Defined in header `<simdee/simd_types/avx.hpp>`

`avx` is an architecture-specific type family that employs the AVX instruction set and its extensions. If you use `avx` type family in your code, AVX support must be enabled. See guide on how to [enable instruction sets].

Avoid coding against architecture-specific types; prefer [architecture-agnostic types] instead.

type   | `width` | `scalar_t`      | satisfies concepts
-------|---------|-----------------|----------------------------------------------------------------
`avxb` | 8       | `sd::bool32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorB`](SIMDVectorB.md)
`avxf` | 8       | `float`         | [`SIMDVector`](SIMDVector.md), [`SIMDVectorF`](SIMDVectorF.md)
`avxu` | 8       | `std::uint32_t` | [`SIMDVector`](SIMDVector.md), [`SIMDVectorU`](SIMDVectorU.md)
`avxs` | 8       | `std::int32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorS`](SIMDVectorS.md)
