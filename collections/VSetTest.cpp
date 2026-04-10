#include "../utils/Testing.hpp"
#include "VSet.hpp"

using namespace utils::test;

TEST_GROUP(vset, test_insert_and_size) {
    VSet vs(16);
    vs.insert(10);
    vs.insert(20);

    ASSERT_EQ(vs.size(), 2);

    vs.insert(10); // duplicate
    ASSERT_EQ(vs.size(), 2);
}

TEST_GROUP(vset, test_contains) {
    VSet vs(16);
    vs.insert(10);
    vs.insert(20);

    ASSERT_TRUE(vs.contains(10));
    ASSERT_TRUE(vs.contains(20));
    ASSERT_FALSE(vs.contains(30));
}

TEST_GROUP(vset, test_resizing) {
    VSet vs(2); // small capacity to trigger resize
    vs.insert(1);
    vs.insert(2);
    vs.insert(3);

    ASSERT_EQ(vs.size(), 3);
    ASSERT_TRUE(vs.contains(1));
    ASSERT_TRUE(vs.contains(2));
    ASSERT_TRUE(vs.contains(3));
}

int main() {
    run_all("vset");
    return 0;
}