add_executable(simdee-raybox raybox.cpp)
target_link_libraries(simdee-raybox PRIVATE simdee)

# Enable strict warnings
if (MSVC)
    target_compile_options(simdee-bench PRIVATE "/W4" "/WX")
else()
    target_compile_options(simdee-bench PRIVATE "-Wall" "-Werror")
endif()
