#pragma once

#include <cstddef>
#include "types.h"

namespace anchor {

// Reproducible sum reduction
// Returns the sum of array[0..n-1] using the specified algorithm
double reproducible_sum(const double* array, size_t n, Algorithm algo);

// Reproducible sum with explicit thread count
double reproducible_sum(const double* array, size_t n, Algorithm algo, int num_threads);

// Float version
float reproducible_sum(const float* array, size_t n, Algorithm algo);
float reproducible_sum(const float* array, size_t n, Algorithm algo, int num_threads);

} // namespace anchor
