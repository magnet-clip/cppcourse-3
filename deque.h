#pragma once

#include <vector>

template <typename T> class Deque {
public:
  Deque() {}
  bool Empty() const;
  size_t Size() const;
  T &operator[](size_t index);
  const T &operator[](size_t index) const;
  const T &At(size_t index) const;
  T &At(size_t index);
  const T &Front() const;
  T &Front();
  const T &Back() const;
  T &Back();

  void PushFront(const T &item);
  void PushBack(const T &item);

private:
  inline bool IsInRange(size_t index) const;
  inline bool IsFront(size_t index) const;

  std::vector<T> _front;
  std::vector<T> _back;
};

template <typename T> inline bool Deque<T>::IsInRange(size_t index) const {
  return index < _front.size() + _back.size();
}

template <typename T> inline bool Deque<T>::IsFront(size_t index) const {
  return index > _front.size();
}

template <typename T> bool Deque<T>::Empty() const {
  return _front.empty() && _back.empty();
}

template <typename T> size_t Deque<T>::Size() const {
  return _front.size() + _back.size();
}