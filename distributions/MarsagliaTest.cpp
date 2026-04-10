#include "../utils/Testing.hpp"
#include "../utils/DistributionStats.hpp"
#include "Marsaglia.hpp"

#include <random>

using namespace utils::test;
using namespace utils::stats;

TEST_GROUP(marsaglia, full_distribution_match_reference) {
    const size_t N = 200000;

    std::mt19937_64 rng(123);
    std::normal_distribution<double> ref_dist(0.0, 1.0);

    VMarsaglia dist(N);

    std::vector<double> ref(N), test(N);

    for (size_t i = 0; i < N; ++i) {
        ref[i] = ref_dist(rng);
    }

    dist.refill();

    for (size_t i = 0; i < N; ++i) {
        test[i] = dist();
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
    run_all("marsaglia");
    return 0;
}