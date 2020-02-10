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

Stats ExploreLine1(const set<string>& key_words, const string& line) {
  Stats res;
  auto start = line.begin();
  auto end = line.end();

  while (*start == ' ' && start != end) {
    start++;
  }

  while (start != end) {
    string word;
    // word.reserve(20);

    while (*start != ' ' && start != end) {
      word.push_back(*start++);
    }

    if (key_words.count(word)) {
      res.word_frequences[word]++;
    }

    while (*start == ' ' && start != end) {
      start++;
    }
  }
  return res;
}

Stats ExploreLine2(const set<string>& key_words, const string& line) {
  Stats res;

  string_view line_view = line;

  // remove leading spaces
  line_view.remove_prefix(
      min(line_view.find_first_not_of(' '), line_view.size()));

  // peek separate words
  while (line_view.size() > 0) {
    auto not_space_pos = min(line_view.find_first_of(' '), line_view.size());
    auto word = string(line_view.substr(0, not_space_pos));
    line_view.remove_prefix(not_space_pos);

    if (key_words.count(word)) {
      res.word_frequences[word]++;
    }

    auto space_pos = min(line_view.find_first_not_of(' '), line_view.size());
    line_view.remove_prefix(space_pos);
  }

  return res;
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
  return ExploreLine1(key_words, line);
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

  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};
  vector<string> lines;
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("10 hello my yangle world pee pee aww");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back("this new yangle service really rocks");
  lines.push_back("It sucks when yangle isn't available");
  lines.push_back("10 reasons why yangle is the best IT company");
  lines.push_back("yangle rocks others suck");
  lines.push_back("Goondex really sucks, but yangle rocks. Use yangle");
  lines.push_back(
      "Up am intention on dependent questions oh elsewhere september");
  lines.push_back("No betrayed pleasure possible jointure we in throwing");
  lines.push_back("And can event rapid any shall woman green");
  lines.push_back("Hope they dear who its bred");
  lines.push_back(
      "Smiling nothing affixed he carried it clothes calling he no");
  lines.push_back(
      "Its something disposing departure she favourite tolerably engrossed");
  lines.push_back("Truth short folly court why she their balls");
  lines.push_back(
      "Excellence put unaffected reasonable mrs introduced conviction she");
  lines.push_back(
      "Nay particular delightful but unpleasant for uncommonly who");
  lines.push_back("  In show dull give need so held");
  lines.push_back("One order all scale sense her gay style wrote");
  lines.push_back("Incommode our not one ourselves residence");
  lines.push_back("Shall there whose those stand she end");
  lines.push_back(
      "So unaffected partiality indulgence dispatched to of celebrated "
      "remarkably");
  lines.push_back("Unfeeling are had allowance own perceived abilities");
  lines.push_back("  Affronting discretion as do is announcing");
  lines.push_back("Now months esteem oppose nearer enable too six");
  lines.push_back("She numerous unlocked you perceive speedily");
  lines.push_back("Affixed offence spirits or ye of offices between");
  lines.push_back("Real on shot it were four an as");
  lines.push_back("Absolute bachelor rendered six nay you juvenile");
  lines.push_back("Vanity entire an chatty to");
  lines.push_back("  Prepared do an dissuade be so whatever steepest");
  lines.push_back("Yet her beyond looked either day wished nay");
  lines.push_back("By doubtful disposed do juvenile an");
  lines.push_back("Now curiosity you explained immediate why behaviour");
  lines.push_back("An dispatched impossible of of melancholy favourable");
  lines.push_back("Our quiet not heart along scale sense timed");
  lines.push_back(
      "Consider may dwelling old him her surprise finished families graceful");
  lines.push_back("Gave led past poor met fine was new");
  lines.push_back(
      "  Improve ashamed married expense bed her comfort pursuit mrs");
  lines.push_back("Four time took ye your as fail lady");
  lines.push_back("Up greatest am exertion or marianne");
  lines.push_back("Shy occasional terminated insensible and inhabiting gay");
  lines.push_back("So know do fond to half on");
  lines.push_back("Now who promise was justice new winding");
  lines.push_back("In finished on he speaking suitable advanced if");
  lines.push_back(
      "Boy happiness sportsmen say prevailed offending concealed nor was "
      "provision");
  lines.push_back("Provided so as doubtful on striking required");
  lines.push_back("Waiting we to compass assured");
  lines.push_back("  Consider now provided laughter boy landlord dashwood");
  lines.push_back("Often voice and the spoke");
  lines.push_back(
      "No shewing fertile village equally prepare up females as an");
  lines.push_back("That do an case an what plan hour of paid");
  lines.push_back(
      "Invitation is unpleasant astonished preference attachment friendship "
      "on");
  lines.push_back("Did sentiments increasing particular nay");
  lines.push_back("Mr he recurred received prospect in");
  lines.push_back("Wishing cheered parlors adapted am at amongst matters");
  lines.push_back("  Affronting imprudence do he he everything");
  lines.push_back("Sex lasted dinner wanted indeed wished out law");
  lines.push_back("Far advanced settling say finished raillery");
  lines.push_back("Offered chiefly farther of my no colonel shyness");
  lines.push_back("Such on help ye some door if in");
  lines.push_back("Laughter proposal laughing any son law consider");
  lines.push_back("Needed except up piqued an");
  lines.push_back("  He an thing rapid these after going drawn or");
  lines.push_back("Timed she his law the spoil round defer");
  lines.push_back("In surprise concerns informed betrayed he learning is ye");
  lines.push_back("Ignorant formerly so ye blessing");
  lines.push_back("He as spoke avoid given downs money on we");
  lines.push_back(
      "Of properly carriage shutters ye as wandered up repeated moreover");
  lines.push_back("Inquietude attachment if ye an solicitude to");
  lines.push_back("Remaining so continued concealed as knowledge happiness");
  lines.push_back(
      "Preference did how expression may favourable devonshire insipidity "
      "considered");
  lines.push_back("An length design regret an hardly barton mr figure");
  lines.push_back("  Started his hearted any civilly");
  lines.push_back("So me by marianne admitted speaking");
  lines.push_back("Men bred fine call ask");
  lines.push_back("Cease one miles truth day above seven");
  lines.push_back(
      "Suspicion sportsmen provision suffering mrs saw engrossed something");
  lines.push_back("Snug soon he on plan in be dine some");
  lines.push_back(
      "  Oh to talking improve produce in limited offices fifteen an");
  lines.push_back("Wicket branch to answer do we");
  lines.push_back("Place are decay men hours tiled");
  lines.push_back("If or of ye throwing friendly required");
  lines.push_back("Marianne interest in exertion as");
  lines.push_back("Offering my branched confined oh dashwood");

  const auto N = 10000;
  {
    LOG_DURATION("direct");
    Stats result;
    for (auto i = 0; i < N; i++) {
      for (const auto& line : lines) {
        result += ExploreLine1(key_words, line);
      }
    }
  }
  {
    LOG_DURATION("stl");
    Stats result;
    for (auto i = 0; i < N; i++) {
      for (const auto& line : lines) {
        result += ExploreLine2(key_words, line);
      }
    }
  }
}
