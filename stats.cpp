#include "stats.h"

HttpRequest ParseRequest(string_view line) {
  HttpRequest res;
  int pos = line.find_first_not_of(' ');
  line.remove_prefix(pos);

  pos = line.find(' ');
  res.method = line.substr(0, pos);
  line.remove_prefix(pos + 1);

  pos = line.find(' ');
  res.uri = line.substr(0, pos);
  line.remove_prefix(pos + 1);

  res.protocol = line;

  return res;
}

Stats::Stats()
    : _methods(
          {{"GET", 0}, {"PUT", 0}, {"POST", 0}, {"DELETE", 0}, {UNKNOWN, 0}}),
      _uris({{"/", 0},
             {"/order", 0},
             {"/product", 0},
             {"/basket", 0},
             {"/help", 0},
             {unknown, 0}}) {}

void Stats::AddMethod(string_view method) {
  if (_methods.count(method)) {
    _methods[method]++;
  } else {
    _methods[UNKNOWN]++;
  }
}
void Stats::AddUri(string_view uri) {
  if (_uris.count(uri)) {
    _uris[uri]++;
  } else {
    _uris[unknown]++;
  }
}
const map<string_view, int>& Stats::GetMethodStats() const { return _methods; }
const map<string_view, int>& Stats::GetUriStats() const { return _uris; }
