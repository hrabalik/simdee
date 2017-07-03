file(GLOB_RECURSE SOURCE_FILES "*.hpp" "*.cpp" "*.inl")
add_executable(simdee-test ${SOURCE_FILES})
target_link_libraries(simdee-test PRIVATE simdee)



# Enable strict warnings
if (MSVC)
    target_compile_options(simdee-test PRIVATE "/W4" "/WX")
else()
    target_compile_options(simdee-test PRIVATE "-Wall" "-Werror")
endif()
