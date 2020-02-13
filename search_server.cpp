#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iostream>
#include <vector>

vector<string> SplitIntoWords(const string_view &line) {
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

SearchServer::SearchServer(istream &document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream &document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document);) {
    new_index.Add(current_document);
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(istream &query_input,
                                    ostream &search_results_output) {
  // array<pair<size_t, size_t>, 50000> search_results;
  // vector<pair<size_t, size_t>> search_results(50000);

  for (string current_query; getline(query_input, current_query);) {
    // search_results.fill({0, 0});
    // pair<size_t, size_t> zero(0, 0);
    // std::fill(search_results.begin(), search_results.end(), zero);
    const auto words = SplitIntoWords(current_query);

    Map<size_t, size_t> docid_count;
    for (const auto &word : words) {
      for (const size_t docid : index.Lookup(word)) {
        // search_results[docid].first = docid;
        // search_results[docid].second++;
        docid_count[docid]++;
      }
    }

    vector<pair<size_t, size_t>> search_results(docid_count.begin(),
                                                docid_count.end());

    // partial_sort
    sort(begin(search_results), end(search_results),
         [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
           int64_t lhs_docid = lhs.first;
           auto lhs_hit_count = lhs.second;
           int64_t rhs_docid = rhs.first;
           auto rhs_hit_count = rhs.second;
           return make_pair(lhs_hit_count, -lhs_docid) >
                  make_pair(rhs_hit_count, -rhs_docid);
         });

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      if (hitcount != 0) {
        search_results_output << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(const string_view &document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto &word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

const list<size_t> &InvertedIndex::Lookup(const string &word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return none;
  }
}
