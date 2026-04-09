#include <unordered_set>
#include <vector>
#include <chrono>
#include <iostream>
#include "VSet.hpp"

int main() {
    const int N = 1000000;
    std::vector<int> data(N);
    for (int i = 0; i < N; ++i) data[i] = i;

    // Benchmark VSet
    VSet vs(N);
    auto start = std::chrono::high_resolution_clock::now();
    for (auto x : data) vs.insert(x);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "VSet time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

    // Benchmark std::unordered_set
    std::unordered_set<int> uset;
    start = std::chrono::high_resolution_clock::now();
    for (auto x : data) uset.insert(x);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::unordered_set time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}