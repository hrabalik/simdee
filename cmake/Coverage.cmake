# credit: https://github.com/lefticus/cpp_starter_project

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")

  option(SIMDEE_ENABLE_COVERAGE "Enable coverage reporting" FALSE)

  if(SIMDEE_ENABLE_COVERAGE)
    if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      message(FATAL_ERROR "Build type must be set to Debug when reporting coverage")
    endif()
    target_compile_options(simdee INTERFACE --coverage)
    target_link_libraries(simdee INTERFACE --coverage)
  endif()

endif()
