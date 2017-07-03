# Can't build this without AVX
if (NOT ${SIMDEE_INSTRUCTION_SET} STREQUAL "AVX" AND
    NOT ${SIMDEE_INSTRUCTION_SET} STREQUAL "AVX2")
    message(FATAL_ERROR "AVX is required to build Simdee benchmarks")
endif()

# Create the benchmarking executable
set(SOURCE_FILES
    ray_box.cpp
)
add_executable(simdee-bench ${SOURCE_FILES})
target_link_libraries(simdee-bench PRIVATE simdee)

# Enable strict warnings
if (MSVC)
    target_compile_options(simdee-bench PRIVATE "/W4" "/WX")
else()
    target_compile_options(simdee-bench PRIVATE "-Wall" "-Werror")
endif()
