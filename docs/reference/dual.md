# `sd::dual<T>`

Defined in header `<simdee/simd_vectors/dual.hpp>`

`sd::dual` is a class template that creates a new vector based on an existing type `T` that satisfies [`SIMDVector`](SIMDVector.md).

* The width of the new vector is doubled, i.e. `sd::dual<T>::width == 2 * T::width`.
* `sd::dual<T>` satisfies [`SIMDVector`](SIMDVector.md).
* If `T` satisfies one of the concepts [`SIMDVectorB`](SIMDVectorB.md), [`SIMDVectorF`](SIMDVectorF.md), [`SIMDVectorU`](SIMDVectorU.md), [`SIMDVectorS`](SIMDVectorS.md), then `sd::dual<T>` satisfies it as well.

## Performance considerations

* The implementation of `reduce(x)` is not effective for repeatedly nested types, e.g. when `x` has type `sd::dual<sd::dual<sd::dumf>>`.
* Expression templates `deferred_bitnot` and `deferred_lognot` are not employed by `sd::dual<T>` even if `T` uses them.
