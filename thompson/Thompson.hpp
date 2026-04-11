#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <utility>

#include "../distributions/Marsaglia.hpp"
#include "../distributions/Uniform.hpp"
#include "../collections/MinHeap.hpp"
#include "../collections/VSet.hpp"

static constexpr int threshold = 20;

struct Item {
    uint32_t id;
    int successes;
    int failures;

    bool isApprox(const int threshold) const {
        return successes >= threshold && failures >= threshold;
    }
};

struct IdWithScore {
    uint32_t id;
    double score;

    bool operator<(const IdWithScore& other) const {
        return score < other.score;
    }
};

class Thompson11 {
public:
    Thompson11(const std::vector<Item>& items)
        : set_(items.size())
    {
        // Copy  items
        items_.reserve(items.size());
        for (const auto& item : items) {
            items_.push_back(item);
        }

        border_ = std::partition(items_.begin(), items_.end(),
                    [](const Item& i) {
                        return !i.isApprox(threshold);
                }) - items_.begin();

        prepareDistribution();

        precomp_.reserve(items.size());
        for (size_t i = 0; i < items.size(); ++i) {
            const auto& item = items_[i];
            double alpha = item.successes + 1.0;
            double beta  = item.failures + 1.0;

            Precomputed p;
            p.id = item.id;
            p.successes = item.successes;
            p.failures = item.failures;

            if (i < border_) {
                p.mu = 0.0;
                p.sigma = 0.0;
            } else {
                double sum = alpha + beta;
                p.mu = alpha / sum;
                p.sigma = std::sqrt(alpha * beta / (sum * sum * (sum + 1)));
            }

            precomp_.push_back(p);
        }
    }

    std::vector<IdWithScore> sample(const size_t num, const std::vector<uint32_t>& forbidden) {
        set_.clear(forbidden);
        heap_.clear(num);
        refill();

        size_t i = 0;

        // exact sampling first
        for (i; i < border_; ++i) {
            const auto& item = precomp_[i];

            if (set_.contains(item.id)) continue;

            const double score = sample_beta(item.successes, item.failures);
            heap_.insert({item.id, float(score)});
        }

        // later approximation
        for (i; i < precomp_.size(); ++i) {
            const auto& item = precomp_[i];

            if (set_.contains(item.id)) continue;

            double z = normal();
            double score = item.mu + item.sigma * z;
            score = std::clamp(score, 0.0, 1.0);

            heap_.insert(IdWithScore{item.id, float(score)});
        }

        return heap_.getTopItems();
    }

private:
    struct alignas(32) Precomputed {
        double mu;      // 8 bytes
        double sigma;   // 8 bytes
        uint32_t id;         // 4 bytes
        int successes;  // 4 bytes
        int failures;   // 4 bytes
        char pad[4];    // padding to make 32 bytes
    };

    std::vector<Item> items_;
    std::vector<Precomputed> precomp_;

    VUniformDistribution udist_;
    VMarsaglia ndist_;

    FixedMinHeap<IdWithScore> heap_;
    VSet set_;
    size_t border_ = 0;

    inline double normal() {
        return ndist_();
    }

    inline double uniform() {
        return udist_();
    }

    double sample_gamma(double alpha) {
        const double d = alpha - 1.0/3.0;
        const double c = 1.0 / std::sqrt(9.0 * d);

        while (true) {
                double x = normal(); // standard normal
                double v = 1.0 + c * x;
                if (v <= 0) continue;
                v = v * v * v;

                double u = uniform();
                if (u < 1 - 0.0331 * x * x * x * x) return d * v;
                if (std::log(u) < 0.5 * x * x + d * (1 - v + std::log(v))) return d * v;
        }
    }

    double sample_beta(double successes, double failures) {
        double g1 = sample_gamma(successes + 1.0);
        double g2 = sample_gamma(failures + 1.0);

        return g1 / (g1 + g2);
    }

    void prepareDistribution() {
        if (items_.size() > 32) {
            udist_.resize(border_);
            ndist_.resize(items_.size());
        }
    }

    void refill() {
        if (items_.size() > 32) {
            udist_.refill();
            ndist_.refill();
        }
    }



};