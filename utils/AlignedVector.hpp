#pragma once

#include <cstddef>
#include <cstdlib>

template <typename T>
class AlignedVector {
    public:
    explicit AlignedVector(const size_t capacity = 256) {
        resize(capacity);
    }

    void resize(const size_t capacity) {
        _capacity = (capacity + 31) & ~static_cast<size_t>(31); // multiple of 32
        if (_data) free(_data);

        _data = static_cast<T *>(aligned_alloc(32, _capacity * sizeof(T)));
    }

    protected:
    T* _data;
    size_t _capacity;
};
