#include "server.h"
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <future>

std::map<std::string, size_t> SearchServer::get_indexes_for_request_words(const std::vector<std::string>& words) {
    std::map<std::string, size_t> freq_map;
    for (const auto& word : words) {
        freq_map[word] = _index->getWordCount(word).size();
    }
    return freq_map;
}

void SearchServer::handleRequest(const std::string& request, std::vector<RelativeIndex>& result, size_t max_responses) {
    std::istringstream iss(request);
    std::string word;
    std::vector<std::string> words;
    while (iss >> word) words.push_back(word);

    auto freq_map = get_indexes_for_request_words(words);
    std::sort(words.begin(), words.end(), [&freq_map](const std::string& a, const std::string& b) {
        return freq_map[a] < freq_map[b];
        });

    std::map<size_t, float> doc_abs_rank;
    std::set<size_t> current_docs;
    bool first_word = true;

    for (const auto& w : words) {
        auto entries = _index->getWordCount(w);
        std::set<size_t> docs_for_word;
        for (const auto& e : entries) docs_for_word.insert(e._doc_id);

        if (first_word) {
            current_docs = docs_for_word;
            first_word = false;
        }
        else {
            std::set<size_t> intersection;
            std::set_intersection(current_docs.begin(), current_docs.end(),
                docs_for_word.begin(), docs_for_word.end(),
                std::inserter(intersection, intersection.begin()));
            current_docs = intersection;
        }

        for (const auto& e : entries) {
            if (current_docs.count(e._doc_id)) {
                doc_abs_rank[e._doc_id] += e._count;
            }
        }
    }

    if (current_docs.empty()) return;

    float max_rank = 0.0f;
    for (const auto& [doc_id, abs_rank] : doc_abs_rank)
        if (abs_rank > max_rank) max_rank = abs_rank;

    std::vector<RelativeIndex> relative;
    for (const auto& [doc_id, abs_rank] : doc_abs_rank) {
        relative.emplace_back(doc_id, abs_rank / max_rank);
    }

    std::sort(relative.begin(), relative.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
        return a._rank > b._rank;
        });

    if (relative.size() > max_responses)
        relative.resize(max_responses);

    result = relative;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, size_t max_responses) {
    std::vector<std::vector<RelativeIndex>> results(queries_input.size());
    std::vector<std::future<void>> futures;

    for (size_t i = 0; i < queries_input.size(); ++i) {
        futures.push_back(std::async(std::launch::async, [this, &queries_input, &results, i, max_responses]() {
            handleRequest(queries_input[i], results[i], max_responses);
            }));
    }

    // Ждём завершения всех потоков
    for (auto& fut : futures) fut.get();

    return results;
}
