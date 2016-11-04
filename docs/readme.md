# Simdee documentation

## Guides

* [Compiler and library configuration](guides/config.md)

## Reference

* Concepts
  * [`SIMDVector`](reference/SIMDVector.md) SIMD vectors
    * [`SIMDVectorB`](reference/SIMDVectorB.md) boolean SIMD vector
    * [`SIMDVectorF`](reference/SIMDVectorF.md) 32-bit floating-point SIMD vector
    * [`SIMDVectorU`](reference/SIMDVectorU.md) 32-bit unsigned integral SIMD vector
    * [`SIMDVectorS`](reference/SIMDVectorS.md) 32-bit signed integral SIMD vector
* Vectors
  * Architecture-independent
    * [`sd::dum_`](reference/dum.md) vectors with 1 scalar
    * [`sd::vec4_`](reference/vec4.md) vectors with 4 scalars
    * [`sd::vec8_`](reference/vec8.md) vectors with 8 scalars
  * Architecture-specific
    * [`sd::sse_`](reference/sse.md) vectors that employ SSE
    * [`sd::avx_`](reference/avx.md) vectors that employ AVX
  * [`sd::dual<T>`](reference/dual.md) vector composition
