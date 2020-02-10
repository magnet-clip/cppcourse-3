#include "profile.h"
#include "test_runner.h"

#include <map>
#include <string>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator+=(const Stats& other);
};

void Stats::operator+=(const Stats& other) {
  for (const auto& x : other.word_frequences) {
    word_frequences[x.first] += x.second;
  }
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
  string_view line_view = line;
  Stats res;

  // remove leading spaces
  line_view.remove_prefix(
      min(line_view.find_first_not_of(' '), line_view.size()));

  // peek separate words
  while (line_view.size() > 0) {
    auto not_space_pos = min(line_view.find_first_of(' '), line_view.size());
    auto word = string(line_view.substr(0, not_space_pos));

    if (key_words.count(word)) {
      res.word_frequences[word]++;
    }

    line_view.remove_prefix(not_space_pos);
    auto space_pos = min(line_view.find_first_not_of(' '), line_view.size());
    line_view.remove_prefix(space_pos);
  }

  return res;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words,
                                  istream& input) {
  Stats result;
  for (string line; getline(input, line);) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  return ExploreKeyWordsSingleThread(key_words, input);
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {{"yangle", 6}, {"rocks", 2}, {"sucks", 1}};
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
