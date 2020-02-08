#include <list>
#include <string>
#include "test_runner.h"
using namespace std;

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() : _position(_text.begin()) {}

  void Left() {
    if (_position != _text.begin()) _position = prev(_position);
  }

  void Right() {
    if (_position != _text.end()) _position = next(_position);
  }

  void Insert(char token) { _text.insert(_position, token); }

  void Cut(size_t tokens = 1) {
    _clipboard.clear();
    if (tokens == 0) return;

    size_t distance_till_end = distance(_position, _text.end());
    size_t count = distance_till_end < tokens ? distance_till_end : tokens;

    if (_position == _text.begin()) {
      _clipboard.splice(_clipboard.begin(), _text, _position,
                        next(_position, count));
      _position = _text.begin();
    } else {
      auto step_back = prev(_position);
      _clipboard.splice(_clipboard.begin(), _text, _position,
                        next(_position, count));
      _position = next(step_back);
    }
  }

  void Copy(size_t tokens = 1) {
    _clipboard.clear();

    if (tokens == 0) return;
    size_t distance_till_end = distance(_position, _text.end());
    size_t count = distance_till_end < tokens ? distance_till_end : tokens;
    _clipboard.insert(_clipboard.begin(), _position, next(_position, count));
  }

  void Paste() {
    _text.insert(_position, _clipboard.begin(), _clipboard.end());
  }

  string GetText() const { return string(_text.begin(), _text.end()); }

 private:
  list<char> _text;
  list<char>::iterator _position;
  list<char> _clipboard;
};

void TypeText(Editor& editor, const string& text) {
  for (char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for (size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }

  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for (char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}