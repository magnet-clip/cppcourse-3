#pragma once

#include <stdexcept>
#include <vector>

template <typename T> class Deque {
public:
  Deque() {}
  bool Empty() const;
  size_t Size() const;

  T &operator[](size_t index);
  const T &operator[](size_t index) const;
  T &At(size_t index);
  const T &At(size_t index) const;

  const T &Front() const;
  T &Front();
  const T &Back() const;
  T &Back();

  void PushFront(const T &item);
  void PushBack(const T &item);

private:
  inline bool IsInRange(size_t index) const;
  inline bool IsFront(size_t index) const;

  // input: index in total array, output: index in front
  inline size_t FrontPosition(size_t index) const;

  // input: index in total array, output: index in back
  inline size_t BackPosition(size_t index) const;

  std::vector<T> _front; // reversed
  std::vector<T> _back;
};

template <typename T> inline bool Deque<T>::IsInRange(size_t index) const {
  return index < _front.size() + _back.size();
}

template <typename T> inline bool Deque<T>::IsFront(size_t index) const {
  return index < _front.size();
}

template <typename T>
inline size_t Deque<T>::FrontPosition(size_t index) const {
  return _front.size() - index - 1;
}

template <typename T> inline size_t Deque<T>::BackPosition(size_t index) const {
  return index - _front.size();
}

template <typename T> bool Deque<T>::Empty() const {
  return _front.empty() && _back.empty();
}

template <typename T> size_t Deque<T>::Size() const {
  return _front.size() + _back.size();
}

template <typename T> void Deque<T>::PushFront(const T &item) {
  _front.push_back(item);
}

template <typename T> void Deque<T>::PushBack(const T &item) {
  _back.push_back(item);
}

template <typename T> const T &Deque<T>::Front() const {
  return _front.empty() ? *(_back.cbegin()) : _front.back();
}

template <typename T> T &Deque<T>::Front() {
  return _front.empty() ? *(_back.begin()) : _front.back();
}

template <typename T> const T &Deque<T>::Back() const {
  return _back.empty() ? *(_front.cbegin()) : _back.back();
}

template <typename T> T &Deque<T>::Back() {
  return _back.empty() ? *(_front.begin()) : _back.back();
}

template <typename T> T &Deque<T>::operator[](size_t index) {
  if (IsFront(index)) {
    return _front[FrontPosition(index)];
  } else {
    return _back[BackPosition(index)];
  }
}

template <typename T> const T &Deque<T>::operator[](size_t index) const {
  if (IsFront(index)) {
    return _front[FrontPosition(index)];
  } else {
    return _back[BackPosition(index)];
  }
}

template <typename T> T &Deque<T>::At(size_t index) {
  if (IsInRange(index))
    return this->operator[](index);

  throw std::out_of_range("");
}

template <typename T> const T &Deque<T>::At(size_t index) const {
  if (IsInRange(index))
    return this->operator[](index);

  throw std::out_of_range("");
}