#pragma once

#include <cmath>
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <exception>

namespace utils::test {

    struct TestContext {
        std::string test_name;

        [[noreturn]] void fail(const std::string& msg,
                               const char* file,
                               int line) const {
            std::ostringstream oss;
            oss << file << ":" << line
                << " [" << test_name << "] " << msg;
            throw std::runtime_error(oss.str());
        }

        template<typename T, typename U>
        void assertEqual(const T& a, const U& b,
                         const char* file,
                         int line) const {
            if (!(a == b)) {
                std::ostringstream oss;
                oss << "assertEqual failed: "
                    << a << " != " << b;
                fail(oss.str(), file, line);
            }
        }

        void assertTrue(bool v,
                        const char* file,
                        int line) const {
            if (!v) {
                fail("assertTrue failed", file, line);
            }
        }

        void assertFalse(bool v,
                const char* file,
                int line) const {
            if (v) {
                fail("assertTrue failed", file, line);
            }
        }

        void assertClose(double a,
                  double b,
                  double tol,
                  const char* file,
                  int line) const
        {
            double diff = std::fabs(a - b);

            if (diff > tol) {
                std::ostringstream oss;
                oss << "ASSERT_CLOSE failed: |"
                    << a << " - " << b << "| = "
                    << diff << " > " << tol;

                fail(oss.str().c_str(), file, line);
            }
        }
    };

    struct Test {
        std::string name;
        std::string group;
        std::function<void(TestContext&)> func;
    };

    inline std::vector<Test>& registry() {
        static std::vector<Test> tests;
        return tests;
    }

    // ---------------------------
    // Registration macro
    // ---------------------------
#define TEST(name) TEST_GROUP(default, name)

#define TEST_GROUP(group, name)                     \
    void name(utils::test::TestContext&);          \
    struct name##_registrar {                      \
        name##_registrar() {                       \
            utils::test::registry().push_back({    \
                #name, #group, name                \
            });                                   \
        }                                          \
    };                                             \
    static name##_registrar name##_instance;       \
    void name(utils::test::TestContext& t)

    // ---------------------------
    // Assertion macros (nice UX)
    // ---------------------------
#define ASSERT_EQ(a, b) t.assertEqual((a), (b), __FILE__, __LINE__)
#define ASSERT_TRUE(v)  t.assertTrue((v), __FILE__, __LINE__)
#define ASSERT_FALSE(v)  t.assertFalse((v), __FILE__, __LINE__)
#define ASSERT_CLOSE(a, b, tol) t.assertClose((a), (b), (tol), __FILE__, __LINE__)

    // ---------------------------
    // Runner
    // ---------------------------
    inline void run_all(const std::string& filter_group = "") {
        int passed = 0;
        int total = 0;

        for (const auto& test : registry()) {

            if (!filter_group.empty() && test.group != filter_group)
                continue;

            ++total;
            TestContext ctx{test.name};

            try {
                test.func(ctx);
                std::cout << "[OK]   " << test.group
                          << "::" << test.name << "\n";
                ++passed;
            }
            catch (const std::exception& e) {
                std::cout << "[FAIL] " << test.group
                          << "::" << test.name << "\n";
                std::cout << "       " << e.what() << "\n";
            }
            catch (...) {
                std::cout << "[FAIL] " << test.group
                          << "::" << test.name
                          << " (unknown error)\n";
            }
        }

        std::cout << "\n"
                  << passed << "/" << total
                  << " tests passed\n";
    }

} // namespace utils::test