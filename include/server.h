#pragma once
#include <string>
#include <vector>
#include "inverted_index.h"

struct RelativeIndex {
    size_t _doc_id = 0;
    float _rank = 0.0f;

    RelativeIndex() = default; // конструктор по умолчанию
    RelativeIndex(size_t doc_id, float rank) : _doc_id(doc_id), _rank(rank) {}

    bool operator == (const RelativeIndex& other) const {
        return (_doc_id == other._doc_id && _rank == other._rank);
    }
};

class SearchServer {
public:
    /**
     * @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
     *      чтобы SearchServer мог узнать частоту слов встречаемых в запросе
     */
    SearchServer(InvertedIndex& idx) : _index(&idx){ };
    /**
     * Метод обработки поисковых запросов
     * @param queries_input поисковые запросы взятые из файла requests.json
     * @return возвращает отсортированный список релевантных ответов для заданных запросов
     */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input, size_t max_responses = 5);
    std::map<std::string, size_t> get_indexes_for_request_words(std::vector<std::string>& vec);
    void handleRequest(const std::string& request, std::vector<RelativeIndex>& , size_t max_responses);
private:
    InvertedIndex* _index;
};