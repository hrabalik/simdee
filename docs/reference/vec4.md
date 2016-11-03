# `vec4` (type family)

Defined in header `<simdee/vec4.hpp>`

`vec4` is an architecture-agnostic type family of vectors that contain 4 scalars.

type    | `width` | `scalar_t`      | satisfies concepts
--------|---------|-----------------|----------------------------------------------------------------
`vec4b` | 4       | `sd::bool32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorB`](SIMDVectorB.md)
`vec4f` | 4       | `float`         | [`SIMDVector`](SIMDVector.md), [`SIMDVectorF`](SIMDVectorF.md)
`vec4u` | 4       | `std::uint32_t` | [`SIMDVector`](SIMDVector.md), [`SIMDVectorU`](SIMDVectorU.md)
`vec4s` | 4       | `std::int32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorS`](SIMDVectorS.md)

The `vec4` family is an alias for another type family, based on supported instruction sets:

                      | `vec4b`                       | `vec4f`                       | `vec4u`                       | `vec4s`   
----------------------|-------------------------------|-------------------------------|-------------------------------|--------------------------------
if `sse` is available | [`sseb`](sse.md)              | [`ssef`](sse.md)              | [`sseu`](sse.md)              | [`sses`](sse.md)
otherwise             | [`dual<dual<dumb>>`](dual.md) | [`dual<dual<dumf>>`](dual.md) | [`dual<dual<dumu>>`](dual.md) | [`dual<dual<dums>>`](dual.md)
