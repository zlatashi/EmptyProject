#pragma once
#include <string>
#include <vector>
#include "server.h"

/**
*  Класс для работы с JSON-файлами
*/
class ConverterJSON {
    std::string _config_path;
    std::string _request_path;
    std::string _answers_path;

    std::string _name;
    std::string _version;
    int _max_responses;
    std::vector<std::string> _file_names;
public:
    ConverterJSON(const std::string&  config_path = "config.json",
                  const std::string& request_path = "request.json",
                  const std::string& answers_path = "answers.json"
                  );

    /**
     * Метод получения содержимого файлов
     * @return Возвращает список с содержимым файлов перечисленных
     *       в config.json
     */
    std::vector<std::string> getTextDocuments();

    /**
     * Метод считывает поле max_responses для определения предельного
     *  количества ответов на один запрос
     * @return
     */
    int getResponsesLimit();

    /**
     * Метод получения запросов из файла requests.json
     * @return возвращает список запросов из файла requests.json
     */
    std::vector<std::string> getRequests();

    /**
     * Положить в файл answers.json результаты поисковых запросов
     */
    void putAnswers(std::vector<std::vector<RelativeIndex>>& answers);

    const std::string &getName() const;

    const std::string &getVersion() const;
};
