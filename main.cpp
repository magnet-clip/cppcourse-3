#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
using namespace std;

template <class T>
class ObjectPool {
 public:
  T* Allocate();
  T* TryAllocate();

  void Deallocate(T* object);

  ~ObjectPool();

 private:
  queue<T*> _freed;
  set<T*> _allocated;
};

template <class T>
T* ObjectPool<T>::Allocate() {
  T* res;
  if (!_freed.empty()) {
    res = _freed.front();
    _freed.pop();
  } else {
    res = new T;
  }
  _allocated.insert(res);
  return res;
}

template <class T>
T* ObjectPool<T>::TryAllocate() {
  if (!_freed.empty()) {
    auto res = _freed.front();
    _freed.pop();
    _allocated.insert(res);
    return res;
  } else {
    return nullptr;
  }
}

template <class T>
void ObjectPool<T>::Deallocate(T* object) {
  if (_allocated.count(object)) {
    _allocated.erase(object);
    _freed.push(object);
  } else {
    throw invalid_argument("object not in pool");
  }
}

template <class T>
ObjectPool<T>::~ObjectPool() {
  while (!_freed.empty()) {
    auto obj = _freed.front();
    delete obj;
    _freed.pop();
  }

  for (auto obj : _allocated) {
    delete obj;
  }
}

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
