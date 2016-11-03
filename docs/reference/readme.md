# Simdee reference

* Concepts
  * [`SIMDVector`](SIMDVector.md) SIMD vectors
    * [`SIMDVectorB`](SIMDVectorB.md) boolean SIMD vector
    * [`SIMDVectorF`](SIMDVectorF.md) 32-bit floating-point SIMD vector
    * [`SIMDVectorU`](SIMDVectorU.md) 32-bit unsigned integral SIMD vector
    * [`SIMDVectorS`](SIMDVectorS.md) 32-bit signed integral SIMD vector
* Vectors
  * Architecture-agnostic
    * [`sd::dum_`](dum.md) vectors with 1 scalar
    * [`sd::vec4_`](vec4.md) vectors with 4 scalars
    * [`sd::vec8_`](vec8.md) vectors with 8 scalars
  * Architecture-specific
    * [`sd::sse_`](sse.md) vectors that employ SSE
    * [`sd::avx_`](avx.md) vectors that employ AVX
  * [`sd::dual<T>`](dual.md) vector composition
