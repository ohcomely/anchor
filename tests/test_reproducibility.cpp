#include "anchor_linalg.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <random>

using namespace anchor;

bool bitwise_equal(double a, double b) {
    uint64_t a_bits, b_bits;
    std::memcpy(&a_bits, &a, sizeof(double));
    std::memcpy(&b_bits, &b, sizeof(double));
    return a_bits == b_bits;
}

bool test_operation(const char* name, double reference, double result, int threads) {
    if (!bitwise_equal(reference, result)) {
        std::cerr << "FAIL: " << name << " (" << threads << " threads)" << std::endl;
        return false;
    }
    return true;
}

int main() {
    const size_t n = 100000;
    std::vector<double> x(n), y(n);

    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (size_t i = 0; i < n; ++i) {
        x[i] = dist(gen);
        y[i] = dist(gen);
    }

    // Get reference results with 1 thread
    double ref_sum = reproducible_sum(x.data(), n, Algorithm::FIXED_TREE, 1);
    double ref_dot = reproducible_dot(x.data(), y.data(), n, Algorithm::FIXED_TREE, 1);
    double ref_norm = reproducible_norm2(x.data(), n, Algorithm::FIXED_TREE, 1);

    bool passed = true;

    // Test with different thread counts
    for (int t : {2, 4, 8}) {
        passed &= test_operation("sum", ref_sum,
            reproducible_sum(x.data(), n, Algorithm::FIXED_TREE, t), t);
        passed &= test_operation("dot", ref_dot,
            reproducible_dot(x.data(), y.data(), n, Algorithm::FIXED_TREE, t), t);
        passed &= test_operation("norm", ref_norm,
            reproducible_norm2(x.data(), n, Algorithm::FIXED_TREE, t), t);
    }

    std::cout << (passed ? "PASS" : "FAIL") << std::endl;
    return passed ? 0 : 1;
}
