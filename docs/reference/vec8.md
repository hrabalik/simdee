# `vec8` (type family)

Defined in header `<simdee/vec8.hpp>`

`vec8` is an architecture-agnostic type family of vectors that contain 8 scalars.

type    | `width` | `scalar_t`      | satisfies concepts
--------|---------|-----------------|----------------------------------------------------------------
`vec8b` | 8       | `sd::bool32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorB`](SIMDVectorB.md)
`vec8f` | 8       | `float`         | [`SIMDVector`](SIMDVector.md), [`SIMDVectorF`](SIMDVectorF.md)
`vec8u` | 8       | `std::uint32_t` | [`SIMDVector`](SIMDVector.md), [`SIMDVectorU`](SIMDVectorU.md)
`vec8s` | 8       | `std::int32_t`  | [`SIMDVector`](SIMDVector.md), [`SIMDVectorS`](SIMDVectorS.md)

The `vec8` family is an alias for another type family, based on supported instruction sets:

                                | `vec8b`                       | `vec8f`                       | `vec8u`                       | `vec8s`   
--------------------------------|-------------------------------|-------------------------------|-------------------------------|--------------------------------
if [`avx`](avx.md) is supported | [`avxb`](avx.md)              | [`avxf`](avx.md)              | [`avxu`](avx.md)              | [`avxs`](avx.md)
otherwise                       | [`dual<vec4f>`](dual.md)      | [`dual<vec4f>`](dual.md)      | [`dual<vec4f>`](dual.md)      | [`dual<vec4f>`](dual.md)
