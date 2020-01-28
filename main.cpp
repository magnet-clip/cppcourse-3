#include "deque.h"
#include "test_runner.h"

void TestEmpty() {
  Deque<int> dq;
  ASSERT(dq.Empty());
  ASSERT_EQUAL(dq.Size(), 0);
}

void TestBack() {
  Deque<int> dq;
  dq.PushBack(1);
  ASSERT_EQUAL(dq.Back(), 1);
  ASSERT_EQUAL(dq.Front(), 1);

  dq.Back() = 3;
  ASSERT_EQUAL(dq.Back(), 3);
  ASSERT_EQUAL(dq.Front(), 3);
}

void TestFront() {
  Deque<int> dq;
  dq.PushFront(1);
  ASSERT_EQUAL(dq.Back(), 1);
  ASSERT_EQUAL(dq.Front(), 1);

  dq.Front() = 3;
  ASSERT_EQUAL(dq.Back(), 3);
  ASSERT_EQUAL(dq.Front(), 3);

  dq.PushFront(2);
  ASSERT_EQUAL(dq.Back(), 3);
  ASSERT_EQUAL(dq.Front(), 2);
}

void TestFrontAndBack() {
  Deque<int> dq;
  dq.PushFront(1);
  ASSERT_EQUAL(dq.Back(), 1);
  ASSERT_EQUAL(dq.Front(), 1);

  dq.PushBack(3);
  ASSERT_EQUAL(dq.Back(), 3);
  ASSERT_EQUAL(dq.Front(), 1);

  dq.PushFront(7);
  ASSERT_EQUAL(dq.Back(), 3);
  ASSERT_EQUAL(dq.Front(), 7);
}

void TestOrder() {
  {
    Deque<int> dq;
    dq.PushFront(1);
    ASSERT_EQUAL(dq[0], 1);
    ASSERT_EQUAL(dq.At(0), 1);

    dq.PushFront(2);
    ASSERT_EQUAL(dq[0], 2);
    ASSERT_EQUAL(dq[1], 1);
    ASSERT_EQUAL(dq.At(0), 2);
    ASSERT_EQUAL(dq.At(1), 1);

    try {
      dq.At(3);
      ASSERT(false);
    } catch (std::out_of_range &) {
    }
  }
  {
    Deque<int> dq;
    dq.PushBack(1);
    ASSERT_EQUAL(dq[0], 1);
    ASSERT_EQUAL(dq.At(0), 1);

    dq.PushBack(2);
    ASSERT_EQUAL(dq[0], 1);
    ASSERT_EQUAL(dq[1], 2);
    ASSERT_EQUAL(dq.At(0), 1);
    ASSERT_EQUAL(dq.At(1), 2);

    try {
      dq.At(3);
      ASSERT(false);
    } catch (std::out_of_range &) {
    }
  }
  {
    Deque<int> dq;
    dq.PushBack(1); // 1
    ASSERT_EQUAL(dq[0], 1);
    ASSERT_EQUAL(dq.At(0), 1);

    dq.PushBack(2); // 1 2
    ASSERT_EQUAL(dq[0], 1);
    ASSERT_EQUAL(dq[1], 2);
    ASSERT_EQUAL(dq.At(0), 1);
    ASSERT_EQUAL(dq.At(1), 2);
    try {
      dq.At(3);
      ASSERT(false);
    } catch (std::out_of_range &) {
    }

    dq.PushFront(10); // 10 1 2

    ASSERT_EQUAL(dq[0], 10);
    ASSERT_EQUAL(dq[1], 1);
    ASSERT_EQUAL(dq[2], 2);

    dq.PushFront(1); // 1 10 1 2
    ASSERT_EQUAL(dq[0], 1);
    ASSERT_EQUAL(dq[1], 10);
    ASSERT_EQUAL(dq[2], 1);
    ASSERT_EQUAL(dq[3], 2);

    dq.PushBack(3); // 1 10 1 2 3
    ASSERT_EQUAL(dq[4], 3);
  }
}

void TestAll() {
  TestRunner tr;
  RUN_TEST(tr, TestEmpty);
  RUN_TEST(tr, TestBack);
  RUN_TEST(tr, TestFront);
  RUN_TEST(tr, TestFrontAndBack);
  RUN_TEST(tr, TestOrder);
}

int main() {
  TestAll();

  return 0;
}
