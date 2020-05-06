# credit: https://github.com/lefticus/cpp_starter_project

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")

  set(SIMDEE_SANITIZERS "")

  option(SIMDEE_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" FALSE)
  if(SIMDEE_ENABLE_SANITIZER_ADDRESS)
    list(APPEND SIMDEE_SANITIZERS "address")
  endif()

  option(SIMDEE_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" FALSE)
  if(SIMDEE_ENABLE_SANITIZER_MEMORY)
    list(APPEND SIMDEE_SANITIZERS "memory")
  endif()

  option(SIMDEE_ENABLE_SANITIZER_UNDEFINED "Enable undefined behavior sanitizer" FALSE)
  if(SIMDEE_ENABLE_SANITIZER_UNDEFINED)
    list(APPEND SIMDEE_SANITIZERS "undefined")
  endif()

  option(SIMDEE_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" FALSE)
  if(SIMDEE_ENABLE_SANITIZER_THREAD)
    list(APPEND SIMDEE_SANITIZERS "thread")
  endif()

  list(JOIN SIMDEE_SANITIZERS "," SIMDEE_SANITIZER_LIST)

endif()

if(SIMDEE_SANITIZER_LIST)
  if(NOT "${SIMDEE_SANITIZER_LIST}" STREQUAL "")
    target_compile_options(simdee INTERFACE -fsanitize=${SIMDEE_SANITIZER_LIST})
    target_link_libraries(simdee INTERFACE -fsanitize=${SIMDEE_SANITIZER_LIST})
  endif()
endif()
