# `sd::dum_` (type family)

Defined in header `<simdee/simd_types/dum.hpp>`

`sd::dum_` is a type family of vectors containing only a single scalar.

type       | `width` | `scalar_t`      | satisfies concepts
-----------|---------|-----------------|----------------------------------------------------------------
`sd::dumb` | 1       | `sd::bool32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorB`](SIMDVectorB.md)
`sd::dumf` | 1       | `float`         | [`SIMDVector`](SIMDVector.md), [`SIMDVectorF`](SIMDVectorF.md)
`sd::dumu` | 1       | `std::uint32_t` | [`SIMDVector`](SIMDVector.md), [`SIMDVectorU`](SIMDVectorU.md)
`sd::dums` | 1       | `std::int32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorS`](SIMDVectorS.md)
