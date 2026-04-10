#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <chrono>
#include <sstream>

namespace utils::bench {

    // ----------------------------------------
    // Prevent compiler optimizations
    // ----------------------------------------
    template<typename T>
    inline void do_not_optimize(const T& value) {
#if defined(__clang__) || defined(__GNUC__)
        asm volatile("" : : "g"(value) : "memory");
#else
        (void)value;
#endif
    }

    // ----------------------------------------
    // Benchmark result
    // ----------------------------------------
    struct Result {
        double total_ms = 0.0;
        double avg_ns = 0.0;
        size_t iterations = 0;
    };

    // ----------------------------------------
    // Benchmark context (core engine)
    // ----------------------------------------
    struct BenchmarkContext {

        size_t iterations = 100000;

        template<typename F>
        Result run(F&& func) {
            using namespace std::chrono;

            // Warmup (important for stable results)
            for (size_t i = 0; i < 1000 && i < iterations; ++i) {
                func(i);
            }

            auto start = high_resolution_clock::now();

            for (size_t i = 0; i < iterations; ++i) {
                func(i);
            }

            auto end = high_resolution_clock::now();

            double total_ns =
                duration<double, std::nano>(end - start).count();

            Result r;
            r.total_ms = total_ns / 1e6;
            r.avg_ns   = total_ns / iterations;
            r.iterations = iterations;

            return r;
        }

        void report(const Result& r) const {
            std::cout << "  iterations: " << r.iterations << "\n";
            std::cout << "  total:      " << r.total_ms << " ms\n";
            std::cout << "  avg:        " << r.avg_ns << " ns\n";
        }
    };

    // ----------------------------------------
    // Benchmark registration
    // ----------------------------------------
    struct Benchmark {
        std::string name;
        std::string group;
        std::function<void(BenchmarkContext&)> func;
    };

    inline std::vector<Benchmark>& registry() {
        static std::vector<Benchmark> benchmarks;
        return benchmarks;
    }

    // ----------------------------------------
    // Macro API
    // ----------------------------------------
#define BENCH(group, name)                             \
    void name(utils::bench::BenchmarkContext& bench);  \
    struct name##_registrar {                          \
        name##_registrar() {                           \
            utils::bench::registry().push_back({       \
                #name, #group, name                    \
            });                                        \
        }                                              \
    };                                                 \
    static name##_registrar name##_instance;           \
    void name(utils::bench::BenchmarkContext& bench)

#define BENCH_RUN(ctx, lambda)                         \
    do {                                               \
        auto result = ctx.run(lambda);                 \
        ctx.report(result);                            \
    } while (0)

    // ----------------------------------------
    // Runner
    // ----------------------------------------
    inline void run_all(const std::string& filter_group = "") {

        for (const auto& b : registry()) {

            if (!filter_group.empty() && b.group != filter_group)
                continue;

            std::cout << "[BENCH] "
                      << b.group << "::" << b.name << "\n";

            BenchmarkContext ctx;

            try {
                b.func(ctx);
            }
            catch (const std::exception& e) {
                std::cout << "  ERROR: " << e.what() << "\n";
            }
            catch (...) {
                std::cout << "  ERROR: unknown exception\n";
            }

            std::cout << "\n";
        }
    }

} // namespace utils::bench