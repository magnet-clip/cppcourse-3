#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

ostream &operator<<(ostream &os, const Date &d) {
  return os << d.year << "-" << d.month << "-" << d.day;
}

istream &operator>>(istream &is, Date &d) {
  char d1, d2;
  is >> d.year >> d1 >> d.month >> d2 >> d.day;
  return is;
}

bool operator<(const Date &d1, const Date &d2) {
  if (d1.year == d2.year) {
    if (d1.month == d2.month) {
      return d1.day < d2.day;
    } else {
      return d1.month < d2.month;
    }
  } else {
    return d1.year < d2.year;
  }
}

bool operator==(const Date &d1, const Date &d2) {
  return d1.year == d2.year && d1.month == d2.month && d1.day == d2.day;
}

ostream &operator<<(ostream &os, const Time &t) {
  return os << t.hours << ":" << t.minutes;
}

istream &operator>>(istream &is, Time &t) {
  char d;
  is >> t.hours >> d >> t.minutes;
  return is;
}

bool operator<(const Time &t1, const Time &t2) {
  if (t1.hours == t2.hours) {
    return t1.minutes < t2.minutes;
  } else {
    return t1.hours < t2.hours;
  }
}

bool operator==(const Time &t1, const Time &t2) {
  return t1.hours == t2.hours && t1.minutes == t2.minutes;
}

#define UPDATE_FIELD(ticket, field, values)                                    \
  {                                                                            \
    const auto it = values.find(#field);                                       \
    if (it != values.end()) {                                                  \
      istringstream is(it->second);                                            \
      is >> ticket.field;                                                      \
    }                                                                          \
  }

void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
      {"departure_date", "2018-2-28"},
      {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
      {"price", "12550"},
      {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
  // значения этих полей не должны измениться
  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}
