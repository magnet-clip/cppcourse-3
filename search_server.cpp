#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

vector<string> SplitIntoWords(string_view line) {
  vector<string> words;
//  array<string, 1000> words; // does not help, stack overhead
  words.reserve(1000); // reservation seems only to increase total time
  // remove leading spaces
  line.remove_prefix(min(line.find_first_not_of(' '), line.size()));
  // peek separate words
//  size_t word_counter = 0;
  while (!line.empty()) {
    auto not_space_pos = min(line.find_first_of(' '), line.size());
    auto word = string(line.substr(0, not_space_pos));
    line.remove_prefix(not_space_pos);

    words.push_back(word);
//    word_counter++;
    auto space_pos = min(line.find_first_not_of(' '), line.size());
    line.remove_prefix(space_pos);
  }
//  words.resize(word_counter);
  return words;//{make_move_iterator(words.begin()), make_move_iterator(words.begin()+word_counter)};
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
    const auto words = SplitIntoWords(move(current_query));

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
    for (auto[docid, hitcount] : Head(search_results, 5)) {
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

const InvertedIndex::DocIndex &InvertedIndex::Lookup(const string &word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return none;
  }
}
