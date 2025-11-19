#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <mutex>

struct Entry {
    size_t _doc_id, _count;
    Entry(size_t doc_id, size_t count) : _doc_id(doc_id), _count(count)
    {
    }
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (_doc_id == other._doc_id &&
                _count == other._count);
    }
};



class InvertedIndex {
public:
    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex& other) : freq_dictionary(other.freq_dictionary){

    }

    /**
     * Обновить или заполнить базу документов, по которой будем совершать поиск
     * @param texts_input содержимое документов
     */
    void updateDocumentBase(const std::vector<std::string>& input_docs);

    /**
     * Метод определяет количество вхождений слова word в загруженной базе документов
     * @param word слово, частоту вхождений которого необходимо определить
     * @return возвращает подготовленный список с частотой слов
     */
    std::vector<Entry> getWordCount(const std::string& word);

    void updateDocument(const std::string& word, size_t doc_id);
protected:
    std::mutex _dictionary_mtx;
    std::unordered_map<std::string, std::map<size_t, size_t>> freq_dictionary; // частотный словарь
};
