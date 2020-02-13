#pragma once
#include "utils.h"

#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

class InvertedIndex {
public:
  void Add(const string_view &document);

  const list<size_t> &Lookup(const string &word) const;

  const string_view &GetDocument(size_t id) const { return docs[id]; }

private:
  Map<string, list<size_t>> index;
  vector<string_view> docs;
  list<size_t> none;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream &document_input);
  void UpdateDocumentBase(istream &document_input);
  void AddQueriesStream(istream &query_input, ostream &search_results_output);

private:
  InvertedIndex index;
};
