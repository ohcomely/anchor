#include "anchor_linalg.h"
#include <iostream>
#include <cmath>

// Simple basic test without a framework
// Just verifies the library compiles and links

int main() {
    using namespace anchor;

    // Test 1: Basic sum
    double data[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double sum = reproducible_sum(data, 5, Algorithm::FIXED_TREE);

    std::cout << "Sum test: " << sum << " (expected 15.0)" << std::endl;
    if (std::abs(sum - 15.0) > 1e-10) {
        std::cerr << "FAILED: Sum test" << std::endl;
        return 1;
    }

    // Test 2: Basic dot product
    double x[] = {1.0, 2.0, 3.0};
    double y[] = {4.0, 5.0, 6.0};
    double dot = reproducible_dot(x, y, 3, Algorithm::FIXED_TREE);

    std::cout << "Dot product test: " << dot << " (expected 32.0)" << std::endl;
    if (std::abs(dot - 32.0) > 1e-10) {
        std::cerr << "FAILED: Dot product test" << std::endl;
        return 1;
    }

    // Test 3: Basic norm
    double v[] = {3.0, 4.0};
    double norm = reproducible_norm2(v, 2, Algorithm::FIXED_TREE);

    std::cout << "Norm test: " << norm << " (expected 5.0)" << std::endl;
    if (std::abs(norm - 5.0) > 1e-10) {
        std::cerr << "FAILED: Norm test" << std::endl;
        return 1;
    }

    std::cout << "All basic tests passed!" << std::endl;
    return 0;
}
