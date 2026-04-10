#include "Benchmarking.hpp"
#include "Uniform.hpp"
#include "Marsaglia.hpp"

#include <vector>
#include <random>

using namespace utils::bench;

BENCH(std_normal, generate) {
    const size_t N = 20000;

    std::mt19937_64 rng(123);
    std::normal_distribution<double> dist(0.0, 1.0);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        for (size_t j = 0; j < N; ++j) {
            double x = dist(rng);
            do_not_optimize(x);
        }
    });
}

BENCH(vmarsaglia, refill_and_scan) {
    const size_t N = 20000;
    VMarsaglia dist(N);

    BENCH_RUN(bench, [&](size_t x, auto&) {
        dist.refill();

        for (size_t j = 0; j < N; ++j) {
            double x = dist();
            do_not_optimize(x);
        }
    });
}

// -----------------------------------------------------
int main() {
    run_all();
}