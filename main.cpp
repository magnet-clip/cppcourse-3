#include "test_runner.h"
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

class ReadingManager {
public:
  ReadingManager() {}

  void Read(int user_id, int page_count) {
    int old_page_count = 0;
    if (_users_pages.count(user_id)) {
      old_page_count = _users_pages[user_id];
    }
    _users_pages[user_id] = page_count;

    if (_number_of_users_with_pages.count(page_count)) {
      _number_of_users_with_pages[page_count] += 1;
    } else {
      _number_of_users_with_pages[page_count] = 1;
    }
    _pages_counts.insert(page_count);

    _number_of_users_with_pages[old_page_count] -= 1;
    if (_number_of_users_with_pages[old_page_count] == 0) {
      _pages_counts.erase(old_page_count);
    }
  }

  double Cheer(int user_id) const {}

private:
  map<int, int> _users_pages;                // user -> page count
  map<int, int> _number_of_users_with_pages; // page count -> number of users
                                             // with this count of pages
  set<int> _pages_counts;                    // distinct pages count
};

void TestAll() {}

int main() {
  TestAll();

  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
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