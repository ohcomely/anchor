#pragma once

#include <cstddef>
#include "types.h"

namespace anchor {

// Reproducible dot product
// Returns the dot product of x[0..n-1] and y[0..n-1] using the specified algorithm
double reproducible_dot(const double* x, const double* y, size_t n, Algorithm algo);

// Reproducible dot product with explicit thread count
double reproducible_dot(const double* x, const double* y, size_t n, Algorithm algo, int num_threads);

// Float version
float reproducible_dot(const float* x, const float* y, size_t n, Algorithm algo);
float reproducible_dot(const float* x, const float* y, size_t n, Algorithm algo, int num_threads);

// L2 norm (uses dot product internally)
double reproducible_norm2(const double* x, size_t n, Algorithm algo);
double reproducible_norm2(const double* x, size_t n, Algorithm algo, int num_threads);

float reproducible_norm2(const float* x, size_t n, Algorithm algo);
float reproducible_norm2(const float* x, size_t n, Algorithm algo, int num_threads);

} // namespace anchor
