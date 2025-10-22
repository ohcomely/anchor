#include "anchor_linalg.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <random>
#include <omp.h>
#include <set>

using namespace anchor;

bool bitwise_equal(double a, double b) {
    uint64_t a_bits, b_bits;
    std::memcpy(&a_bits, &a, sizeof(double));
    std::memcpy(&b_bits, &b, sizeof(double));
    return a_bits == b_bits;
}

double non_reproducible_sum(const double* array, size_t n, int num_threads) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum) num_threads(num_threads)
    for (size_t i = 0; i < n; ++i) {
        sum += array[i];
    }
    return sum;
}

int main() {
    const size_t n = 1000000;
    std::vector<double> data(n);

    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (size_t i = 0; i < n; ++i) {
        data[i] = dist(gen);
    }

    // Test 1: Multiple runs, same thread count
    std::set<uint64_t> openmp_results, anchor_results;
    for (int run = 0; run < 20; ++run) {
        uint64_t bits;
        double r = non_reproducible_sum(data.data(), n, 8);
        std::memcpy(&bits, &r, sizeof(double));
        openmp_results.insert(bits);

        r = reproducible_sum(data.data(), n, Algorithm::FIXED_TREE, 8);
        std::memcpy(&bits, &r, sizeof(double));
        anchor_results.insert(bits);
    }

    std::cout << "20 runs with 8 threads:" << std::endl;
    std::cout << "  OpenMP:  " << openmp_results.size() << " unique results" << std::endl;
    std::cout << "  Anchor:  " << anchor_results.size() << " unique results" << std::endl;

    // Test 2: Different thread counts
    std::vector<double> openmp_threads, anchor_threads;
    for (int t : {1, 2, 4, 8}) {
        openmp_threads.push_back(non_reproducible_sum(data.data(), n, t));
        anchor_threads.push_back(reproducible_sum(data.data(), n, Algorithm::FIXED_TREE, t));
    }

    bool openmp_same = true, anchor_same = true;
    for (size_t i = 1; i < 4; ++i) {
        if (!bitwise_equal(openmp_threads[0], openmp_threads[i])) openmp_same = false;
        if (!bitwise_equal(anchor_threads[0], anchor_threads[i])) anchor_same = false;
    }

    std::cout << "\nAcross 1,2,4,8 threads:" << std::endl;
    std::cout << "  OpenMP:  " << (openmp_same ? "identical" : "different") << std::endl;
    std::cout << "  Anchor:  " << (anchor_same ? "identical" : "different") << std::endl;

    return 0;
}
