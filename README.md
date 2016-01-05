# simdify

## Overview

- Header-only SIMD abstraction library.
- C++11 is required (GCC 4.7, Clang 3.2, Visual Studio 2015).
- Open-source, distributed under the MIT license.

## Basic usage

    #include <simdify/simdify.hpp>
    #include <simdify/storage.hpp>
    #include <iostream>
    
    int main() {
        // gather some data
        simd::storage<simd::ssef> stor;
        stor[0] = 1.0f;
        stor[1] = 2.0f;
        stor[2] = 3.0f;
        stor[3] = 4.0f;

        // load from memory into a SSE register
        simd::ssef reg = stor;
        
        // do some math
        reg = reg * reg + reg;

        // store back into memory
        stor = reg;
        
        // print the results
        std::cout << stor[0] << std::endl; // 2
        std::cout << stor[1] << std::endl; // 6
        std::cout << stor[2] << std::endl; // 12
        std::cout << stor[3] << std::endl; // 20
    }

## Basic usage - with `vecf`

    #include <simdify/simdify.hpp>
    #include <simdify/storage.hpp>
    #include <iostream>

    int main() {
        // gather some data
        // simd::vecf is:
        // - simd::avxf, if AVX is available
        // - simd::ssef, if SSE is available
        // - simd::dumf otherwise
        // simd::vecf::W (SIMD width) is:
        // - 8, if AVX is available
        // - 4, if SSE is available
        // - 1 otherwise
        simd::storage<simd::vecf> stor;
        for (int i = 0; i < simd::vecf::W; ++i) {
            stor[i] = i + 1.f;
        }

        // load from memory into a register -- as before
        simd::vecf reg = stor;

        // do some math -- as before
        reg = reg * reg + reg;

        // store back into memory -- as before
        stor = reg;

        // print the results
        for (int i = 0; i < simd::vecf::W; ++i) {
            std::cout << stor[i] << std::endl;
        }
    }
