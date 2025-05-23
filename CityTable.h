#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include "IntHashMap.h"
#include "Filters.h"
#include <map>

class CityTable {
public:
    // Узел списка городов (сделан публичным для доступа из UI)
    struct CityNode {
        int    id;
        std::string name;
        int    population;
        enum class PopulationGrade { SMALL, MEDIUM, LARGE };
        enum class SettlementType { CITY, TOWN, VILLAGE };
        PopulationGrade grade;
        SettlementType  type;
        CityNode* next;
        CityNode(int id, const std::string& name, int population,
            PopulationGrade grade, SettlementType type, CityNode* next)
            : id(id), name(name), population(population),
            grade(grade), type(type), next(next) {
        }
    };

    // Перечисления
    using PopulationGrade = CityNode::PopulationGrade;
    using SettlementType = CityNode::SettlementType;

    // Структура для передачи информации о городе
    struct CityInfo {
        int    id;
        std::string name;
        int    population;
        PopulationGrade grade;
        SettlementType  type;
    };


    // Конструктор / деструктор
    CityTable();
    ~CityTable();

    // Основные операции
    void loadFromFile();
    void saveToFile() const;
    void addCity(const std::string& name, int population,
        PopulationGrade grade, SettlementType type);
    void deleteCity(const std::string& name);

    // Работа с фильтрами
    void addFilter(const std::string& field, const std::string& pattern);
    void clearFilters();
    CityNode* applyFilters() const;

    // Преобразование перечислений в строку
    static std::string populationGradeToString(PopulationGrade grade);
    static std::string settlementTypeToString(SettlementType type);

    // Обновление ширины колонок (после загрузки)
    void updateColumnWidths();
    std::string formatNode(const CityNode* node) const;

    // Итератор по списку городов
    void cityIteratorReset(CityNode* start = nullptr) const;
    bool cityIteratorHasNext() const;
    CityInfo cityIteratorNext() const;

    bool cityExists(int cityId) const;

    // Геттеры
    std::string getCityNameById(int id) const;
    int         getCityIdByName(const std::string& name) const;
private:
    // Структура фильтра для поиска
    struct Filter {
        std::string field;
        std::string pattern;
        Filter* next;
    };

    // Основные поля
    CityNode* head;           // заголовочный узел
    IntHashMap idToCityMap;    // хеш-таблица для быстрого поиска по ID
    std::map<std::string, int> nameToIdMap; // карта «имя → ID»
    Filter* currentFilter;  // цепочка активных фильтров
    mutable CityNode* currentIterator; // текущая позиция итератора

    // Ширины колонок (для форматированного вывода)
    int idWidth, nameWidth, populationWidth, typeWidth;

    // Вспомогательные методы
    void parseLine(const std::string& line);
    void addCityNode(int id, const std::string& name, int population,
        PopulationGrade grade, SettlementType type);
    bool matchField(const CityNode* node,
        const std::string& field,
        const std::string& pattern) const;
    bool checkNumeric(int value, const std::string& pattern) const;
    CityNode* cloneNode(const CityNode* src) const;
    bool matches_filter(const CityNode* node, const Filter& f) const;
};
