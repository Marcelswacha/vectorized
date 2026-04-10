#include "../utils/Testing.hpp"
#include "MinHeap.hpp"

#include <vector>
#include <algorithm>
#include <random>

using namespace utils::test;

using Item = int;

static std::vector<Item> getTopKReference(const std::vector<Item>& data, size_t k) {
    std::vector<Item> ref = data;

    if (ref.size() > k) {
        std::nth_element(ref.begin(), ref.end() - k, ref.end());
        ref.erase(ref.begin(), ref.end() - k);
    }

    std::sort(ref.begin(), ref.end());
    return ref;
}

static std::vector<Item> normalize(std::vector<Item> v) {
    std::sort(v.begin(), v.end());
    return v;
}

bool assertVectorsEqual(const std::vector<Item>& expected, const std::vector<Item>& actual) {
    if (expected.size() != actual.size()) {
        return false;
    }

    for (size_t i = 0; i < expected.size(); ++i) {
        if (expected[i] != actual[i]) {
            return false;
        }
    }

    return true;
}

TEST_GROUP(minheap, basic_insert_and_size) {
    FixedMinHeap<Item> heap(4);

    heap.insert(10);
    heap.insert(20);

    ASSERT_EQ(heap.size(), 2);
}

TEST_GROUP(minheap, respects_capacity) {
    FixedMinHeap<Item> heap(3);

    heap.insert(10);
    heap.insert(20);
    heap.insert(30);
    heap.insert(40); // should evict 10

    ASSERT_EQ(heap.size(), 3);

    auto res = normalize(heap.getTopItems());

    ASSERT_TRUE(std::find(res.begin(), res.end(), 10) == res.end());
    ASSERT_TRUE(std::find(res.begin(), res.end(), 40) != res.end());
}

TEST_GROUP(minheap, keeps_top_k_elements) {
    FixedMinHeap<Item> heap(5);

    std::vector<Item> input = {1, 9, 3, 7, 5, 8, 2, 6, 4};

    for (auto x : input) {
        heap.insert(x);
    }

    auto res = normalize(heap.getTopItems());
    auto ref = getTopKReference(input, 5);

    ASSERT_TRUE(assertVectorsEqual(res, ref));
}

TEST_GROUP(minheap, clear_behavior) {
    FixedMinHeap<Item> heap(8);

    for (int i = 0; i < 100; ++i) {
        heap.insert(i);
    }

    ASSERT_EQ(heap.size(), 8);

    heap.clear();

    ASSERT_EQ(heap.size(), 0);

    heap.insert(42);
    ASSERT_EQ(heap.size(), 1);

    auto res = heap.getTopItems();
    ASSERT_EQ(res[0], 42);
}

TEST_GROUP(minheap, clear_with_resize) {
    FixedMinHeap<Item> heap(4);

    for (int i = 0; i < 100; ++i) {
        heap.insert(i);
    }

    heap.clear(16);

    ASSERT_EQ(heap.size(), 0);

    for (int i = 0; i < 20; ++i) {
        heap.insert(i);
    }

    ASSERT_EQ(heap.size(), 16);
}

TEST_GROUP(minheap, duplicate_handling) {
    FixedMinHeap<Item> heap(4);

    for (int i = 0; i < 10; ++i) {
        heap.insert(5);
    }

    ASSERT_EQ(heap.size(), 4);

    auto res = heap.getTopItems();
    for (auto x : res) {
        ASSERT_EQ(x, 5);
    }
}

TEST_GROUP(minheap, increasing_sequence) {
    FixedMinHeap<Item> heap(5);

    for (int i = 1; i <= 100; ++i) {
        heap.insert(i);
    }

    auto res = normalize(heap.getTopItems());
    auto ref = getTopKReference(
        std::vector<Item>(100, 0), 5
    );

    std::vector<Item> expected = {96,97,98,99,100};

    ASSERT_TRUE(assertVectorsEqual(res, expected));
}

TEST_GROUP(minheap, decreasing_sequence) {
    FixedMinHeap<Item> heap(5);

    for (int i = 100; i >= 1; --i) {
        heap.insert(i);
    }

    auto res = normalize(heap.getTopItems());

    std::vector<Item> expected = {96,97,98,99,100};

    ASSERT_TRUE(assertVectorsEqual(res, expected));
}

TEST_GROUP(minheap, randomized_reference_model) {
    FixedMinHeap<Item> heap(32);

    std::vector<Item> data;

    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist(0, 1000000);

    for (int i = 0; i < 5000; ++i) {
        int x = dist(rng);

        data.push_back(x);
        heap.insert(x);

        if (i % 50 == 0) {
            auto res = normalize(heap.getTopItems());
            auto ref = getTopKReference(data, 32);

            ASSERT_TRUE(assertVectorsEqual(res, ref));
        }
    }
}

TEST_GROUP(minheap, stress_large_input) {
    FixedMinHeap<Item> heap(64);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist;

    std::vector<Item> data;

    for (int i = 0; i < 20000; ++i) {
        int x = dist(rng);

        data.push_back(x);
        heap.insert(x);
    }

    auto res = normalize(heap.getTopItems());
    auto ref = getTopKReference(data, 64);

    ASSERT_TRUE(assertVectorsEqual(res, ref));
}

int main() {
    run_all("minheap");
    return 0;
}