#include "profile.h"
#include "test_runner.h"

#include <algorithm>
#include <future>
#include <map>
#include <numeric>
#include <string>
using namespace std;

template <typename Iterator> class Page {
public:
  Page(const Iterator &begin, const Iterator &end) : _begin(begin), _end(end) {
    _size = distance(begin, end);
  }
  Iterator begin() { return _begin; }
  Iterator end() { return _end; }
  Iterator begin() const { return _begin; }
  Iterator end() const { return _end; }
  size_t size() const { return _size; }

private:
  const Iterator _begin, _end;
  size_t _size;
};

template <typename Iterator> class Paginator {
public:
  Paginator(const Iterator &begin, const Iterator &end,
            unsigned int page_size) {
    if (begin == end) {
      _size = 0;
      return;
    }
    size_t page_count = 0;
    Iterator start = begin;
    Iterator finish = begin;
    do {
      if (distance(start, end) > page_size) {
        finish = next(start, page_size);
      } else {
        finish = end;
      }
      _pages.push_back({start, finish});
      start = finish;
      page_count++;
    } while (finish != end);
    _size = page_count;
  }

  auto begin() { return _pages.begin(); }
  auto begin() const { return _pages.begin(); }
  auto end() { return _pages.end(); }
  auto end() const { return _pages.end(); }
  size_t size() const { return _size; }

private:
  vector<Page<Iterator>> _pages;
  size_t _size;
};

template <typename Container> auto Paginate(Container &c, size_t page_size) {
  return Paginator(c.begin(), c.end(), page_size);
}

struct Stats {
  map<string, int> word_frequences;
  // void operator+=(const Stats &other);
};

// void Stats::operator+=(const Stats &other) {
//   word_frequences =
//       accumulate(other.word_frequences.begin(), other.word_frequences.end(),
//                  word_frequences, [](map<string, int> m, pair<string, int> p)
//                  {
//                    m[p.first] += p.second;
//                    return m;
//                  });
// }

vector<string> SplitLine(const string &line) {
  string_view line_view = line;

  vector<string> words;
  // remove leading spaces
  line_view.remove_prefix(
      min(line_view.find_first_not_of(' '), line_view.size()));

  // peek separate words
  while (line_view.size() > 0) {
    auto not_space_pos = min(line_view.find_first_of(' '), line_view.size());
    auto word = string(line_view.substr(0, not_space_pos));
    line_view.remove_prefix(not_space_pos);

    words.push_back(word);

    auto space_pos = min(line_view.find_first_not_of(' '), line_view.size());
    line_view.remove_prefix(space_pos);
  }

  return words;
}

Stats ExploreKeyWordsSingleThread(const set<string> &key_words,
                                  vector<string>::iterator begin,
                                  vector<string>::iterator end) {
  Stats result;

  while (begin != end) {
    for (const auto word : SplitLine(*begin++)) {
      if (key_words.count(word)) {
        result.word_frequences[word]++;
      }
    }
  }

  return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  vector<string> lines;
  string line;
  while (getline(input, line)) {
    lines.push_back(line);
  }

  return ExploreKeyWordsSingleThread(key_words, lines.begin(), lines.end());
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
#define PERF
#ifdef PERF

  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};
  vector<string> lines = {
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "10 hello my yangle world pee pee aww",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "this new yangle service really rocks",
      "It sucks when yangle isn't available",
      "10 reasons why yangle is the best IT company",
      "yangle rocks others suck",
      "Goondex really sucks, but yangle rocks. Use yangle",
      "Up am intention on dependent questions oh elsewhere september",
      "No betrayed pleasure possible jointure we in throwing",
      "And can event rapid any shall woman green",
      "Hope they dear who its bred",
      "Smiling nothing affixed he carried it clothes calling he no",
      "Its something disposing departure she favourite tolerably engrossed",
      "Truth short folly court why she their balls",
      "Excellence put unaffected reasonable mrs introduced conviction she",
      "Nay particular delightful but unpleasant for uncommonly who",
      "  In show dull give need so held",
      "One order all scale sense her gay style wrote",
      "Incommode our not one ourselves residence",
      "Shall there whose those stand she end",
      "So unaffected partiality indulgence dispatched to of celebrated "
      "remarkably",
      "Unfeeling are had allowance own perceived abilities",
      "  Affronting discretion as do is announcing",
      "Now months esteem oppose nearer enable too six",
      "She numerous unlocked you perceive speedily",
      "Affixed offence spirits or ye of offices between",
      "Real on shot it were four an as",
      "Absolute bachelor rendered six nay you juvenile",
      "Vanity entire an chatty to",
      "  Prepared do an dissuade be so whatever steepest",
      "Yet her beyond looked either day wished nay",
      "By doubtful disposed do juvenile an",
      "Now curiosity you explained immediate why behaviour",
      "An dispatched impossible of of melancholy favourable",
      "Our quiet not heart along scale sense timed",
      "Consider may dwelling old him her surprise finished families "
      "graceful ",
      " Gave led past poor met fine was new ",
      " Improve ashamed married expense bed her comfort pursuit mrs",
      " Four time took ye your as fail lady ",
      " Up greatest am exertion or        marianne ",
      " Shy occasional terminated insensible and inhabiting gay",
      " So know do fond to half on ",
      " Now who promise was justice new winding    ",
      " In finished on he speaking suitable        advanced if ",
      "  He an thing rapid these after going drawn or",
      "Timed she his law the spoil round defer",
      "In surprise concerns informed betrayed he learning is ye",
      "Ignorant formerly so ye blessing",
      "He as spoke avoid given downs money on we",
      "Of properly carriage shutters ye as wandered up repeated moreover",
      "Inquietude attachment if ye an solicitude to",
      "Remaining so continued concealed as knowledge happiness",
      "So me by marianne admitted speaking",
      "Men bred fine call ask",
      "Cease one miles truth day above seven",
      "Suspicion sportsmen provision suffering mrs saw engrossed something",
      "Snug soon he on plan in be dine some",
      "  Oh to talking improve produce in limited offices fifteen an",
      "Wicket branch to answer do we",
      "Place are decay men hours tiled",
      "If or of ye throwing friendly required",
      "Marianne interest in exertion as",
      "Offering my branched confined oh dashwood",
  };

  const auto N = 10000;

  {
    LOG_DURATION("stl");
    Stats result;
    for (auto i = 0; i < N; i++) {
      ExploreKeyWordsSingleThread(key_words, lines.begin(), lines.end());
    }
  }
#endif
}
