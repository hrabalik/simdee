# simdify

## Overview

- Header-only SIMD abstraction library.
- C++11 is required (GCC 4.7, Clang 3.2, Visual Studio 2013).
- Open-source, distributed under the MIT license.

## Basic usage

    #include <simdify/simdify.hpp>
    #include <iostream>
    
    int main(int, char**) {
        simd::sse sse; // also try: simd::avx
        
        // fill the SIMD variable with data
        sse.f[0] = 1.0f;
        sse.f[1] = 2.0f;
        sse.f[2] = 3.0f;
        sse.f[3] = 4.0f;
        
        // do some math
        sse = sse * sse + sse;
        
        // print the contents of the SIMD variable
        std::cout << sse.f[0] << std::endl; // 2
        std::cout << sse.f[1] << std::endl; // 6
        std::cout << sse.f[2] << std::endl; // 12
        std::cout << sse.f[3] << std::endl; // 20
        
        return 0;
    }

## Basic usage - generic programming

    #include <simdify/simdify.hpp>
    #include <iostream>
    
    int main(int, char**) {
        // reg has type:
        // - simd::avx, if AVX is available
        // - simd::sse, if SSE is available
        // - simd::flt otherwise
        simd::simd_t reg;
        
        // fill the SIMD variable with data
        for (int i = 0; i < simd::simd_t::W; ++i) {
            reg.f[i] = i + 1;
        }
        
        // do some math
        reg = reg * reg + reg;
        
        // print the contents of the SIMD variable
        for (int i = 0; i < simd::simd_t::W; +i) {
            std::cout << reg.f[i] << std::endl;
        }
        
        return 0;
    }
