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

bool operator<(NonCopyableInt a, NonCopyableInt b) { return a.value < b.value; }

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  // 1) Если диапазон содержит меньше 2 элементов, выйти из функции.
  size_t count = distance(range_begin, range_end);
  if (count < 2) {
    return;
  }

  // 2) Создать вектор, содержащий все элементы текущего диапазона.
  vector<typename RandomIt::value_type> temp;
  temp.reserve(count);
  move(range_begin, range_end, back_inserter(temp));

  // 3) Разбить вектор на три равные части. (В этой задаче гарантируется, что
  // длина передаваемого диапазона является степенью тройки, так что вектор
  // всегда можно разбить на три равные части.)

  // 4) Вызвать функцию MergeSort от каждой части вектора.
  MergeSort(temp.begin(), temp.begin() + count / 3);
  MergeSort(temp.begin() + count / 3, temp.begin() + 2 * count / 3);
  MergeSort(temp.begin() + 2 * count / 3, temp.end());

  // 5) Слить первые две трети вектора с помощью алгоритма merge, сохранив
  // результат во временный вектор с помощью back_inserter.
  vector<typename RandomIt::value_type> temp2;
  temp.reserve(count);

  merge(make_move_iterator(temp.begin()),
        make_move_iterator(temp.begin() + count / 3),
        make_move_iterator(temp.begin() + count / 3),
        make_move_iterator(temp.begin() + 2 * count / 3), back_inserter(temp2));

  // 6) Слить временный вектор из предыдущего пункта с последней третью вектора
  // из п. 2, записав полученный отсортированный диапазон вместо исходного.

  merge(make_move_iterator(temp2.begin()), make_move_iterator(temp2.end()),
        make_move_iterator(temp.begin() + 2 * count / 3),
        make_move_iterator(temp.end()), range_begin);
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
  RUN_TEST(tr, TestNonCopyIntVector);
  return 0;
}
