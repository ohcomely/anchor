#pragma once

namespace anchor {

// Algorithm selection
enum class Algorithm {
    FIXED_TREE,        // Algorithm A: Fixed Binary Tree Reduction
    COMPENSATED,       // Algorithm B: Hierarchical Compensated Summation
    EXACT,             // Algorithm C: Exact Accumulation with Superaccumulators
    TWO_LEVEL          // Algorithm D: Two-Level Hierarchical Reduction
};

} // namespace anchor
