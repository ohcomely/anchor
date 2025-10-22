#include "dot_product.h"
#include <omp.h>
#include <stdexcept>
#include <cmath>
#include <vector>

namespace anchor {

// Configuration: chunk size for fixed tree algorithm
static const size_t DEFAULT_CHUNK_SIZE = 1024;

// Helper function to get default thread count
static int get_default_thread_count() {
    return omp_get_max_threads();
}

// Algorithm A: Fixed Binary Tree Reduction for Dot Product
template<typename T>
static T fixed_tree_dot(const T* x, const T* y, size_t n, int num_threads) {
    // Calculate number of chunks (independent of thread count)
    const size_t chunk_size = DEFAULT_CHUNK_SIZE;
    const size_t num_chunks = (n + chunk_size - 1) / chunk_size;

    // Allocate storage for partial results
    std::vector<T> partial_dots(num_chunks);

    // Phase 1: Parallel reduction of chunks
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for schedule(static)
        for (size_t chunk_id = 0; chunk_id < num_chunks; ++chunk_id) {
            size_t start = chunk_id * chunk_size;
            size_t end = std::min(start + chunk_size, n);

            // Sequential dot product within chunk
            T chunk_dot = 0;
            for (size_t i = start; i < end; ++i) {
                chunk_dot += x[i] * y[i];
            }

            partial_dots[chunk_id] = chunk_dot;
        }
    }

    // Phase 2: Sequential binary tree reduction of partial sums
    size_t active_count = num_chunks;
    while (active_count > 1) {
        size_t next_count = (active_count + 1) / 2;

        for (size_t i = 0; i < active_count / 2; ++i) {
            partial_dots[i] = partial_dots[2 * i] + partial_dots[2 * i + 1];
        }

        // Handle odd count
        if (active_count % 2 == 1) {
            partial_dots[next_count - 1] = partial_dots[active_count - 1];
        }

        active_count = next_count;
    }

    return partial_dots[0];
}

// Double precision dot product
double reproducible_dot(const double* x, const double* y, size_t n, Algorithm algo) {
    return reproducible_dot(x, y, n, algo, get_default_thread_count());
}

double reproducible_dot(const double* x, const double* y, size_t n, Algorithm algo, int num_threads) {
    if (x == nullptr || y == nullptr) {
        throw std::invalid_argument("arrays cannot be null");
    }
    if (n == 0) {
        return 0.0;
    }
    if (num_threads < 1) {
        throw std::invalid_argument("num_threads must be at least 1");
    }

    // Dispatch to algorithm implementation
    switch (algo) {
        case Algorithm::FIXED_TREE:
            return fixed_tree_dot(x, y, n, num_threads);

        case Algorithm::COMPENSATED:
        case Algorithm::EXACT:
        case Algorithm::TWO_LEVEL:
            throw std::runtime_error("Algorithm not yet implemented");

        default:
            throw std::invalid_argument("Unknown algorithm");
    }
}

// Float precision dot product
float reproducible_dot(const float* x, const float* y, size_t n, Algorithm algo) {
    return reproducible_dot(x, y, n, algo, get_default_thread_count());
}

float reproducible_dot(const float* x, const float* y, size_t n, Algorithm algo, int num_threads) {
    if (x == nullptr || y == nullptr) {
        throw std::invalid_argument("arrays cannot be null");
    }
    if (n == 0) {
        return 0.0f;
    }
    if (num_threads < 1) {
        throw std::invalid_argument("num_threads must be at least 1");
    }

    // Dispatch to algorithm implementation
    switch (algo) {
        case Algorithm::FIXED_TREE:
            return fixed_tree_dot(x, y, n, num_threads);

        case Algorithm::COMPENSATED:
        case Algorithm::EXACT:
        case Algorithm::TWO_LEVEL:
            throw std::runtime_error("Algorithm not yet implemented");

        default:
            throw std::invalid_argument("Unknown algorithm");
    }
}

// L2 norm - double precision
double reproducible_norm2(const double* x, size_t n, Algorithm algo) {
    return reproducible_norm2(x, n, algo, get_default_thread_count());
}

double reproducible_norm2(const double* x, size_t n, Algorithm algo, int num_threads) {
    if (x == nullptr) {
        throw std::invalid_argument("array cannot be null");
    }
    if (n == 0) {
        return 0.0;
    }

    // Stub implementation - use dot product with itself
    double dot = reproducible_dot(x, x, n, algo, num_threads);
    return std::sqrt(dot);
}

// L2 norm - float precision
float reproducible_norm2(const float* x, size_t n, Algorithm algo) {
    return reproducible_norm2(x, n, algo, get_default_thread_count());
}

float reproducible_norm2(const float* x, size_t n, Algorithm algo, int num_threads) {
    if (x == nullptr) {
        throw std::invalid_argument("array cannot be null");
    }
    if (n == 0) {
        return 0.0f;
    }

    // Stub implementation
    float dot = reproducible_dot(x, x, n, algo, num_threads);
    return std::sqrt(dot);
}

} // namespace anchor
