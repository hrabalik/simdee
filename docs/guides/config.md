# Compiler and library configuration

## Compiler configuration

### Check compiler version

When coding with Simdee, be sure to use a recent version of your C++ toolchain. These are the supported compilers:

* Clang, currently tested under 4.0.0
* GCC, currently tested under 6.3.0
* MSVC, currrently tested under 15.0 (Visual Studio 2017)

### Add Simdee to project via CMake

If you are using CMake to build your project, integrating and configuring Simdee is quite straightforward (effectively allowing you to skip the rest of this document). First, copy the contents of the Simdee repository into a subdirectory in your source tree. Add the line `add_subdirectory(path/to/simdee)` to your `CMakeLists.txt`, where `path/to/simdee` is the path where Simdee is located relative to this file.

After that, you can make Simdee available to your sources by adding the line `target_link_libraries(mytarget PRIVATE simdee)` for each of the binaries that you wish to accelerate, `mytarget` being the name the binary. Finally, when you perform CMake configuration, additional parameters will pop up for tweaking; the most important one is `SIMDEE_INSTRUCTION_SET`, which lets you pick what instructions should be used.

### Add Simdee to include path

No CMake? Not to worry, integrating Simdee to your project is still very simple. Since Simdee is a header-only library, all you need to do is to add its headers to your include path. You can find all the headers in the `include` directory, which is located in the root directory of the repository. Consult your compiler or IDE documentation on how to add include directories to your project.

### Enable a modern C++ standard

Simdee requires the C++11 language standard or newer. Recent compilers either support it out of the box or provide a flag to turn it on; the same applies to C++14 (which we recommend for developing new applications).

Language standard  | Clang          | GCC <= 5       | GCC 6          | VS 2015      | VS 2017
-------------------|----------------|----------------|----------------|--------------|--------------
C++11              | `-std=c++11`   | `-std=c++11`   | no flag        | no flag      | no flag
C++14              | `-std=c++14`   | `-std=c++14`   | no flag        | N/A          | no flag

If you are using an IDE, look for a "language standard" option.

### Enable instruction set

Simdee cannot produce any SIMD-accelerated code unless there is a SIMD instruction set extension available. The table below lists which compiler flag to use to enable the desired instruction set:

Instruction set         | Clang          | GCC            | MSVC
------------------------|----------------|----------------|--------------
SSE2 on x85 (32-bit)    | `-msse2`       | `-msse2`       | `/arch:SSE2`
SSE2 on AMD64           | no flag        | no flag        | no flag
AVX on x85 (32-bit)     | `-mavx`        | `-mavx`        | `/arch:AVX`
AVX on AMD64            | `-mavx`        | `-mavx`        | `/arch:AVX`
AVX2 on x85 (32-bit)    | `-mavx2`       | `-mavx2`       | `/arch:AVX2`
AVX2 on AMD64           | `-mavx2`       | `-mavx2`       | `/arch:AVX2`
NEON on ARM (32-bit)    | `-mfpu=neon`   | `-mfpu=neon`   | no flag
NEON on ARM64 (AArch64) | no flag        | no flag        | N/A

Beware that if your computer lacks support for the instruction set that you select, the program may silently compile (without any warnings!), only to crash horribly once you run it.

## Library configuration

You can define the following macros before you include Simdee headers:

* `SIMDEE_NEED_INT` (default value `1`) enables or disables integral arithmetic. When set to `0`, most operations with signed and unsigned integral vectors are disabled (see reference for [SIMDVectorS](../reference/SIMDVectorS.md) and [SIMDVectorU](../reference/SIMDVectorU.md) for a full list).

## Availability of type families

Both compiler and library configuration influence what types (and, therefore, what instruction sets) are enabled for use. The following table lists enabled architecture-dependent type families for a specific configuration:

Instruction set         | `#define SIMDEE_NEED_INT 1` (default)                                | `#define SIMDEE_NEED_INT 0`
------------------------|----------------------------------------------------------------------|------------------------------------------------------------
SSE2                    | [`sd::sse_`](../reference/sse.md)                                    | [`sd::sse_`](../reference/sse.md)
AVX                     | [`sd::sse_`](../reference/sse.md)                                    | [`sd::sse_`](../reference/sse.md), [`sd::avx_`](../reference/avx.md)
AVX2                    | [`sd::sse_`](../reference/sse.md), [`sd::avx_`](../reference/avx.md) | [`sd::sse_`](../reference/sse.md), [`sd::avx_`](../reference/avx.md)
NEON                    | [`sd::neon_`](../reference/neon.md)                                  | [`sd::neon_`](../reference/neon.md)

* Note that even though using AVX doesn't allow you to use types named `avx_` (at least not in the default configuration), there  might still be significant performance gains over SSE2.
* Note that when coding with Simdee, one should prefer architecture-independent type families [`sd::vec4_`](../reference/vec4.md) and [`sd::vec8_`](../reference/vec8.md) to the architecture-specific ones above.
