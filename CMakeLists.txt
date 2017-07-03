cmake_minimum_required(VERSION 3.4.1)
project(simdee)

# Options
set(SIMDEE_INSTRUCTION_SET "default" CACHE STRING "Instruction set used to produce SIMD-accelerated code")
set(SIMDEE_BUILD_TESTS OFF CACHE BOOL "Build test runner executable")
set(SIMDEE_BUILD_BENCHMARKS OFF CACHE BOOL "Build benchmark runner executable")
set_property(CACHE SIMDEE_INSTRUCTION_SET PROPERTY STRINGS "default" "SSE2" "AVX" "AVX2")

# Add a target for the header-only library
add_library(simdee INTERFACE)
target_include_directories(simdee INTERFACE include)
target_compile_features(simdee INTERFACE cxx_defaulted_functions cxx_deleted_functions cxx_delegating_constructors)

# Enable selected instruction set for library users
if(${SIMDEE_INSTRUCTION_SET} STREQUAL "default")
    # No flags added
elseif(${SIMDEE_INSTRUCTION_SET} STREQUAL "SSE2")
    if(MSVC)
        target_compile_options(simdee INTERFACE "/arch:SSE2")
    else()
        target_compile_options(simdee INTERFACE "-msse2")
    endif()
elseif(${SIMDEE_INSTRUCTION_SET} STREQUAL "AVX")
    if(MSVC)
        target_compile_options(simdee INTERFACE "/arch:AVX")
    else()
        target_compile_options(simdee INTERFACE "-mavx")
    endif()
elseif(${SIMDEE_INSTRUCTION_SET} STREQUAL "AVX2")
    if(MSVC)
        target_compile_options(simdee INTERFACE "/arch:AVX2")
    else()
        target_compile_options(simdee INTERFACE "-mavx2")
    endif()
else()
    message(FATAL_ERROR "SIMDEE_INSTRUCTION_SET has an unexpected value")
endif()

# Build executables
if(SIMDEE_BUILD_TESTS)
    add_subdirectory(test)
endif()
if(SIMDEE_BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()
