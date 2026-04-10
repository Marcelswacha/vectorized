#include "../utils/Benchmarking.hpp"
#include "VSet.hpp"

#include <vector>
#include <random>

using namespace utils::bench;

// ----------------------------------------
// Random insert benchmark
// ----------------------------------------
BENCH(vset, insert_random) {

    const size_t N = bench.iterations;

    std::vector<int> dataset(N);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 1'000'000);

    for (auto& x : dataset)
        x = dist(rng);

    VSet vs(N * 2); // avoid too many rehashes

    BENCH_RUN(bench, [&](size_t i) {
        vs.insert(dataset[i]);
    });
}

// ----------------------------------------
// Sequential insert benchmark
// ----------------------------------------
BENCH(vset, insert_sequential) {

    const size_t N = bench.iterations;

    VSet vs(N * 2);

    BENCH_RUN(bench, [&](size_t i) {
        vs.insert(static_cast<int>(i));
    });
}

// ----------------------------------------
// Lookup benchmark
// ----------------------------------------
BENCH(vset, lookup) {

    const size_t N = bench.iterations;

    VSet vs(N * 2);

    for (size_t i = 0; i < N; ++i)
        vs.insert(static_cast<int>(i));

    BENCH_RUN(bench, [&](size_t i) {
        auto res = vs.contains(static_cast<int>(i));
        do_not_optimize(res);
    });
}

// ----------------------------------------
// Main
// ----------------------------------------
int main() {
    run_all();          // run all benchmarks
    // run_all("vset"); // only vset group
}