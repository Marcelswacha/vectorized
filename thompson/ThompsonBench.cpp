#include "Thompson.hpp"
#include "../utils/Benchmarking.hpp"

#include <vector>
#include <random>

using namespace utils::bench;

// -----------------------------------------------------
// Shared data container
// -----------------------------------------------------
struct ThompsonBenchData {
    std::vector<Item> items;
    std::vector<uint32_t> forbidden;
    size_t topk;
    Thompson11* thompson;
};

// -----------------------------------------------------
// Helper: build items with controlled regime
// -----------------------------------------------------
enum class Mode {
    ExactOnly,
    ApproxOnly,
    Mixed
};

static ThompsonBenchData prepare(size_t N, size_t forbiddenNum, size_t topk, Mode mode) {
    std::mt19937_64 rng(123);

    std::uniform_int_distribution<uint32_t> id_dist(1, 1'000'000);

    std::uniform_int_distribution<int> low_dist(0, 8);
    std::uniform_int_distribution<int> high_dist(100, 200);

    std::uniform_int_distribution<int> real_success(0, 10000);
    std::uniform_int_distribution<int> real_failure(0, 90000);

    ThompsonBenchData data;
    data.items.reserve(N);

    for (size_t i = 0; i < N; ++i) {
        int successes, failures;

        if (mode == Mode::ExactOnly) {
            // force Beta path
            successes = low_dist(rng);
            failures  = low_dist(rng);
        }
        else if (mode == Mode::ApproxOnly) {
            // force Gaussian path
            successes = high_dist(rng);
            failures  = successes + 1;
        }
        else {
            successes = real_success(rng);
            failures  = real_failure(rng);
        }

        data.items.push_back(Item{
            id_dist(rng),
            successes,
            failures
        });
    }

    // forbidden set
    data.forbidden.reserve(forbiddenNum);
    for (size_t i = 0; i < forbiddenNum && i < data.items.size(); ++i) {
        data.forbidden.push_back(data.items[i].id);
    }

    data.topk = topk;
    data.thompson = new Thompson11(data.items);

    return data;
}

// -----------------------------------------------------
// BENCH 1: exact Beta path
// -----------------------------------------------------
BENCH(thompson, exact_beta_only) {
    const size_t N = 20000;
    const size_t FORBIDDEN = 300;
    const size_t TOPK = 10;

    auto data = prepare(N, FORBIDDEN, TOPK, Mode::ExactOnly);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        auto result = data.thompson->sample(data.topk, data.forbidden);
        do_not_optimize(result);
    });

    delete data.thompson;
}

// -----------------------------------------------------
// BENCH 2: approximation path
// -----------------------------------------------------
BENCH(thompson, normal_approx_only) {
    const size_t N = 20000;
    const size_t FORBIDDEN = 300;
    const size_t TOPK = 10;

    auto data = prepare(N, FORBIDDEN, TOPK, Mode::ApproxOnly);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        auto result = data.thompson->sample(data.topk, data.forbidden);
        do_not_optimize(result);
    });

    delete data.thompson;
}

// -----------------------------------------------------
// BENCH 3: mixed real-world workload
// -----------------------------------------------------
BENCH(thompson, mixed_real_world) {
    const size_t N = 20000;
    const size_t FORBIDDEN = 300;
    const size_t TOPK = 10;

    auto data = prepare(N, FORBIDDEN, TOPK, Mode::Mixed);

    BENCH_RUN(bench, [&](size_t i, auto&) {
        auto result = data.thompson->sample(data.topk, data.forbidden);
        do_not_optimize(result);
    });

    delete data.thompson;
}

// -----------------------------------------------------
int main() {
    run_all();
}