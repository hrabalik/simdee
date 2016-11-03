# `SIMDVectorB` (concept)

Describes all SIMD vector types provided by Simdee that have a boolean underlying scalar type.

## Implementations

* `sd::sseb`, `sd::avxb`, `sd::dumb`
* `sd::dual<T>`, where `T` satisfies `SIMDVectorB`
* `sd::vec4b`, `sd::vec8b`

## Requirements

Type that satisfies `SIMDVectorB` must also satisfy `SIMDVector`.

### Member types

Additional requirements apply regarding member types of a type `T` that satisfies `SIMDVectorB`:

* `vec_b` is `T`
* `scalar_t` is `sd::bool32_t`

### Operations

Additional operations must be provided for a type `T` that satisfies `SIMDVectorB`:

syntax             | description
-------------------|-----------------------------------------------------------------------------
`mask(x)`          | provide a bit-mask of truth values, returns an instantiation of `sd::impl::mask`
`!x`               | element-wise logical not
`x && y`           | element-wise logical and, short-circuit evaluation is disabled
`x || y`           | element-wise logical or, short-circuit evaluation is disabled
`andnot(x, y)`     | same as `x && !y`

where `x`, `y` are values of type `T`
