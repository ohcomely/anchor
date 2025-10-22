#pragma once

#include <cstddef>

namespace anchor {

// Simple thread pool for deterministic parallel execution
class ThreadPool {
public:
    ThreadPool(int num_threads);
    ~ThreadPool();

    // Execute function f on range [begin, end) in parallel
    // Each thread gets a deterministic chunk of work
    void parallel_for(size_t begin, size_t end, void (*f)(size_t, size_t, int));

    // Wait for all threads to reach this point
    void barrier();

    // Get thread ID (0 to num_threads-1)
    int get_thread_id() const;

    // Get total number of threads
    int get_num_threads() const;

private:
    int num_threads_;
};

} // namespace anchor
