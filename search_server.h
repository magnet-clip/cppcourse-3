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
  using DocIndex = vector<size_t>;

  void Add(const string_view &document);

  const DocIndex &Lookup(const string &word) const;

  const string_view &GetDocument(size_t id) const { return docs[id]; }

private:
  Map<string, DocIndex> index; // word -> [documents]
  vector<string_view> docs;        // # (docid) -> document
  DocIndex none;
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
