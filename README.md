# Search Engine Project

Компактный многопоточный поисковый движок на C++17 с поддержкой JSON и ранжированием результатов.

## Технологии
- C++17
- Google Test
- nlohmann/json (JSON for Modern C++)

## Структура проекта

**Корень проекта**
- `CMakeLists.txt` – главный CMake для сборки проекта
- `config/` – конфигурационные файлы
  - `config.json` – настройки и список файлов для поиска
  - `requests.json` – поисковые запросы
  - `answers.json` – результаты поиска
- `include/` – заголовочные файлы
  - `converter_json.h`
  - `inverted_index.h`
  - `search_server.h`
- `src/` – исходные файлы
  - `converter_json.cpp`
  - `inverted_index.cpp`
  - `search_server.cpp`
  - `main.cpp`
- `tests/` – тесты
  - `CMakeLists.txt` – CMake для тестов
  - `test_index.cpp`
  - `test_server.cpp`
- `resources/` – текстовые файлы для поиска
  - `file001.txt`
  - `file002.txt`
  - `file003.txt`
  - `file004.txt`

## Особенности
- Поддержка многопоточной обработки запросов;
- Проверка синтаксиса JSON и информативные ошибки;
- Ранжирование документов по релевантности;
- Безопасная работа с файлами (проверка открытия)

## Сборка и запуск
```bash
mkdir build
cd build
cmake ..
cmake --build .
./search_server
