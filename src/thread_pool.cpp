#include "thread_pool.h"
#include <omp.h>

namespace anchor {

ThreadPool::ThreadPool(int num_threads) : num_threads_(num_threads) {
    omp_set_num_threads(num_threads_);
}

ThreadPool::~ThreadPool() {
    // Cleanup if needed
}

void ThreadPool::parallel_for(size_t begin, size_t end, void (*f)(size_t, size_t, int)) {
    // Use OpenMP to execute function f on chunks of the range
    // Each thread gets a deterministic chunk based on thread ID
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        size_t range = end - begin;
        size_t chunk_size = range / nthreads;
        size_t remainder = range % nthreads;

        // Deterministic chunk assignment
        size_t my_begin, my_end;
        if (tid < (int)remainder) {
            my_begin = begin + tid * (chunk_size + 1);
            my_end = my_begin + chunk_size + 1;
        } else {
            my_begin = begin + remainder * (chunk_size + 1) + (tid - remainder) * chunk_size;
            my_end = my_begin + chunk_size;
        }

        f(my_begin, my_end, tid);
    }
}

void ThreadPool::barrier() {
    #pragma omp barrier
}

int ThreadPool::get_thread_id() const {
    return omp_get_thread_num();
}

int ThreadPool::get_num_threads() const {
    return num_threads_;
}

} // namespace anchor
