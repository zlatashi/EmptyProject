#include "converter.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

ConverterJSON::ConverterJSON(const std::string& config_path,
    const std::string& request_path,
    const std::string& answers_path)
    : _config_path(config_path), _request_path(request_path), _answers_path(answers_path) {

    std::ifstream config_file(_config_path);
    if (!config_file.is_open()) {
        throw std::runtime_error("Config file is missing: " + _config_path);
    }

    json config_json;
    try {
        config_file >> config_json;
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("Invalid JSON syntax in config file: " + _config_path);
    }

    if (!config_json.contains("config") || !config_json.contains("files")) {
        throw std::runtime_error("Config file missing required fields: " + _config_path);
    }

    _name = config_json["config"]["name"];
    _version = config_json["config"]["version"];
    _max_responses = config_json["config"].value("max_responses", 5);

    for (auto& file_path : config_json["files"]) {
        _file_names.push_back(file_path);
    }
}

std::vector<std::string> ConverterJSON::getTextDocuments() {
    std::vector<std::string> result;
    for (auto& path : _file_names) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Cannot open file: " << path << std::endl;
            result.push_back("");
            continue;
        }

        try {
            std::string content((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            result.push_back(content);
        }
        catch (...) {
            std::cerr << "Error reading file: " << path << std::endl;
            result.push_back("");
        }
    }
    return result;
}

std::vector<std::string> ConverterJSON::getRequests() {
    std::ifstream file(_request_path);
    if (!file.is_open()) {
        throw std::runtime_error("Request file is missing: " + _request_path);
    }

    json req_json;
    try {
        file >> req_json;
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("Invalid JSON syntax in request file: " + _request_path);
    }

    if (!req_json.contains("requests")) {
        throw std::runtime_error("Request file missing 'requests' field: " + _request_path);
    }

    std::vector<std::string> requests;
    for (auto& req : req_json["requests"]) {
        requests.push_back(req.get<std::string>());
    }
    return requests;
}

int ConverterJSON::getResponsesLimit() {
    return _max_responses;
}

const std::string& ConverterJSON::getName() const {
    return _name;
}

const std::string& ConverterJSON::getVersion() const {
    return _version;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>& answers) {
    std::ofstream file(_answers_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create answers file: " + _answers_path);
    }

    json j;
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string req_id = "request" + std::to_string(i + 1);
        while (req_id.length() < 10) req_id.insert(7, "0");

        if (answers[i].empty()) {
            j["answers"][req_id]["result"] = false;
        }
        else {
            j["answers"][req_id]["result"] = true;
            for (auto& r : answers[i]) {
                j["answers"][req_id]["relevance"].push_back({
                    {"docid", r._doc_id},
                    {"rank", r._rank}
                    });
            }
        }
    }

    file << std::setw(4) << j;
}
