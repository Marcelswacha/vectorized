#include "../utils/Testing.hpp"
#include "Uniform.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

using namespace utils::test;

static double mean(const std::vector<double>& v) {
    double sum = 0.0;
    for (double x : v) sum += x;
    return sum / v.size();
}

static double variance(const std::vector<double>& v, double m) {
    double sum = 0.0;
    for (double x : v) {
        double d = x - m;
        sum += d * d;
    }
    return sum / v.size();
}

static double skewness(const std::vector<double>& v, double m, double var) {
    double m3 = 0.0;
    for (double x : v) {
        double d = x - m;
        m3 += d * d * d;
    }
    m3 /= v.size();

    double sigma = std::sqrt(var);
    return m3 / (sigma * sigma * sigma);
}

static double quantile(std::vector<double> v, double p) {
    std::sort(v.begin(), v.end());
    size_t idx = static_cast<size_t>(p * (v.size() - 1));
    return v[idx];
}

TEST_GROUP(uniform, full_distribution_match_reference) {
    const size_t N = 200000;

    std::mt19937_64 rng(123);
    std::uniform_real_distribution<double> ref_dist(0.0, 1.0);

    VUniformDistribution dist(N);

    std::vector<double> ref(N), test(N);

    for (size_t i = 0; i < N; ++i) {
        ref[i] = ref_dist(rng);
    }

    dist.refill();
    for (size_t i = 0; i < N; ++i) {
        test[i] = dist();
    }

    double m_ref = mean(ref);
    double m_test = mean(test);

    double v_ref = variance(ref, m_ref);
    double v_test = variance(test, m_test);

    double s_ref = skewness(ref, m_ref, v_ref);
    double s_test = skewness(test, m_test, v_test);

    ASSERT_CLOSE(m_ref, m_test, 1e-2);
    ASSERT_CLOSE(v_ref, v_test, 1e-2);
    ASSERT_CLOSE(s_ref, s_test, 1e-1);

    // quantiles
    const double qps[] = {0.01, 0.25, 0.50, 0.75, 0.99};

    for (double p : qps) {
        double q_ref = quantile(ref, p);
        double q_test = quantile(test, p);

        ASSERT_CLOSE(q_ref, q_test, 1e-2);
    }
}

int main() {
    run_all("uniform");
    return 0;
}