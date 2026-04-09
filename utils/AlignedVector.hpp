#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

template <typename T>
class AlignedVector {
public:
    explicit AlignedVector(const size_t capacity = 256)
        : _data(nullptr), _capacity(0)
    {
        resize(capacity);
    }

    virtual ~AlignedVector() {
        if (_data) free(_data);
    }

    void resize(const size_t capacity) {
        // Round capacity up to nearest multiple of 32
        size_t newCapacity = (capacity + 31) & ~static_cast<size_t>(31);
        if (newCapacity == 0) newCapacity = 32;

        T* newData = static_cast<T*>(aligned_alloc(32, newCapacity * sizeof(T)));
        if (!newData) throw std::bad_alloc();

        if (_data) {
            // Copy old data safely
            std::memcpy(newData, _data, std::min(_capacity, newCapacity) * sizeof(T));
            free(_data);
        }

        _data = newData;
        _capacity = newCapacity;
    }

protected:
    T* _data;
    size_t _capacity;
};