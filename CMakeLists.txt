cmake_minimum_required(VERSION 3.14)
project(AnchorLinAlg VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Disable optimizations for reproducibility testing
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -O0)
endif()

# Find OpenMP
find_package(OpenMP REQUIRED)

# Library sources
set(REPRO_SOURCES
    src/reduction.cpp
    src/dot_product.cpp
    src/thread_pool.cpp
)

# Main library
add_library(anchor_linalg ${REPRO_SOURCES})
target_include_directories(anchor_linalg PUBLIC include)
target_link_libraries(anchor_linalg PUBLIC OpenMP::OpenMP_CXX)

# Optional: Enable tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Optional: Enable benchmarks
option(BUILD_BENCHMARKS "Build benchmarks" ON)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
