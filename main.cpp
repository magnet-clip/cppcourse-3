#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

class NonCopyableInt {
public:
  int value;
  NonCopyableInt(const NonCopyableInt &) = delete;
  NonCopyableInt &operator=(const NonCopyableInt &) = delete;
  NonCopyableInt(NonCopyableInt &&) = default;
  NonCopyableInt &operator=(NonCopyableInt &&) = default;
};

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  // Напишите реализацию функции,
  // не копируя сортируемые элементы
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

void TestNonCopyIntVector() {
  vector<NonCopyableInt> numbers;
  numbers.push_back({6});
  numbers.push_back({1});
  numbers.push_back({3});
  numbers.push_back({9});
  numbers.push_back({1});
  numbers.push_back({9});
  numbers.push_back({8});
  numbers.push_back({12});
  numbers.push_back({1});
  MergeSort(begin(numbers), end(numbers));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
