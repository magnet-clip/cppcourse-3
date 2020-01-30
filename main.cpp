// #include "test_runner.h"
#include <algorithm>
#include <set>
#include <string>
#include <vector>

using namespace std;

class Learner {
private:
  set<string> dict;

public:
  int Learn(const vector<string> &words) {
    auto last_size = dict.size();
    dict.insert(words.begin(), words.end());
    return dict.size() - last_size;
  }

  vector<string> KnownWords() {
    return vector<string>(dict.begin(), dict.end());
  }
};

// void Test1() {}

// void TestAll() {
//   TestRunner tr;
//   RUN_TEST(tr, Test1);
// }

// int main() {
//   TestAll();

//   Learner learner;
//   string line;
//   while (getline(cin, line)) {
//     vector<string> words;
//     stringstream ss(line);
//     string word;
//     while (ss >> word) {
//       words.push_back(word);
//     }
//     cout << learner.Learn(words) << "\n";
//   }
//   cout << "=== known words ===\n";
//   for (auto word : learner.KnownWords()) {
//     cout << word << "\n";
//   }

//   return 0;
// }
