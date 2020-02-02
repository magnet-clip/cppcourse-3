#include <deque>
#include <iomanip>
#include <iostream>
#include <map>

//#define TEST
#ifdef TEST
#include "test_runner.h"
#endif
using namespace std;

struct ReservationEvent {
  int64_t time;
  string hotel_name;
  uint32_t user_id;
  uint16_t room_number;
};

struct AggregatedReservation {
  uint32_t room_count;

  void DecreaseUserCount(uint32_t user_id) {
    if (--user_counts[user_id] == 0) {
      user_counts.erase(user_id);
    }
  }

  inline void IncreaseUserCount(uint32_t user_id) { user_counts[user_id] += 1; }

  inline size_t GetUserCount() const { return user_counts.size(); }

 private:
  map<uint32_t, uint32_t> user_counts;
};

template <auto N = 86400>
class BookingManager {
 public:
  BookingManager() {}

  void Book(int64_t curr_time, string hotel_name, uint32_t user_id,
            uint16_t room_count) {
    const auto new_last_time = curr_time - N;
    while (!_events.empty() && _events.front().time <= new_last_time) {
      const auto& last_event = _events.front();

      auto& last_hotel = _aggregated[last_event.hotel_name];
      last_hotel.room_count -= last_event.room_number;
      last_hotel.DecreaseUserCount(last_event.user_id);

      _events.pop_front();
    }

    _events.push_back({curr_time, hotel_name, user_id, room_count});
    auto& new_hotel = _aggregated[hotel_name];
    new_hotel.room_count += room_count;
    new_hotel.IncreaseUserCount(user_id);
  }

  uint32_t Clients(string hotel_name) const {
    if (!_aggregated.count(hotel_name)) return 0;
    auto& hotel = _aggregated.at(hotel_name);
    return hotel.GetUserCount();
  }

  uint32_t Rooms(string hotel_name) const {
    if (!_aggregated.count(hotel_name)) return 0;
    auto& hotel = _aggregated.at(hotel_name);
    return hotel.room_count;
  }

 private:
  deque<ReservationEvent> _events;

  // hotel name -> AggregatedReservation
  map<string, AggregatedReservation> _aggregated;
};
#ifdef TEST

void TestSingle() {
  BookingManager<100> manager;
  ASSERT_EQUAL(manager.Clients("A"), 0);
  ASSERT_EQUAL(manager.Rooms("A"), 0);

  manager.Book(0, "A", 1, 1);  // at 0 user 1 books 1 room
  ASSERT_EQUAL(manager.Clients("A"), 1);
  ASSERT_EQUAL(manager.Rooms("A"), 1);

  manager.Book(10, "A", 1, 2);  // at 10 user 1 books 2 rooms
  ASSERT_EQUAL(manager.Clients("A"), 1);
  ASSERT_EQUAL(manager.Rooms("A"), 3);

  manager.Book(20, "A", 2, 1);  // at 20 user 2 books 1 room
  ASSERT_EQUAL(manager.Clients("A"), 2);
  ASSERT_EQUAL(manager.Rooms("A"), 4);

  manager.Book(100, "A", 3, 3);  // at 100 user 3 books 3 rooms
  ASSERT_EQUAL(manager.Clients("A"), 3);
  ASSERT_EQUAL(manager.Rooms("A"), 6);

  manager.Book(105, "A", 3, 1);  // at 105 user 3 books 1 rooms
  ASSERT_EQUAL(manager.Clients("A"), 3);
  ASSERT_EQUAL(manager.Rooms("A"), 7);

  manager.Book(111, "A", 2, 4);  // at 111 user 2 books 4 rooms
  ASSERT_EQUAL(manager.Rooms("A"), 9);
  ASSERT_EQUAL(manager.Clients("A"), 2);
}

void TestNegative() {
  BookingManager<100> manager;
  ASSERT_EQUAL(manager.Clients("A"), 0);
  ASSERT_EQUAL(manager.Rooms("A"), 0);

  manager.Book(-100, "A", 1, 1);  // at 0 user 1 books 1 room
  ASSERT_EQUAL(manager.Clients("A"), 1);
  ASSERT_EQUAL(manager.Rooms("A"), 1);

  manager.Book(-90, "A", 1, 2);  // at 10 user 1 books 2 rooms
  ASSERT_EQUAL(manager.Clients("A"), 1);
  ASSERT_EQUAL(manager.Rooms("A"), 3);

  manager.Book(-80, "A", 2, 1);  // at 20 user 2 books 1 room
  ASSERT_EQUAL(manager.Clients("A"), 2);
  ASSERT_EQUAL(manager.Rooms("A"), 4);

  manager.Book(0, "A", 3, 3);  // at 100 user 3 books 3 rooms
  ASSERT_EQUAL(manager.Clients("A"), 3);
  ASSERT_EQUAL(manager.Rooms("A"), 6);

  manager.Book(5, "A", 3, 1);  // at 105 user 3 books 1 rooms
  ASSERT_EQUAL(manager.Clients("A"), 3);
  ASSERT_EQUAL(manager.Rooms("A"), 7);

  manager.Book(11, "A", 2, 4);  // at 111 user 2 books 4 rooms
  ASSERT_EQUAL(manager.Rooms("A"), 9);
  ASSERT_EQUAL(manager.Clients("A"), 2);
}

void TestSimple() {
  BookingManager manager;

  // CLIENTS Marriott
  ASSERT_EQUAL(manager.Clients("Marriott"), 0);

  // ROOMS Marriott
  ASSERT_EQUAL(manager.Rooms("Marriott"), 0);

  // BOOK 10 FourSeasons 1 2
  manager.Book(10, "FourSeasons", 1, 2);

  // BOOK 10 Marriott 1 1
  manager.Book(10, "Marriott", 1, 1);

  // BOOK 86409 FourSeasons 2 1
  manager.Book(86409, "FourSeasons", 2, 1);

  // CLIENTS FourSeasons
  ASSERT_EQUAL(manager.Clients("FourSeasons"), 2);

  // ROOMS FourSeasons
  ASSERT_EQUAL(manager.Rooms("FourSeasons"), 3);

  // CLIENTS Marriott
  ASSERT_EQUAL(manager.Clients("Marriott"), 1);

  // BOOK 86410 Marriott 2 10
  manager.Book(86410, "Marriott", 2, 10);

  // ROOMS FourSeasons
  ASSERT_EQUAL(manager.Rooms("FourSeasons"), 1);

  // ROOMS Marriott
  ASSERT_EQUAL(manager.Rooms("Marriott"), 10);
}

void TestAll() {
  TestRunner tr;
  RUN_TEST(tr, TestSingle);
  RUN_TEST(tr, TestSimple);
  RUN_TEST(tr, TestNegative);
}
#endif
int main() {
#ifdef TEST
  TestAll();
#endif

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  BookingManager manager;

  int query_count;
  cin >> query_count;

  int idx = 0;
  while (idx++ < query_count) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int64_t time;
      string hotel_name;
      uint32_t user_id;
      int16_t room_count;
      cin >> time >> hotel_name >> user_id >> room_count;
      manager.Book(time, hotel_name, user_id, room_count);
    } else if (query_type == "CLIENTS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Clients(hotel_name) << "\n";
    } else if (query_type == "ROOMS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Rooms(hotel_name) << "\n";
    }
  }

  return 0;
}