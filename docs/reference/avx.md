# `avx` (type family)

Defined in header `<simdee/simd_types/avx.hpp>`

`avx` is an architecture-specific type family that employs the AVX instruction set and its extensions. If you use `avx` type family in your code, AVX support must be enabled. Unless the macro `SIMDEE_NEED_INT` is set to `0`, AVX2 support must be enabled. See guide on how to [enable instruction sets].

`SIMDEE_NEED_INT` | required instruction set
------------------|--------------------------
`0`               | AVX
`1` (default)     | AVX2

Avoid coding against architecture-specific types; prefer [architecture-agnostic types](vec8.md) instead.

type       | `width` | `scalar_t`      | satisfies concepts
-----------|---------|-----------------|----------------------------------------------------------------
`sd::avxb` | 8       | `sd::bool32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorB`](SIMDVectorB.md)
`sd::avxf` | 8       | `float`         | [`SIMDVector`](SIMDVector.md), [`SIMDVectorF`](SIMDVectorF.md)
`sd::avxu` | 8       | `std::uint32_t` | [`SIMDVector`](SIMDVector.md), [`SIMDVectorU`](SIMDVectorU.md)
`sd::avxs` | 8       | `std::int32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorS`](SIMDVectorS.md)
