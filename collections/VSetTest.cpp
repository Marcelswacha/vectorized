#include "../utils/Testing.hpp"
#include "VSet.hpp"

using namespace utils::test;

TEST(test_insert_and_size) {
    VSet vs(16);
    vs.insert(10);
    vs.insert(20);

    assertEqual(vs.size(), 2);

    vs.insert(10); // duplicate
    assertEqual(vs.size(), 2);
}

TEST(test_contains) {
    VSet vs(16);
    vs.insert(10);
    vs.insert(20);

    assertEqual(vs.contains(10), true);
    assertEqual(vs.contains(20), true);
    assertEqual(vs.contains(30), false);
}

TEST(test_resizing) {
    VSet vs(2); // small capacity to trigger resize
    vs.insert(1);
    vs.insert(2);
    vs.insert(3);

    assertEqual(vs.size(), 3);
    assertEqual(vs.contains(1), true);
    assertEqual(vs.contains(2), true);
    assertEqual(vs.contains(3), true);
}

int main() {
    run_all();
    return 0;
}