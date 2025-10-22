# Testing Strategy

## Test Suite

### 1. `basic_test` - Correctness
Simple sanity checks: sum([1,2,3,4,5])=15, dot([1,2,3]·[4,5,6])=32, norm([3,4])=5

### 2. `reproducibility_test` - Strong Reproducibility
Tests bitwise equality (not tolerance-based) across operations and thread counts:
```cpp
bool bitwise_equal(double a, double b) {
    uint64_t a_bits, b_bits;
    std::memcpy(&a_bits, &a, sizeof(double));
    std::memcpy(&b_bits, &b, sizeof(double));
    return a_bits == b_bits;
}
```

Tests sum, dot product, and norm with 1, 2, 4, 8 threads on 100K random elements.
Each must be bit-for-bit identical to 1-thread reference result.

**Output:** `PASS` or `FAIL` with specific operation/thread count on failure.

### 3. `comparison_test` - Anchor vs OpenMP
Direct comparison on identical data:
- **Test 1:** 20 runs with 8 threads → counts unique bitwise results
- **Test 2:** Single run with 1,2,4,8 threads → checks if all identical

**Expected Results:**
```
20 runs with 8 threads:
  OpenMP:  2+ unique results
  Anchor:  1 unique results

Across 1,2,4,8 threads:
  OpenMP:  different
  Anchor:  identical
```

## Running Tests

```bash
cd build
make
./tests/basic_test
./tests/reproducibility_test
./tests/comparison_test

# Or via CTest
ctest
```

## Why This Works

**The Problem:** Standard parallel reductions have non-deterministic operation order
- Different thread scheduling → different operand grouping
- `(a+b)+c ≠ a+(b+c)` in floating-point

**Our Solution:** Algorithm A enforces deterministic operation order
- Fixed chunk boundaries (independent of thread count)
- Sequential chunk reduction
- Deterministic tree combination

**Validation:** Bitwise equality proves identical operation sequence across all configurations.
