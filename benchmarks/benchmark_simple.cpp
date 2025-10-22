#include "anchor_linalg.h"
#include <iostream>
#include <chrono>
#include <vector>

// Simple timing benchmark without a framework

int main() {
    using namespace anchor;
    using namespace std::chrono;

    const size_t n = 1000000;
    std::vector<double> data(n);

    // Initialize with some values
    for (size_t i = 0; i < n; ++i) {
        data[i] = static_cast<double>(i) / n;
    }

    std::cout << "Running benchmark with n = " << n << std::endl;

    // Benchmark sum
    auto start = high_resolution_clock::now();
    double result = reproducible_sum(data.data(), n, Algorithm::FIXED_TREE);
    auto end = high_resolution_clock::now();

    auto duration_ms = duration_cast<milliseconds>(end - start).count();

    std::cout << "Result: " << result << std::endl;
    std::cout << "Time: " << duration_ms << " ms" << std::endl;

    return 0;
}
