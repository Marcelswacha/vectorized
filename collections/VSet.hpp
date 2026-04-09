#pragma once
#include <cstdint>

#include "../utils/AlignedVector.hpp"

class VSet: AlignedVector<uint32_t> {
    static constexpr uint32_t empty = 0u;
    static constexpr float loadFactor = 0.5;

public:
    explicit VSet(const size_t capacity = 256) : AlignedVector<uint32_t>(capacity) {}

    void insert(const uint32_t element) {
        size_t idx = hash(element) % _capacity;
        while (_data[idx] != empty) {
            idx = (idx + 1) % _capacity;
            // we must find something, because we keep resizing
        }
        _data[idx] = element;
        ++_size;

        maybeResize();
    }

private:
    size_t _size;

    static inline uint32_t hash(uint32_t x) {
        x ^= x >> 16;
        x *= 0x7feb352d;
        x ^= x >> 15;
        x *= 0x846ca68b;
        x ^= x >> 16;
        return x;
    }

    void maybeResize() {
        if (_size / (float)_capacity > loadFactor) {
            resize(_capacity * 2);
        }
    }
};