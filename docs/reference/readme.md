# Simdee reference

* Concepts
  * [`SIMDVector`](SIMDVector.md) SIMD vectors
    * [`SIMDVectorB`](SIMDVectorB.md) boolean SIMD vector
    * [`SIMDVectorF`](SIMDVectorF.md) 32-bit floating-point SIMD vector
    * [`SIMDVectorU`](SIMDVectorU.md) 32-bit unsigned integral SIMD vector
    * [`SIMDVectorS`](SIMDVectorS.md) 32-bit signed integral SIMD vector
* Vectors
  * Architecture-agnostic
    * [`dum`](dum.md) vectors with 1 scalar
    * [`vec4`](vec4.md) vectors with 4 scalars
    * [`vec8`](vec8.md) vectors with 8 scalars
  * Architecture-specific
    * [`sse`](sse.md) vectors that employ SSE
    * [`avx`](avx.md) vectors that employ AVX
  * [`dual<T>`](dual.md) vector composition
