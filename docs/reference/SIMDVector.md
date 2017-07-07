# `SIMDVector` (concept)

Describes all SIMD vector types provided by Simdee, regardless of underlying datatype. A particular vector type always satisfies one of the extended concepts [`SIMDVectorB`](SIMDVectorB.md), [`SIMDVectorF`](SIMDVectorF.md), [`SIMDVectorU`](SIMDVectorU.md), [`SIMDVectorS`](SIMDVectorS.md).

## Implementations

* [`sd::sseb`](sse.md), [`sd::ssef`](sse.md), [`sd::sseu`](sse.md), [`sd::sses`](sse.md)
* [`sd::avxb`](avx.md), [`sd::avxf`](avx.md), [`sd::avxu`](avx.md), [`sd::avxs`](avx.md)
* [`sd::dumb`](dum.md), [`sd::dumf`](dum.md), [`sd::dumu`](dum.md), [`sd::dums`](dum.md)
* [`sd::dual<T>`](dual.md)
* [`sd::vec4b`](vec4.md), [`sd::vec4f`](vec4.md), [`sd::vec4u`](vec4.md), [`sd::vec4s`](vec4.md)
* [`sd::vec8b`](vec8.md), [`sd::vec8f`](vec8.md), [`sd::vec8u`](vec8.md), [`sd::vec8s`](vec8.md)

## Requirements

### Member values

Type that satisfies `SIMDVector` must provide the following member values:

value   | description
--------|---------------------------------
`width` | number of scalars in the vector

### Member types

Type that satisfies `SIMDVector` must provide the following member types:

type           | description                                  | additional requirements
---------------|----------------------------------------------|-------------------------
`vec_b`        | related boolean SIMD vector                  | satisfies [`SIMDVectorB`](SIMDVectorB.md)
`vec_f`        | related floating-point SIMD vector           | satisfies [`SIMDVectorF`](SIMDVectorF.md)
`vec_u`        | related unsigned integral SIMD vector        | satisfies [`SIMDVectorU`](SIMDVectorU.md)
`vec_s`        | related signed integral SIMD vector          | satisfies [`SIMDVectorS`](SIMDVectorS.md)
`vector_t`     | low-level underlying vector type             |
`scalar_t`     | scalar type                                  |
`storage_t`    | type for storing vector in memory            |

### Construction and assignment

Type `T` that satisfies `SIMDVector` must be implicitly constructible and assignable from:
* `T`
* `vector_t`, `scalar_t`, `storage_t`
* result of `sd::zero()`, `sd::all_bits()`, `sd::sign_bit()`, `sd::abs_mask()`
* result of `sd::aligned()`, `sd::unaligned()`, `sd::interleaved()`

Additionally, `T` must be assignable to:
* `storage_t`
* result of `sd::aligned()`, `sd::unaligned()`, `sd::interleaved()`

### Operations

For a type `T` that satisfies `SIMDVector`, the following operations must be provided:

syntax                           | description
---------------------------------|-----------------------------------------------------------------------------
`x.eval()`                       | no-op, used to enforce evaluation of expression templates
`x.data()`                       | provides access to the underlying low-level register value
`x.aligned_load(ptr)`            | load vector from a memory location aligned to `alignof(T)` bytes
`x.aligned_store(ptr)`           | store vector to a memory location aligned to `alignof(T)` bytes
`x.unaligned_load(ptr)`          | load vector from an arbitrary memory location
`x.unaligned_store(ptr)`         | store vector to an arbitrary memory location
`x.interleaved_load(ptr, step)`  | load vector, `i`-th scalar is loaded from `ptr + i*step`
`x.interleaved_store(ptr, step)` | store vector, `i`-th scalar is stored `ptr + i*step`
`cond(b, x, y)`                  | based on values in `b`, select scalars from `x` (if true) or `y` (if false)
`first_scalar(x)`                | retrieve the value of the first scalar in vector
`reduce(x, f)`                   | apply reduction `f` to `x`, storing the result in each scalar

where:
* `x`, `y` are values of type `T`
* `ptr` is a value of type `scalar_t*`
* `step` is a value of type `int`
* `b` is a value of type `vec_b`
* `f` is a function template or function object with a templated `operator()` that has a signature equivalent to `S(const S&, const S&)`, where `S` is a template parameter.

