# Simdee

## Overview

- Header-only SIMD abstraction library.
- Handles SSE and AVX instruction sets.
- Open-source, distributed under the MIT license.

## Supported compilers

- Clang, currently tested under 3.8.0
- GCC, currently tested under 5.4.0
- MSVC, currrently tested under 14.0 (Visual Studio 2015)

## Installing

- Add the headers from the `include` directory into your include path.
- Set up your compiler to support C++11 or newer.
- Set up your compiler to emit the desired instruction set.

The table below lists which compiler flag to use to enable a modern C++ standard.

Language standard  | Clang        | GCC          | MSVC
-------------------|--------------|--------------|--------------
C++11              | -std=c++11   | -std=c++11   | no flag
C++14              | -std=c++14   | -std=c++14   | N/A

The table below lists which compiler flag to use to enable the desired instruction set.

Instruction set    | Clang        | GCC          | MSVC
-------------------|--------------|--------------|--------------
SSE2               | -msse2       | -msse2       | /arch:SSE2
AVX                | -mavx        | -mavx        | /arch:AVX
AVX2               | -mavx2       | -mavx2       | /arch:AVX2

No compiler flag is needed when you want to target SSE2 on a x86-64 architecture.
