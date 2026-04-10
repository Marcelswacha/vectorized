#include "../utils/Benchmarking.hpp"
#include "MinHeap.hpp"

#include <vector>
#include <algorithm>
#include <random>

using namespace utils::bench;

static std::vector<int> make_dataset(size_t n) {
    std::vector<int> data(n);

    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist(0, 1'000'000);

    for (size_t i = 0; i < n; ++i) {
        data[i] = dist(rng);
    }

    return data;
}

BENCH(fixed_minheap, topk_20_from_20000) {
    const size_t N = 20000;
    const size_t K = 20;

    auto data = make_dataset(N);

    FixedMinHeap<int> heap(K);

    BENCH_RUN(bench, [&](size_t, auto&) {
        heap.clear();

        for (size_t i = 0; i < N; ++i) {
            heap.insert(data[i]);
        }

        auto res = heap.getTopItems();
        do_not_optimize(res);
    });
}

BENCH(nth_element, topk_20_from_20000) {
    const size_t N = 20000;
    const size_t K = 20;

    auto original = make_dataset(N);
    std::vector<int> data;

    BENCH_RUN(bench, [&](size_t, auto&) {
        data = original;

        std::nth_element(
            data.begin(),
            data.end() - K,
            data.end()
        );

        data.erase(data.begin(), data.end() - K);

        do_not_optimize(data);
    });
}

int main() {
    run_all();
}