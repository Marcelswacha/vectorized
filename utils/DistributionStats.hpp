#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

namespace utils::stats {
    inline double mean(const std::vector<double>& v) {
        double sum = 0.0;
        for (double x : v) sum += x;
        return sum / v.size();
    }

    inline double variance(const std::vector<double>& v, double m) {
        double sum = 0.0;
        for (double x : v) {
            double d = x - m;
            sum += d * d;
        }
        return sum / v.size();
    }

    inline double skewness(const std::vector<double>& v, double m, double var) {
        double m3 = 0.0;

        for (double x : v) {
            double d = x - m;
            m3 += d * d * d;
        }

        m3 /= v.size();

        double sigma = std::sqrt(var);
        return m3 / (sigma * sigma * sigma);
    }

    inline double quantile(std::vector<double> v, double p) {
        std::sort(v.begin(), v.end());
        size_t idx = static_cast<size_t>(p * (v.size() - 1));
        return v[idx];
    }

    struct DistributionStats {
        double mean;
        double variance;
        double skewness;

        double q01;
        double q25;
        double q50;
        double q75;
        double q99;
    };

    inline DistributionStats compute_stats(std::vector<double> v) {
        DistributionStats s{};

        s.mean = mean(v);
        s.variance = variance(v, s.mean);
        s.skewness = skewness(v, s.mean, s.variance);

        s.q01 = quantile(v, 0.01);
        s.q25 = quantile(v, 0.25);
        s.q50 = quantile(v, 0.50);
        s.q75 = quantile(v, 0.75);
        s.q99 = quantile(v, 0.99);

        return s;
    }

} // namespace utils::stats