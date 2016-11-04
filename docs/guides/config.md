# Compiler and library configuration

## Compiler configuration

### Check compiler version

When coding with Simdee, be sure to use a recent version of your C++ toolchain. These are the supported compilers:

* Clang, currently tested under 3.8.0
* GCC, currently tested under 5.4.0
* MSVC, currrently tested under 14.0 (Visual Studio 2015)

### Add Simdee to include path

Since Simdee is a header-only library, all you need to do to start using Simdee is to add its headers to your include path. You can find all the headers in the `include` directory, which is located in the root directory of the repository. Consult your compiler or IDE documentation on how to add include directories to your project.

### Enable a modern C++ standard

Simdee requires C++11 language standard or newer. The table below lists which compiler flag to use to enable a modern C++ standard:

Language standard  | Clang          | GCC            | MSVC
-------------------|----------------|----------------|--------------
C++11              | `-std=c++11`   | `-std=c++11`   | no flag
C++14              | `-std=c++14`   | `-std=c++14`   | N/A

If you are using an IDE, look for a language standard option. You can skip this step when using Microsoft Visual Studio 2015 or newer.

### Enable instruction set

Simdee cannot produce any SIMD-accelerated code unless there is a SIMD instruction set extension available. The table below lists which compiler flag to use to enable the desired instruction set:

Instruction set    | Clang          | GCC            | MSVC
-------------------|----------------|----------------|--------------
SSE2               | `-msse2`       | `-msse2`       | `/arch:SSE2`
AVX                | `-mavx`        | `-mavx`        | `/arch:AVX`
AVX2               | `-mavx2`       | `-mavx2`       | `/arch:AVX2`

Beware that if your computer doesn't support the instruction set that you select, the program will crash. No compiler flag is necessary when you want to target SSE2 on a x86-64 architecture; all 64-bit processors support SSE2.

## Library configuration

You can define the following macros before you include Simdee headers:

* `SIMDEE_NEED_INT` (default value `1`) enables or disables integral arithmetic. When set to `0`, most operations with signed and unsigned vectors are disabled (see reference for [SIMDVectorS](../reference/SIMDVectorS.md) and [SIMDVectorU](../reference/SIMDVectorU.md) for a full list).

## Availability of type families

Both compiler and library configuration influence what types (and, therefore, what instruction sets) are enabled for use. The following table lists enabled architecture-dependent type families for a specific configuration:

Instruction set         | `#define SIMDEE_NEED_INT 1`                                | `#define SIMDEE_NEED_INT 0`
------------------------|------------------------------------------------------------|------------------------------------------------------------
SSE2                    | [`sse`](../reference/sse.md)                               | [`sse`](../reference/sse.md)
AVX                     | [`sse`](../reference/sse.md)                               | [`sse`](../reference/sse.md), [`avx`](../reference/avx.md)
AVX2                    | [`sse`](../reference/sse.md), [`avx`](../reference/avx.md) | [`sse`](../reference/sse.md), [`avx`](../reference/avx.md)

When coding with Simdee, you should prefer architecture-agnostic type families [`vec4`](../reference/vec4.md) and [`vec8`](../reference/vec8.md) to the architecture-specific ones above.
