#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <chrono>
#include <algorithm>

namespace utils::bench {

    // -----------------------------------------------------
    // Prevent optimization
    // -----------------------------------------------------
    template<typename T>
    inline void do_not_optimize(const T& value) {
#if defined(__GNUC__) || defined(__clang__)
        asm volatile("" : : "g"(value) : "memory");
#else
        (void)value;
#endif
    }

    // -----------------------------------------------------
    // Benchmark result
    // -----------------------------------------------------
    struct Result {
        double total_ms = 0.0;
        double avg_ns_per_op = 0.0;
        size_t iterations = 0;
    };

    // -----------------------------------------------------
    // Context
    // -----------------------------------------------------
    struct BenchmarkContext {

        size_t max_iterations = 10'000'000;
        double max_seconds = 1.0;

        size_t warmup_iterations = 10'000;
        size_t chunk_size = 1000;

        template<typename F>
        Result run(F&& func) {
            using namespace std::chrono;

            // -------------------------
            // WARMUP
            // -------------------------
            for (size_t i = 0; i < warmup_iterations; ++i) {
                func(i, *this);
            }

            // -------------------------
            // MEASURE
            // -------------------------
            size_t total_iters = 0;
            double total_ns = 0.0;

            auto global_start = high_resolution_clock::now();

            while (true) {

                if (total_iters >= max_iterations)
                    break;

                auto now = high_resolution_clock::now();
                double elapsed = duration<double>(now - global_start).count();

                if (elapsed >= max_seconds)
                    break;

                auto chunk_start = high_resolution_clock::now();

                for (size_t i = 0; i < chunk_size; ++i) {
                    func(total_iters + i, *this);
                }

                auto chunk_end = high_resolution_clock::now();

                total_iters += chunk_size;

                total_ns += duration<double, std::nano>(chunk_end - chunk_start).count();
            }

            Result r;
            r.iterations = total_iters;
            r.total_ms = total_ns / 1e6;
            r.avg_ns_per_op = total_iters ? (total_ns / total_iters) : 0.0;

            return r;
        }

        void report(const Result& r) const {
            std::cout << "  avg per op:     " << r.avg_ns_per_op << " ns\n";
        }
    };

    // -----------------------------------------------------
    // Registry
    // -----------------------------------------------------
    struct Benchmark {
        std::string name;
        std::string group;
        std::function<void(BenchmarkContext&)> func;
    };

    inline std::vector<Benchmark>& registry() {
        static std::vector<Benchmark> benchmarks;
        return benchmarks;
    }

    // -----------------------------------------------------
    // MACROS
    // -----------------------------------------------------
#define BENCH(group, name)                                                \
    static void group##_##name(utils::bench::BenchmarkContext& bench);     \
    struct group##_##name##_reg {                                         \
        group##_##name##_reg() {                                          \
            utils::bench::registry().push_back({                          \
                #name, #group, group##_##name                             \
            });                                                           \
        }                                                                 \
    };                                                                    \
    static group##_##name##_reg group##_##name##_instance;               \
    static void group##_##name(utils::bench::BenchmarkContext& bench)

#define BENCH_RUN(ctx, lambda) \
    do { auto r = ctx.run(lambda); ctx.report(r); } while (0)

    // -----------------------------------------------------
    // Runner
    // -----------------------------------------------------
    inline void run_all(const std::string& filter = "") {
        for (const auto& b : registry()) {

            if (!filter.empty() && b.group != filter)
                continue;

            std::cout << "[BENCH] " << b.group << "::" << b.name << "\n";

            BenchmarkContext ctx;
            b.func(ctx);

            std::cout << "\n";
        }
    }

} // namespace utils::bench