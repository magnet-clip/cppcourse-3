#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y)                                                \
  do {                                                                         \
    out << (x) << endl;                                                        \
    out << (y) << endl;                                                        \
  } while (0)

int main() {
  // volatile int x = 2;
  // ostringstream output;
  // if (1 == x) {
  //   PRINT_VALUES(output, 1 ? 2 : 3, x++);
  //   if (2 == x)
  //     PRINT_VALUES(output, []() { return 1; }(), x++);
  //   else
  //     PRINT_VALUES(output, 1, 2);
  // }
  TestRunner tr;
  tr.RunTest(
      [] {
        ostringstream output;
        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
      },
      "PRINT_VALUES usage example");
}
