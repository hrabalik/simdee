# List test files
set(TEST_FILES
    allocator.cpp
    casts.cpp
    deferred_not.cpp
    expr.cpp
    main.cpp
    mask.cpp
    simd_vector.inl
    simd_vector_dual.cpp
    simd_vector_dum.cpp
    simd_vector_vec4.cpp
    simd_vector_vec8.cpp
    storage.cpp
)

# List library files
set(LIB_FILES_TOPLEVEL
    "../include/simdee/simdee.hpp"
    "../include/simdee/vec4.hpp"
    "../include/simdee/vec8.hpp"
)
set(LIB_FILES_COMMON
    "../include/simdee/common/casts.hpp"
    "../include/simdee/common/deferred_not.hpp"
    "../include/simdee/common/expr.hpp"
    "../include/simdee/common/init.hpp"
    "../include/simdee/common/mask.hpp"
    "../include/simdee/common/storage.hpp"
)
set(LIB_FILES_SIMD_VECTORS
    "../include/simdee/simd_vectors/avx.hpp"
    "../include/simdee/simd_vectors/common.hpp"
    "../include/simdee/simd_vectors/dual.hpp"
    "../include/simdee/simd_vectors/dum.hpp"
    "../include/simdee/simd_vectors/neon.hpp"
    "../include/simdee/simd_vectors/sse.hpp"
)
set(LIB_FILES_UTIL
    "../include/simdee/util/allocator.hpp"
    "../include/simdee/util/bitfiddle.hpp"
    "../include/simdee/util/inline.hpp"
    "../include/simdee/util/integral.hpp"
    "../include/simdee/util/macros.hpp"
    "../include/simdee/util/noexcept.hpp"
)

# Create test runner, add library files to it
add_executable(simdee-test
    ${TEST_FILES}
    ${LIB_FILES_TOPLEVEL}
    ${LIB_FILES_COMMON}
    ${LIB_FILES_SIMD_VECTORS}
    ${LIB_FILES_UTIL}
)
target_link_libraries(simdee-test PRIVATE simdee simdee-warnings Catch2::Catch2)

# Enable fast Catch compilation
target_compile_definitions(simdee-test PRIVATE "CATCH_CONFIG_FAST_COMPILE")

# Add the executable to CTest
add_test(NAME simdee-test COMMAND simdee-test)

# Organize files in the IDE
source_group("tests" FILES ${TEST_FILES})
source_group("library" FILES ${LIB_FILES_TOPLEVEL})
source_group("library\\common" FILES ${LIB_FILES_COMMON})
source_group("library\\simd_vectors" FILES ${LIB_FILES_SIMD_VECTORS})
source_group("library\\util" FILES ${LIB_FILES_UTIL})
