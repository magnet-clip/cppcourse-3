#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iostream>
#include <numeric>

vector<string> SplitIntoWords(const string &line) {
  string_view line_view = line;

  vector<string> words;
  // remove leading spaces
  line_view.remove_prefix(
      min(line_view.find_first_not_of(' '), line_view.size()));

  // peek separate words
  while (!line_view.empty()) {
    auto not_space_pos = min(line_view.find_first_of(' '), line_view.size());
    auto word = string(line_view.substr(0, not_space_pos));
    line_view.remove_prefix(not_space_pos);

    words.push_back(word);

    auto space_pos = min(line_view.find_first_not_of(' '), line_view.size());
    line_view.remove_prefix(space_pos);
  }

  return words;
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(current_document);
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  vector<size_t> docid_count(50000);
  vector<size_t> docid_positions(50000);

  for (string current_query; getline(query_input, current_query); ) {
    docid_count.clear();
    iota(docid_positions.begin(), docid_positions.end(), 0);

    const auto words = SplitIntoWords(current_query);

    for (const auto& word : words) {
      for (const size_t docid : index.Lookup(word)) {
        docid_count[docid]++;
      }
    }

    sort(
      begin(docid_positions),
      end(docid_positions),
      [&docid_count](size_t lhs_docid, size_t rhs_docid) {
        auto lhs_hit_count = docid_count[lhs_docid];
        auto rhs_hit_count = docid_count[rhs_docid];
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto docid : Head(docid_positions, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << docid_count[docid] << '}';
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
