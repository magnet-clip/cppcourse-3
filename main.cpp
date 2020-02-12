#include "profile.h"
#include "test_runner.h"

#include <algorithm>
#include <future>
#include <numeric>
#include <random>
#include <string>
#include <vector>
using namespace std;

template <typename K, typename V> class ConcurrentMap {
public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access {
    Access(V &value, mutex *m) : ref_to_value(value), _m(m) {}
    ~Access() { _m->unlock(); }
    V &ref_to_value;

  private:
    mutex *_m;
  };

  explicit ConcurrentMap(size_t bucket_count) : _bucket_count(bucket_count) {
    _maps.resize(bucket_count);
    for (size_t i = 0; i < _bucket_count; i++) {
      _locks.push_back(new mutex);
    }
  }

  ~ConcurrentMap() {
    for (size_t i = 0; i < _bucket_count; i++) {
      delete _locks[i];
    }
  }

  Access operator[](const K &key) {
    size_t index = (key < 0 ? -key : key) % _bucket_count;
    auto &map = _maps[index];
    _locks[index]->lock();
    return Access(map[key], _locks[index]);
  }

  map<K, V> BuildOrdinaryMap() {
    map<K, V> res;
    for (size_t i = 0; i < _bucket_count; i++) {
      lock_guard guard(*_locks[i]);
      res.insert(_maps[i].begin(), _maps[i].end());
    }
    return res;
  }

private:
  vector<map<K, V>> _maps;
  vector<mutex *> _locks;
  size_t _bucket_count;
};

void RunConcurrentUpdates(ConcurrentMap<int, int> &cm, size_t thread_count,
                          int key_count) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void SimpleTest() {
  ConcurrentMap<int, int> test_map(2);

  test_map[-1].ref_to_value = 1;
  test_map[-2].ref_to_value = 2;
  test_map[0].ref_to_value = 0;
  test_map[1].ref_to_value = 1;
  test_map[2].ref_to_value = 2;

  ASSERT_EQUAL(test_map[-2].ref_to_value, 2);
  ASSERT_EQUAL(test_map[-1].ref_to_value, 1);
  ASSERT_EQUAL(test_map[0].ref_to_value, 0);
  ASSERT_EQUAL(test_map[1].ref_to_value, 1);
  ASSERT_EQUAL(test_map[2].ref_to_value, 2);

  auto x = test_map.BuildOrdinaryMap();

  ASSERT_EQUAL(x[-2], 2);
  ASSERT_EQUAL(x[-1], 1);
  ASSERT_EQUAL(x[0], 0);
  ASSERT_EQUAL(x[1], 1);
  ASSERT_EQUAL(x[2], 2);

  RunConcurrentUpdates(test_map, 12, 100);
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  // const auto result = cm.BuildOrdinaryMap();
  // ASSERT_EQUAL(result.size(), key_count);
  // for (auto &[k, v] : result) {
  //   AssertEqual(v, 6, "Key = " + to_string(k));
  // }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string &s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, SimpleTest);
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}
