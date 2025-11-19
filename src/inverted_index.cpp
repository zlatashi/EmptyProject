#include "inverted_index.h"
#include <sstream>

void InvertedIndex::updateDocumentBase(const std::vector<std::string>& input_docs) {
    std::lock_guard<std::mutex> lock(_dictionary_mtx);
    freq_dictionary.clear();

    for (size_t doc_id = 0; doc_id < input_docs.size(); ++doc_id) {
        std::istringstream ss(input_docs[doc_id]);
        std::string word;
        std::map<std::string, size_t> word_count;

        while (ss >> word) {
            ++word_count[word];
        }

        for (const auto& [w, count] : word_count) {
            freq_dictionary[w][doc_id] = count;
        }
    }
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string& word) {
    std::vector<Entry> result;

    std::lock_guard<std::mutex> lock(_dictionary_mtx);
    if (freq_dictionary.count(word)) {
        for (const auto& [doc_id, count] : freq_dictionary[word]) {
            result.emplace_back(doc_id, count);
        }
    }
    return result;
}

void InvertedIndex::updateDocument(const std::string& word, size_t doc_id) {
    std::lock_guard<std::mutex> lock(_dictionary_mtx);
    ++freq_dictionary[word][doc_id];
}
