#include "deque.h"
#include "test_runner.h"

void TestEmpty() {
  Deque<int> dq;
  ASSERT(dq.Empty());
  ASSERT_EQUAL(dq.Size(), 0);
}

void TestAll() {
  TestRunner tr;
  RUN_TEST(tr, TestEmpty);
}

int main() {
  TestAll();

  return 0;
}
