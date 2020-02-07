#include "test_runner.h"

#include <array>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <string>
using namespace std;

const int MAX_QUEUE = 100'001;

class PosManager {
  using Position = list<int>::iterator;
  array<Position, MAX_QUEUE> _positions;
  list<int> _players;

 public:
  PosManager() { _positions.fill(_players.end()); }

  void Insert(int number, int prev) {
    Position prev_position = _positions[prev];
    Position curr_position = _players.insert(prev_position, number);
    _positions[number] = curr_position;
  }

  const list<int>& Get() const { return _players; }
};

void TestAsInSample() {
  PosManager pm;
  pm.Insert(42, 0);
  pm.Insert(17, 42);
  pm.Insert(13, 0);
  pm.Insert(123, 42);
  pm.Insert(5, 13);

  const auto res = pm.Get();
  const auto vec = vector<int>(res.begin(), res.end());
  ASSERT_EQUAL(vec, vector<int>({17, 123, 42, 5, 13}));
}

void TestEmpty() {
  PosManager pm;

  const auto res = pm.Get();
  const auto vec = vector<int>(res.begin(), res.end());
  ASSERT_EQUAL(vec, vector<int>({}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEmpty);
  RUN_TEST(tr, TestAsInSample);

  int num;
  cin >> num;
  PosManager pm;

  while (num-- > 0) {
    int cur, prev;
    cin >> cur >> prev;
    pm.Insert(cur, prev);
  }

  for (const auto& item : pm.Get()) {
    cout << item << " ";
  }
  return 0;
}
