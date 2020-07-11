add_executable(simdee-precision-sqrt precision_sqrt.cpp)
target_link_libraries(simdee-precision-sqrt PRIVATE simdee simdee-warnings)

add_executable(simdee-precision-rsqrt precision_rsqrt.cpp)
target_link_libraries(simdee-precision-rsqrt PRIVATE simdee simdee-warnings)
