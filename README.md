# Anchor: Reproducible Multi-threaded Linear Algebra

A C++ library providing bitwise-reproducible parallel linear algebra operations.

## The Problem

Standard parallel reductions are non-deterministic:
```cpp
// Different thread counts → different results
#pragma omp parallel for reduction(+:sum)
for (i...) sum += array[i];
// Result varies due to floating-point non-associativity: (a+b)+c ≠ a+(b+c)
```

## The Solution

Anchor guarantees identical results across all thread counts and runs:
```cpp
#include "anchor_linalg.h"

double sum = anchor::reproducible_sum(data, n, Algorithm::FIXED_TREE);
// Always bitwise identical, regardless of threads or execution schedule
```

## Features

- **Operations:** sum, dot product, L2 norm
- **Algorithm A:** Fixed Binary Tree Reduction (currently implemented)
- **Strong reproducibility:** Bitwise identical across thread counts and runs
- **Zero dependencies:** Only OpenMP (standard)

## Building

```bash
mkdir build && cd build
cmake ..
make
./tests/reproducibility_test  # Verify bitwise reproducibility
./tests/comparison_test        # Compare with standard OpenMP
```

## Usage

```cpp
#include "anchor_linalg.h"

double data[1000000];
// ... initialize data ...

// Reproducible sum
double s = anchor::reproducible_sum(data, 1000000, anchor::Algorithm::FIXED_TREE);

// Reproducible dot product
double d = anchor::reproducible_dot(x, y, n, anchor::Algorithm::FIXED_TREE);

// Reproducible L2 norm
double norm = anchor::reproducible_norm2(x, n, anchor::Algorithm::FIXED_TREE);
```

## How It Works

**Algorithm A: Fixed Binary Tree Reduction**
1. Partition input into fixed-size chunks (1024 elements)
2. Parallel: each thread reduces assigned chunks sequentially
3. Sequential: combine partial results in deterministic binary tree

This ensures identical operation order regardless of thread scheduling.

## Status

- ✓ Algorithm A (Fixed Tree) implemented and tested
- ⏳ Algorithm B (Compensated Summation) - planned
- ⏳ Algorithm C (Exact Accumulation) - planned
- ⏳ Algorithm D (Two-Level Hierarchical) - planned

## Research

This library implements algorithms from reproducible computing research. See `document.txt` for full theoretical background and experimental design.
