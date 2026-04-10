#include "Benchmarking.hpp"
#include "VSet.hpp"

#include <vector>
#include <random>
#include <unordered_set>

using namespace utils::bench;

// -----------------------------------------------------
static std::vector<int> make_dataset(size_t N) {
    std::vector<int> data(N);

    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist;

    for (auto& x : data)
        x = dist(rng);

    return data;
}

// -----------------------------------------------------
// VSet INSERT
// -----------------------------------------------------
BENCH(vset, insert_random) {

    size_t N = 5'000'000;
    auto data = make_dataset(N);

    VSet vs(N * 2);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        vs.insert(data[i % N]);
        do_not_optimize(vs);
    });
}

// -----------------------------------------------------
// VSet LOOKUP
// -----------------------------------------------------
BENCH(vset, lookup) {

    size_t N = 5'000'000;
    auto data = make_dataset(N);

    VSet vs(N * 2);

    for (size_t i = 0; i < N; ++i)
        vs.insert(data[i]);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        bool res = vs.contains(data[i % N]);
        do_not_optimize(res);
    });
}

// -----------------------------------------------------
// unordered_set INSERT
// -----------------------------------------------------
BENCH(unordered_set, insert_random) {

    size_t N = 5'000'000;
    auto data = make_dataset(N);

    std::unordered_set<int> s;
    s.reserve(N * 2);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        s.insert(data[i % N]);
        do_not_optimize(s.size());
    });
}

// -----------------------------------------------------
// unordered_set LOOKUP
// -----------------------------------------------------
BENCH(unordered_set, lookup) {

    size_t N = 5'000'000;
    auto data = make_dataset(N);

    std::unordered_set<int> s;
    s.reserve(N * 2);

    for (size_t i = 0; i < N; ++i)
        s.insert(data[i]);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        bool res = s.find(data[i % N]) != s.end();
        do_not_optimize(res);
    });
}

BENCH(vset, insert_and_scan) {
    const size_t N = 20000;
    auto data = make_dataset(N);

    const size_t K = 300;
    auto forbidden = make_dataset(K);

    BENCH_RUN(bench, [&](size_t x, auto&) {
        VSet s (K*2);

        // insert
        for (size_t i = 0; i < K; ++i) {
            s.insert(forbidden[i]);
        }

        // scan
        for (size_t i = 0; i < N; ++i) {
            bool res = s.contains(data[i]);
            do_not_optimize(res);
        }
        });
}

BENCH(unordered_set, insert_and_scan) {
    const size_t N = 20000;
    auto data = make_dataset(N);

    const size_t K = 300;
    auto forbidden = make_dataset(K);

    BENCH_RUN(bench, [&](size_t x, auto&) {
        std::unordered_set<int> s;
        s.reserve(K * 2);

        // insert
        for (size_t i = 0; i < K; ++i) {
            s.insert(forbidden[i]);
        }

        // scan
        for (size_t i = 0; i < N; ++i) {
            bool res = s.find(data[i]) != s.end();
            do_not_optimize(res);
        }
        });
}

// -----------------------------------------------------
int main() {
    run_all();
}