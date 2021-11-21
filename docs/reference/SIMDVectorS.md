# `SIMDVectorS` (concept)

Describes all SIMD vector types provided by Simdee that have a 32-bit signed integral underlying scalar type.

## Implementations

* [`sd::sses`](sse.md), [`sd::avxs`](avx.md), [`sd::neons`](neon.md), [`sd::dums`](dum.md)
* [`sd::dual<T>`](dual.md), where `T` satisfies `SIMDVectorS`
* [`sd::vec4s`](vec4.md), [`sd::vec8s`](vec8.md)

## Requirements

Type that satisfies `SIMDVectorS` must also satisfy [`SIMDVector`](SIMDVector.md).

### Member types

Additional requirements apply regarding member types of a type `T` that satisfies `SIMDVectorS`:

* `vec_s` is `T`
* `scalar_t` is `std::int32_t`

### Conversions

A type that satisfies `SIMDVectorS` must be:
* explicitly convertible to and from [`vec_f`](SIMDVectorF.md).
* explicitly convertible to and from [`vec_u`](SIMDVectorU.md).

### Operations

Additional operations must be provided for a type `T` that satisfies `SIMDVectorS`, unless the macro `SIMDEE_NEED_INT` is set to `0`:

syntax         | result type               | description                                           | notes
---------------|---------------------------|-------------------------------------------------------|-------
`x == y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise equal                                     |
`x != y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise not equal                                 |
`x > y`        | [`vec_b`](SIMDVectorB.md) | scalar-wise greater than                              |
`x < y`        | [`vec_b`](SIMDVectorB.md) | scalar-wise less than                                 |
`x >= y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise greater than or equal                     |
`x <= y`       | [`vec_b`](SIMDVectorB.md) | scalar-wise less than or equal                        |
`~x`           | `T`                       | bit-wise not                                          |
`x & y`        | `T`                       | bit-wise and                                          |
`x \| y`       | `T`                       | bit-wise or                                           |
`x ^ y`        | `T`                       | bit-wise xor                                          |
`x &= y`       | `T`                       | same as `x = x & y`                                   |
`x \|= y`      | `T`                       | same as `x = x \| y`                                  |
`x ^= y`       | `T`                       | same as `x = x ^ y`                                   |
`+x`           | `T`                       | scalar-wise unary plus                                |
`-x`           | `T`                       | scalar-wise unary minus                               |
`x + y`        | `T`                       | scalar-wise addition                                  |
`x - y`        | `T`                       | scalar-wise subtraction                               |
`x * y`        | `T`                       | scalar-wise multiplication                            | [1]
`x += y`       | `T`                       | same as `x = x + y`                                   |
`x -= y`       | `T`                       | same as `x = x - y`                                   |
`x *= y`       | `T`                       | same as `x = x * y`                                   | [1]
`min(x)`       | `T`                       | scalar-wise minimum                                   |
`max(x)`       | `T`                       | scalar-wise maximum                                   |
`abs(x)`       | `T`                       | scalar-wise absolute value                            |

where `x`, `y` are values of type `T`.

[1] Multiplication operations are inefficient on SSE2. Requiring SSE4.1 or AVX ensures efficient implementation.
