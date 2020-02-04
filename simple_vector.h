#pragma once

#include <cstdlib>

template <typename T> class SimpleVector {
public:
  SimpleVector() = default;

  explicit SimpleVector(size_t size) {
    _capacity = size;
    _data = new T[_capacity];
    _size = size;
    _end = _data + size;
  }

  ~SimpleVector() { delete[] _data; }

  T &operator[](size_t index) { return _data[index]; };

  T *begin() { return _data; }
  T *end() { return _end; };

  size_t Size() const { return _size; };
  size_t Capacity() const { return _capacity; };

  void PushBack(const T &value) {
    if (_data == nullptr) {
      _capacity = 1;
      _data = new T[_capacity];
      _data[_size++] = value;
      _end = _data + _size;
      return;
    }

    if (_size == _capacity) {
      _capacity *= 2;
      T *data = new T[_capacity];
      for (size_t i = 0; i < _size; i++) {
        data[i] = _data[i];
      }
      delete[] _data;
      _data = data;
    }
    _data[_size++] = value;
    _end = _data + _size;
  }

private:
  size_t _size = 0;
  size_t _capacity = 0;
  T *_data = nullptr;
  T *_end = nullptr;
};
