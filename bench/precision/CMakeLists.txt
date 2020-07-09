add_executable(simdee-precision 
  # precision_sqrt.cpp
  precision_rsqrt.cpp
)
target_link_libraries(simdee-precision PRIVATE simdee simdee-warnings)
