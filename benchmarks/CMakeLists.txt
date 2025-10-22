# Benchmarks CMakeLists.txt
# Placeholder for now - will add Google Benchmark later

# Simple benchmark executable (no framework yet)
add_executable(simple_benchmark benchmark_simple.cpp)
target_link_libraries(simple_benchmark PRIVATE anchor_linalg)
