#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <cassert>

namespace utils::test {

    struct Test {
        std::string name;
        std::function<void()> func;
    };

    inline std::vector<Test>& get_tests() {
        static std::vector<Test> tests;
        return tests;
    }

#define TEST(name) \
void name(); \
struct name##_registrar { \
name##_registrar() { utils::test::get_tests().push_back({#name, name}); } \
}; \
static name##_registrar name##_instance; \
void name()

    // ---------------------------
    // Assertions (keep simple)
    // ---------------------------
    template<typename T, typename U>
    inline void assertEqual(const T& a, const U& b) {
        if (!(a == b)) {
            throw std::runtime_error("assertEqual failed");
        }
    }

    inline void assertTrue(bool v) {
        if (!v) {
            throw std::runtime_error("assertTrue failed");
        }
    }

    // ---------------------------
    // Runner with reporting
    // ---------------------------
    inline void run_all() {
        int passed = 0;
        int total = 0;

        for (const auto& t : get_tests()) {
            ++total;

            try {
                t.func();
                std::cout << t.name << ": OK\n";
                ++passed;
            }
            catch (const std::exception&) {
                std::cout << t.name << ": FAIL\n";
            }
            catch (...) {
                std::cout << t.name << ": FAIL (unknown error)\n";
            }
        }

        std::cout << "\n"
                  << passed << "/" << total << " tests passed\n";
    }

} // namespace utils::test