#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T, typename Id> struct rec {
  T value;
  Id id;
  int priority;

  rec(T &&v, Id i, int p) : value(move(v)), id(i), priority(p) {}
};

template <typename T, typename Id>
bool operator<(const rec<T, Id> &a, const rec<T, Id> &b) {
  return tie(a.priority, a.id) > tie(b.priority, b.id);
}

template <typename T> class PriorityCollection {
public:
  using Id = int;
  Id _last = 0;

private:
  using Item = rec<T, Id>;
  using Pool = set<Item>;
  using PoolIterator = typename set<Item>::iterator;

  Pool _pool;
  map<Id, PoolIterator> _references;

public:
  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    auto new_id = _last++;
    auto inserted = _pool.insert({move(object), new_id, 0});
    _references[new_id] = inserted.first;
    return new_id;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for (auto it = range_begin; it != range_end; it++) {
      *ids_begin++ = Add(*it);
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const { return _references.count(id); }

  // Получить объект по идентификатору
  const T &Get(Id id) const { return (_references.at(id))->first; }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    auto iterator = _references[id];
    auto handle = _pool.extract(iterator);
    handle.value().priority++;
    _pool.insert(move(handle));
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T &, int> GetMax() const;

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    auto item = _pool.begin();
    auto handle = _pool.extract(item);
    _references.erase(handle.value().id);
    return {move(handle.value().value), handle.value().priority};
  }
};

class StringNonCopyable : public string {
public:
  using string::string; // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable &) = delete;
  StringNonCopyable(StringNonCopyable &&) = default;
  StringNonCopyable &operator=(const StringNonCopyable &) = delete;
  StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}