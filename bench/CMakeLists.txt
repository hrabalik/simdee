file(GLOB_RECURSE SOURCE_FILES "*.hpp" "*.cpp" "*.inl")
add_executable(simdee-bench ${SOURCE_FILES})
target_link_libraries(simdee-bench PRIVATE simdee)

# Enable strict warnings
if (MSVC)
    target_compile_options(simdee-bench PRIVATE "/W4" "/WX")
else()
    target_compile_options(simdee-bench PRIVATE "-Wall" "-Werror")
endif()
