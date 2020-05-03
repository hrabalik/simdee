cmake_minimum_required(VERSION 3.4.1)
project(simdee)

# Options
set(SIMDEE_INSTRUCTION_SET "default" CACHE STRING "Instruction set used to produce SIMD-accelerated code")
set(SIMDEE_BUILD_TESTS OFF CACHE BOOL "Build test runner executable")
set(SIMDEE_BUILD_BENCHMARKS OFF CACHE BOOL "Build benchmark runner executable")
set(SIMDEE_AARCH64 OFF CACHE BOOL "Set this option when the target architecture is AArch64, as opposed to 32-bit ARM (determines how NEON is enabled via compiler switches)")
set_property(CACHE SIMDEE_INSTRUCTION_SET PROPERTY STRINGS "default" "SSE2" "AVX" "AVX2" "NEON")

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
        target_compile_definitions(simdee INTERFACE "__AVX__") # Fixes MSVC code highlighting
    else()
        target_compile_options(simdee INTERFACE "-mavx")
    endif()
elseif(${SIMDEE_INSTRUCTION_SET} STREQUAL "AVX2")
    if(MSVC)
        target_compile_options(simdee INTERFACE "/arch:AVX2")
        target_compile_definitions(simdee INTERFACE "__AVX__" "__AVX2__") # Fixes MSVC code highlighting
    else()
        target_compile_options(simdee INTERFACE "-mavx2")
    endif()
elseif(${SIMDEE_INSTRUCTION_SET} STREQUAL "NEON")
    if(NOT SIMDEE_AARCH64)
        target_compile_options(simdee INTERFACE "-mfpu=neon")
    endif()
else()
    message(FATAL_ERROR "SIMDEE_INSTRUCTION_SET has an unexpected value")
endif()

# Add a target that supplies warning flags to depending targets
include(cmake/CompilerWarnings.cmake)
add_library(simdee-warnings INTERFACE)
target_set_strict_warnings(simdee-warnings)

# Build executables
if(SIMDEE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
if(SIMDEE_BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()
