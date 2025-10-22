# Basic correctness test
add_executable(basic_test test_basic.cpp)
target_link_libraries(basic_test PRIVATE anchor_linalg)

# Reproducibility validation (bitwise equality across thread counts)
add_executable(reproducibility_test test_reproducibility.cpp)
target_link_libraries(reproducibility_test PRIVATE anchor_linalg)

# Comparison: Anchor vs standard OpenMP
add_executable(comparison_test test_comparison.cpp)
target_link_libraries(comparison_test PRIVATE anchor_linalg)

# Add to CTest
add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME ReproducibilityTest COMMAND reproducibility_test)
