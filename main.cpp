#include "test_runner.h"

#include <list>
#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

enum class ReaderState { Started, ReadingEndOfSequence, ReadingRegularChar };

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  vector<Sentence<Token>> res;

  list<Token> tokens_list;
  {
    auto start = tokens.begin();
    while (start != tokens.end()) {
      tokens_list.push_back(move(*start++));
    }
  }
  {
    ReaderState state = ReaderState::Started;
    while (!tokens_list.empty()) {
      auto& next_token = tokens_list.front();
      bool next_is_end = next_token.IsEndSentencePunctuation();

      if (state == ReaderState::Started) {
        vector<Token> new_one;
        res.push_back(move(new_one));
        state = next_is_end ? ReaderState::ReadingEndOfSequence
                            : ReaderState::ReadingRegularChar;
      }

      if (next_is_end && state == ReaderState::ReadingRegularChar) {
        // we were reading a regular symbol and hit end of sentence
        // nothing really to do here as there can be few of them
        state = ReaderState::ReadingEndOfSequence;
      } else if (!next_is_end && state == ReaderState::ReadingEndOfSequence) {
        // we were reading end of sentence and now hit regular symbol => we have
        // to start  a new sentence
        state = ReaderState::ReadingRegularChar;
        vector<Token> new_one;
        res.push_back(move(new_one));
      }

      // keep storing into current sentence
      res.back().push_back(move(tokens_list.front()));
      tokens_list.pop_front();
    }
  }
  return res;
}

struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const { return is_end_sentence_punctuation; }
  bool operator==(const TestToken& other) const {
    return data == other.data &&
           is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

struct TestToken2 {
  string data;
  bool is_end_sentence_punctuation = false;

  TestToken2(const TestToken2&) = delete;
  TestToken2& operator=(const TestToken2&) = delete;

  TestToken2(TestToken2&&) = default;
  TestToken2& operator=(TestToken2&&) = default;

  bool IsEndSentencePunctuation() const { return is_end_sentence_punctuation; }
  bool operator==(const TestToken2& other) const {
    return data == other.data &&
           is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}
ostream& operator<<(ostream& stream, const TestToken2& token) {
  return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
  ASSERT_EQUAL(SplitIntoSentences(vector<TestToken>(
                   {{"Split"}, {"into"}, {"sentences"}, {"!"}})),
               vector<Sentence<TestToken>>(
                   {{{"Split"}, {"into"}, {"sentences"}, {"!"}}}));

  ASSERT_EQUAL(SplitIntoSentences(vector<TestToken>(
                   {{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
               vector<Sentence<TestToken>>(
                   {{{"Split"}, {"into"}, {"sentences"}, {"!", true}}}));

  ASSERT_EQUAL(
      SplitIntoSentences(vector<TestToken>({{"Split"},
                                            {"into"},
                                            {"sentences"},
                                            {"!", true},
                                            {"!", true},
                                            {"Without"},
                                            {"copies"},
                                            {".", true}})),
      vector<Sentence<TestToken>>({
          {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
          {{"Without"}, {"copies"}, {".", true}},
      }));
}

void TestEmpty() {
  ASSERT_EQUAL(SplitIntoSentences(vector<TestToken>()),
               vector<Sentence<TestToken>>());
  ASSERT_EQUAL(SplitIntoSentences(vector<TestToken>({{"a"}})),
               vector<Sentence<TestToken>>({{{"a"}}}));
  ASSERT_EQUAL(SplitIntoSentences(vector<TestToken>()),
               vector<Sentence<TestToken>>());
  ASSERT_EQUAL(SplitIntoSentences(vector<TestToken>({{"a", true}})),
               vector<Sentence<TestToken>>({{{"a", true}}}));
}

void TestSplitting2() {
  {
    vector<TestToken2> v1;
    v1.push_back({"Split"});
    v1.push_back({"into"});
    v1.push_back({"sentences"});
    v1.push_back({"!"});

    vector<Sentence<TestToken2>> v2;
    Sentence<TestToken2> v2_sentence;
    v2.push_back(move(v2_sentence));
    v2.back().push_back({"Split"});
    v2.back().push_back({"into"});
    v2.back().push_back({"sentences"});
    v2.back().push_back({"!"});

    for (size_t i = 0; i < v1.size(); i++) {
      ASSERT_EQUAL(v1[i], v2[0][i]);
    }
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  RUN_TEST(tr, TestSplitting2);
  RUN_TEST(tr, TestEmpty);
  return 0;
}
