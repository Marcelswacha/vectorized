#include "../utils/Testing.hpp"
#include "../utils/DistributionStats.hpp"
#include "Thompson.hpp"

#include <random>

using namespace utils::test;
using namespace utils::stats;

template<typename GDistT>
struct StdBetaDistribution {
    double operator()(std::mt19937_64 &gen) {
        double x1 = g1(gen);
        double x2 = g2(gen);

        return x1 / (x1 + x2);
    }

    StdBetaDistribution(double successes, double failures) {
        g1 = GDistT(successes + 1, 1);
        g2 = GDistT(failures + 1, 1);

    }

    GDistT g1;
    GDistT g2;
};

TEST_GROUP(thompson, full_distribution_match_reference) {
    const size_t N = 20000;

    std::mt19937_64 rng(123);
    StdBetaDistribution<std::gamma_distribution<double>> ref_dist(1, 1);
    std::vector<double> ref(N), test(N);

    for (size_t i = 0; i < N; ++i) {
        ref[i] = ref_dist(rng);
    }

    std::vector<Item> items{Item{1, 1}};
    Thompson11 tom(items);
    auto f = std::vector<uint32_t>{};

    for (size_t i = 0; i < N; ++i) {
        std::cout << i << std::endl;
        auto r = tom.sample(1, f);
        test[i] = r[0].score;
    }

    auto ref_stats  = compute_stats(ref);
    auto test_stats = compute_stats(test);

    ASSERT_CLOSE(ref_stats.mean, test_stats.mean, 1e-2);
    ASSERT_CLOSE(ref_stats.variance, test_stats.variance, 1e-2);
    ASSERT_CLOSE(ref_stats.skewness, test_stats.skewness, 1e-1);

    ASSERT_CLOSE(ref_stats.q01, test_stats.q01, 5e-2);
    ASSERT_CLOSE(ref_stats.q25, test_stats.q25, 2e-2);
    ASSERT_CLOSE(ref_stats.q50, test_stats.q50, 2e-2);
    ASSERT_CLOSE(ref_stats.q75, test_stats.q75, 2e-2);
    ASSERT_CLOSE(ref_stats.q99, test_stats.q99, 5e-2);
}

int main() {
    run_all("thompson");
    return 0;
}