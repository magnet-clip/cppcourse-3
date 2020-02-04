// #include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>
// #include "test_runner.h"

using namespace std;

class ReadingManager {
public:
  ReadingManager() {}

  void Read(int user_id, int page_count) {
    int old_page_count = 0;

    // If user did exists, save his previous pages count
    if (_users_pages.count(user_id)) {
      old_page_count = _users_pages[user_id];
    }
    _users_pages[user_id] = page_count;

    // If there are already users with same amount of pages
    if (_number_of_users_with_pages.count(page_count)) {
      // Increase their number
      _number_of_users_with_pages[page_count] += 1;
    } else {
      // Otherwise indicate that now one does have such number of pages
      _number_of_users_with_pages[page_count] = 1;
    }
    _pages_counts.insert(page_count);

    _number_of_users_with_pages[old_page_count] -= 1;
    if (_number_of_users_with_pages[old_page_count] == 0) {
      _pages_counts.erase(old_page_count);
    }
  }

  double Cheer(int user_id) const {
    // 1) Check if he read anything

    // If this user has not read anything
    if (!_users_pages.count(user_id)) {
      return 0;
    }

    // If this user is the only user
    if (_users_pages.size() == 1) {
      return 1;
    }

    // 2) Find his pages count
    auto pages_count = _users_pages.at(user_id);

    // 3) Find all pages counts less than his
    auto finish = _pages_counts.lower_bound(pages_count);

    // 4) Go thru all pages counts and sum up counts of users
    auto total_users_with_pages_less_then_his = 0;
    for (auto it = _pages_counts.begin(); it != finish; it++) {
      total_users_with_pages_less_then_his +=
          _number_of_users_with_pages.at(*it);
    }

    auto res =
        total_users_with_pages_less_then_his * 1.0 / (_users_pages.size() - 1);
    return res;
  }

private:
  map<int, int> _users_pages;                // user -> page count
  map<int, int> _number_of_users_with_pages; // page count -> number of users
                                             // with this count of pages
  set<int> _pages_counts;                    // distinct pages count
};

// template <typename T>
// inline bool fpequal(T a, T b) {
//   return fabs(a - b) < 1e-4;
// }

// void TestVerySimple() {
//   ReadingManager manager;

//   manager.Read(1, 10);
//   manager.Read(2, 20);
//   manager.Read(3, 30);

//   ASSERT(fpequal(manager.Cheer(1), 0.0));
//   ASSERT(fpequal(manager.Cheer(2), 0.5));
//   ASSERT(fpequal(manager.Cheer(3), 1.0));
// }

// void TestAsInSample() {
//   ReadingManager manager;

//   ASSERT(fpequal(manager.Cheer(5), 0.0));

//   manager.Read(1, 10);
//   ASSERT(fpequal(manager.Cheer(1), 1.0));

//   manager.Read(2, 5);
//   manager.Read(3, 7);
//   ASSERT(fpequal(manager.Cheer(2), 0.0));
//   ASSERT(fpequal(manager.Cheer(3), 0.5));

//   manager.Read(3, 10);
//   ASSERT(fpequal(manager.Cheer(3), 0.5));

//   manager.Read(3, 11);
//   ASSERT(fpequal(manager.Cheer(3), 1.0));
//   ASSERT(fpequal(manager.Cheer(1), 0.5));
// }

// void TestAll() {
//   TestRunner tr;
//   RUN_TEST(tr, TestVerySimple);
//   RUN_TEST(tr, TestAsInSample);
// }

int main() {
  // TestAll();

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}