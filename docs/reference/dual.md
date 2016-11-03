# `dual<T>`

Defined in header `<simdee/simd_types/dual.hpp>`

`dual` is a class template that creates a new vector based on an existing [`SIMDVector`](SIMDVector.md) `T`.

* The width of the new vector is doubled, i.e. `dual<T>::width == 2 * T::width`.
* `dual<T>` satisfies [`SIMDVector`](SIMDVector.md).
* If `T` satisfies one of the concepts [`SIMDVectorB`](SIMDVectorB.md), [`SIMDVectorF`](SIMDVectorF.md), [`SIMDVectorU`](SIMDVectorU.md), [`SIMDVectorS`](SIMDVectorS.md), then `dual<T>` satisfies it as well.

## Performance considerations

* The implementation of `reduce(x)` is not effective for repeatedly nested types, e.g. when `x` has type `dual<dual<dumf>>`.
* Expression templates `deferred_bitnot` and `deferred_lognot` are not employed by `dual<T>` even if `T` uses them.
