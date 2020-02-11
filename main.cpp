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
  void operator+=(const Stats &other);
};

void Stats::operator+=(const Stats &other) {
  word_frequences =
      accumulate(other.word_frequences.begin(), other.word_frequences.end(),
                 word_frequences, [](map<string, int> m, pair<string, int> p) {
                   m[p.first] += p.second;
                   return m;
                 });
}

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
                                  vector<string>::iterator end, int thread) {
  Stats result;

  while (begin != end) {
    // cout << thread << "\n";
    for (const auto word : SplitLine(*begin++)) {
      if (key_words.count(word)) {
        result.word_frequences[word]++;
      }
    }
  }

  return result;
}
Stats ExploreKeyWords2(const set<string> &key_words, istream &input) {
  vector<string> lines;
  string line;
  while (getline(input, line)) {
    lines.push_back(line);
  }

  return ExploreKeyWordsSingleThread(key_words, lines.begin(), lines.end(), 1);
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  vector<string> lines;
  string line;
  while (getline(input, line)) {
    lines.push_back(line);
  }
  vector<Stats> res;
  mutex m;

  const auto pages = Paginate(lines, lines.size() / 4);
  {
    vector<future<void>> futures;
    int thread = 0;
    for (const auto &page : pages) {
      futures.push_back(async([&key_words, &page, &m, &res, thread]() {
        auto stats = ExploreKeyWordsSingleThread(key_words, page.begin(),
                                                 page.end(), thread);
        {
          lock_guard g(m);
          res.push_back(stats);
        }
      }));
      thread++;
    }
  }

  Stats result;
  for (const auto &stats : res) {
    for (const auto &item : stats.word_frequences) {
      result.word_frequences[item.first] += item.second;
    }
  }
  return result;
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
// #define TEST
#ifdef TEST
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
#endif

// #define PERF
#ifdef PERF

  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "10 hello my yangle world pee pee aww\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  ss << "Up am intention on dependent questions oh elsewhere september\n";
  ss << "No betrayed pleasure possible jointure we in throwing\n";
  ss << "And can event rapid any shall woman green\n";
  ss << "Hope they dear who its bred\n";
  ss << "Smiling nothing affixed he carried it clothes calling he no\n";
  ss << "Its something disposing departure she favourite tolerably engrossed\n";
  ss << "Truth short folly court why she their balls\n";
  ss << "Excellence put unaffected reasonable mrs introduced conviction she\n";
  ss << "Nay particular delightful but unpleasant for uncommonly who\n";
  ss << "  In show dull give need so held\n";
  ss << "One order all scale sense her gay style wrote\n";
  ss << "Incommode our not one ourselves residence\n";
  ss << "Shall there whose those stand she end\n";
  ss << "So unaffected partiality indulgence dispatched to of celebrated "
        "remarkably\n";
  ss << "Unfeeling are had allowance own perceived abilities\n";
  ss << "  Affronting discretion as do is announcing\n";
  ss << "Now months esteem oppose nearer enable too six\n";
  ss << "She numerous unlocked you perceive speedily\n";
  ss << "Affixed offence spirits or ye of offices between\n";
  ss << "Real on shot it were four an as\n";
  ss << "Absolute bachelor rendered six nay you juvenile\n";
  ss << "Vanity entire an chatty to\n";
  ss << "  Prepared do an dissuade be so whatever steepest\n";
  ss << "Yet her beyond looked either day wished nay\n";
  ss << "By doubtful disposed do juvenile an\n";
  ss << "Now curiosity you explained immediate why behaviour\n";
  ss << "An dispatched impossible of of melancholy favourable\n";
  ss << "Our quiet not heart along scale sense timed\n";
  ss << "Consider may dwelling old him her surprise finished families graceful "
        "\n";
  ss << " Gave led past poor met fine was new \n";
  ss << " Improve ashamed married expense bed her comfort pursuit mrs\n";
  ss << " Four time took ye your as fail lady \n";
  ss << " Up greatest am exertion or        marianne \n";
  ss << " Shy occasional terminated insensible and inhabiting gay\n";
  ss << " So know do fond to half on \n";
  ss << " Now who promise was justice new winding    \n";
  ss << " In finished on he speaking suitable        advanced if \n";
  ss << "  He an thing rapid these after going drawn or\n";
  ss << "Timed she his law the spoil round defer\n";
  ss << "In surprise concerns informed betrayed he learning is ye\n";
  ss << "Ignorant formerly so ye blessing\n";
  ss << "He as spoke avoid given downs money on we\n";
  ss << "Of properly carriage shutters ye as wandered up repeated moreover\n";
  ss << "Inquietude attachment if ye an solicitude to\n";
  ss << "Remaining so continued concealed as knowledge happiness\n";
  ss << "So me by marianne admitted speaking\n";
  ss << "Men bred fine call ask\n";
  ss << "Cease one miles truth day above seven\n";
  ss << "Suspicion sportsmen provision suffering mrs saw engrossed something\n";
  ss << "Snug soon he on plan in be dine some\n";
  ss << "  Oh to talking improve produce in limited offices fifteen an\n";
  ss << "Wicket branch to answer do we\n";
  ss << "Place are decay men hours tiled\n";
  ss << "If or of ye throwing friendly required\n";
  ss << "Marianne interest in exertion as\n";
  ss << "Offering my branched confined oh dashwood\n";

  const auto N = 10'000'000;

  {
    LOG_DURATION("par");
    Stats result;
    for (auto i = 0; i < N; i++) {
      ExploreKeyWords(key_words, ss);
    }
  }

  {
    LOG_DURATION("seq");
    Stats result;
    for (auto i = 0; i < N; i++) {
      ExploreKeyWords2(key_words, ss);
    }
  }
#endif
}
