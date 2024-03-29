# `SIMDVectorF` (concept)

Describes all SIMD vector types provided by Simdee that have a 32-bit floating-point underlying scalar type.

## Implementations

* [`sd::ssef`](sse.md), [`sd::avxf`](avx.md), `sd::dumf`](dum.md)
* [`sd::dual<T>`](dual.md), where `T` satisfies `SIMDVectorF`
* [`sd::vec4f`](vec4.md), [`sd::vec8f`](vec8.md)

## Requirements

Type that satisfies `SIMDVectorF` must also satisfy [`SIMDVector`](SIMDVector.md).

### Member types

Additional requirements apply regarding member types of a type `T` that satisfies `SIMDVectorF`:

* `vec_f` is `T`
* `scalar_t` is `float`

### Conversions

A type that satisfies `SIMDVectorF` must be explicitly convertible to and from [`vec_s`](SIMDVectorS.md).

### Operations

Additional operations must be provided for a type `T` that satisfies `SIMDVectorF`:

syntax         | result type               | description                                           | notes
---------------|---------------------------|-------------------------------------------------------|-------
`x == y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise equal                                     |
`x != y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise not equal                                 |
`x > y`        | [`vec_b`](SIMDVectorB.md) | scalar-wise greater than                              |
`x < y`        | [`vec_b`](SIMDVectorB.md) | scalar-wise less than                                 |
`x >= y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise greater than or equal                     |
`x <= y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise less than or equal                        |
`+x`           | `T`                       | scalar-wise unary plus                                |
`-x`           | `T`                       | scalar-wise unary minus                               |
`x + y`        | `T`                       | scalar-wise addition                                  |
`x - y`        | `T`                       | scalar-wise subtraction                               |
`x * y`        | `T`                       | scalar-wise multiplication                            |
`x / y`        | `T`                       | scalar-wise division                                  | [1]
`x += y`       | `T`                       | same as `x = x + y`                                   |
`x -= y`       | `T`                       | same as `x = x - y`                                   |
`x *= y`       | `T`                       | same as `x = x * y`                                   |
`x /= y`       | `T`                       | same as `x = x / y`                                   |
`min(x)`       | `T`                       | scalar-wise minimum                                   |
`max(x)`       | `T`                       | scalar-wise maximum                                   |
`abs(x)`       | `T`                       | scalar-wise absolute value                            |
`sqrt(x)`      | `T`                       | scalar-wise square root                               | [1]
`rsqrt(x)`     | `T`                       | scalar-wise fast reciprocal square root               | [2]
`rcp(x)`       | `T`                       | scalar-wise fast reciprocal                           | [2]

where `x`, `y` are values of type `T`.

[1] Division and `sqrt` is inefficient on 32-bit ARM.

[2] Result of `rsqrt` and `rcp` is not consistent across CPU architectures. Maximum relative error for both operations is `1.5*2^-12`.
