#include "../utils/Testing.hpp"
#include "VSet.hpp"

#include <unordered_set>
#include <vector>
#include <random>

using namespace utils::test;

TEST_GROUP(vset, basic_insert_and_size) {
    VSet vs(16);

    vs.insert(10);
    vs.insert(20);

    ASSERT_EQ(vs.size(), 2);

    vs.insert(10);
    ASSERT_EQ(vs.size(), 2);
}

TEST_GROUP(vset, contains_basic_and_missing) {
    VSet vs(16);

    vs.insert(10);
    vs.insert(20);

    ASSERT_TRUE(vs.contains(10));
    ASSERT_TRUE(vs.contains(20));

    ASSERT_FALSE(vs.contains(30));
    ASSERT_FALSE(vs.contains(999999));
}

TEST_GROUP(vset, collision_handling) {
    VSet vs(64);

    for (uint32_t i = 1; i <= 1000; ++i) {
        vs.insert(i);
    }

    ASSERT_EQ(vs.size(), 1000);

    for (uint32_t i = 1; i <= 1000; ++i) {
        ASSERT_TRUE(vs.contains(i));
    }
}

TEST_GROUP(vset, resize_stress) {
    VSet vs(8);

    const int N = 5000;

    for (int i = 1; i <= N; ++i) {
        vs.insert(i);
    }

    ASSERT_EQ(vs.size(), N);

    for (int i = 1; i <= N; ++i) {
        ASSERT_TRUE(vs.contains(i));
    }
}

TEST_GROUP(vset, clear_behavior) {
    VSet vs(64);

    for (int i = 1; i <= 100; ++i) {
        vs.insert(i);
    }

    ASSERT_EQ(vs.size(), 100);

    vs.clear();

    ASSERT_EQ(vs.size(), 0);

    for (int i = 1; i <= 100; ++i) {
        ASSERT_FALSE(vs.contains(i));
    }

    vs.insert(42);
    ASSERT_TRUE(vs.contains(42));
    ASSERT_EQ(vs.size(), 1);
}

TEST_GROUP(vset, rehash_integrity_after_resize) {
    VSet vs(4);

    std::vector<uint32_t> values;

    for (uint32_t i = 1; i <= 2000; ++i) {
        values.push_back(i * 2654435761u);
        vs.insert(values.back());
    }

    for (auto v : values) {
        ASSERT_TRUE(vs.contains(v));
    }
}

TEST_GROUP(vset, randomized_reference_model) {
    VSet vs(128);
    std::unordered_set<uint32_t> ref;

    std::mt19937 rng(123);
    std::uniform_int_distribution<uint32_t> dist;

    for (int i = 0; i < 10000; ++i) {
        uint32_t x = dist(rng);

        vs.insert(x);
        ref.insert(x);

        ASSERT_EQ(vs.size(), ref.size());

        if (i % 50 == 0) {
            for (auto v : ref) {
                ASSERT_TRUE(vs.contains(v));
            }
        }
    }
}

TEST_GROUP(vset, heavy_duplicates) {
    VSet vs(128);

    for (int i = 0; i < 1000; ++i) {
        vs.insert(42);
    }

    ASSERT_EQ(vs.size(), 1);
    ASSERT_TRUE(vs.contains(42));
}

TEST_GROUP(vset, zero_value_behavior) {
    VSet vs(64);

    vs.insert(0);

    ASSERT_FALSE(vs.contains(0));
}

int main() {
    run_all("vset");
    return 0;
}