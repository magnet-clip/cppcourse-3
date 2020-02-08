#pragma once

#include "http_request.h"

#include <map>
#include <string_view>
using namespace std;

// const array<string_view, 5> METHODS = {"GET", "PUT", "POST", "DELETE",
//                                        "UNKNOWN"};
// const array<string_view, 6> URIS = {"/",       "/order", "/product",
//                                     "/basket", "/help",  "unknown"};
class Stats {
 public:
  Stats();

  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;

 private:
  const string_view UNKNOWN{"UNKNOWN"};
  const string_view unknown{"unknown"};
  map<string_view, int> _methods;
  map<string_view, int> _uris;
};

HttpRequest ParseRequest(string_view line);