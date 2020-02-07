#include <list>
#include <map>
#include <string>
#include <vector>
#include "test_runner.h"

using namespace std;

class Translator {
 public:
  void Add(string_view source, string_view target) {
    if (!_fwd.count(source)) {
      auto source_string = string(source);
      _words.push_back(source_string);
      source = _words.back();
    }
    if (!_rwd.count(target)) {
      auto target_string = string(target);
      _words.push_back(target_string);
      target = _words.back();
    }
    _fwd[source] = target;
    _rwd[target] = source;
  }

  string_view TranslateForward(string_view source) const {
    if (_fwd.count(source)) return _fwd.at(source);
    return "";
  }

  string_view TranslateBackward(string_view target) const {
    if (_rwd.count(target)) return _rwd.at(target);
    return "";
  }

 private:
  list<string> _words;
  map<string_view, string_view> _fwd;
  map<string_view, string_view> _rwd;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
