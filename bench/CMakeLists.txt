if (${SIMDEE_INSTRUCTION_SET} STREQUAL "AVX" OR
    ${SIMDEE_INSTRUCTION_SET} STREQUAL "AVX2")
    # AVX is required to build the raybox benchmark
    add_subdirectory(raybox)
endif()
