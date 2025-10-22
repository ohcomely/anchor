#include "reduction.h"
#include <omp.h>
#include <stdexcept>
#include <vector>
#include <cmath>

namespace anchor {

// Configuration: chunk size for fixed tree algorithm
// Should be large enough to amortize synchronization cost
static const size_t DEFAULT_CHUNK_SIZE = 1024;

// Helper function to get default thread count
static int get_default_thread_count() {
    return omp_get_max_threads();
}

// Algorithm A: Fixed Binary Tree Reduction
// Template for code reuse between float and double
template<typename T>
static T fixed_tree_sum(const T* array, size_t n, int num_threads) {
    // Calculate number of chunks (independent of thread count)
    const size_t chunk_size = DEFAULT_CHUNK_SIZE;
    const size_t num_chunks = (n + chunk_size - 1) / chunk_size;

    // Allocate storage for partial results
    std::vector<T> partial_sums(num_chunks);

    // Phase 1: Parallel reduction of chunks
    // Each thread processes one or more chunks sequentially
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for schedule(static)
        for (size_t chunk_id = 0; chunk_id < num_chunks; ++chunk_id) {
            size_t start = chunk_id * chunk_size;
            size_t end = std::min(start + chunk_size, n);

            // Sequential reduction within chunk
            T chunk_sum = 0;
            for (size_t i = start; i < end; ++i) {
                chunk_sum += array[i];
            }

            partial_sums[chunk_id] = chunk_sum;
        }
    }

    // Phase 2: Sequential binary tree reduction of partial sums
    // This ensures deterministic combination order
    size_t active_count = num_chunks;
    while (active_count > 1) {
        size_t next_count = (active_count + 1) / 2;

        for (size_t i = 0; i < active_count / 2; ++i) {
            partial_sums[i] = partial_sums[2 * i] + partial_sums[2 * i + 1];
        }

        // Handle odd count - last element stays in place
        if (active_count % 2 == 1) {
            partial_sums[next_count - 1] = partial_sums[active_count - 1];
        }

        active_count = next_count;
    }

    return partial_sums[0];
}

// Double precision implementations
double reproducible_sum(const double* array, size_t n, Algorithm algo) {
    return reproducible_sum(array, n, algo, get_default_thread_count());
}

double reproducible_sum(const double* array, size_t n, Algorithm algo, int num_threads) {
    if (array == nullptr) {
        throw std::invalid_argument("array cannot be null");
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
            return fixed_tree_sum(array, n, num_threads);

        case Algorithm::COMPENSATED:
        case Algorithm::EXACT:
        case Algorithm::TWO_LEVEL:
            throw std::runtime_error("Algorithm not yet implemented");

        default:
            throw std::invalid_argument("Unknown algorithm");
    }
}

// Float precision implementations
float reproducible_sum(const float* array, size_t n, Algorithm algo) {
    return reproducible_sum(array, n, algo, get_default_thread_count());
}

float reproducible_sum(const float* array, size_t n, Algorithm algo, int num_threads) {
    if (array == nullptr) {
        throw std::invalid_argument("array cannot be null");
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
            return fixed_tree_sum(array, n, num_threads);

        case Algorithm::COMPENSATED:
        case Algorithm::EXACT:
        case Algorithm::TWO_LEVEL:
            throw std::runtime_error("Algorithm not yet implemented");

        default:
            throw std::invalid_argument("Unknown algorithm");
    }
}

} // namespace anchor
