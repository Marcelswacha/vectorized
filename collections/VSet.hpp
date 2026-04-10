#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>

#include "../utils/AlignedVector.hpp"

class VSet : public AlignedVector<uint32_t> {
    static constexpr uint32_t empty = 0u;
    static constexpr double loadFactor = 0.5;

    public:
    explicit VSet(const size_t capacity = 256)
        : AlignedVector<uint32_t>(capacity)
        , _size(0)
    {
        // Initialize array to empty
        for (size_t i = 0; i < _capacity; ++i) {
            _data[i] = empty;
        }
    }

    void insert(uint32_t element) {
        size_t idx = hash(element) % _capacity;

        while (_data[idx] != empty) {
            if (_data[idx] == element) return;
            idx = (idx + 1) % _capacity;
        }

        _data[idx] = element;
        ++_size;

        maybeResize();
    }

    bool contains(uint32_t element) const {
        size_t idx = hash(element) % _capacity;
        while (_data[idx] != empty) {
            if (_data[idx] == element) return true;
            idx = (idx + 1) % _capacity;
        }
        return false;
    }

    size_t size() const { return _size; }

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
        if (_capacity == 0) return;

        double load = static_cast<double>(_size) / static_cast<double>(_capacity);
        if (load > loadFactor) {
            size_t newCapacity = _capacity * 2;
            resize(newCapacity);

            for (size_t i = _size; i < _capacity; ++i) {
                _data[i] = empty;
            }
        }
    }
};