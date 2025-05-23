#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include "IntHashMap.h"

class FineTable {
public:
    // Уровень тяжести штрафа
    enum class Severity { LIGHT, MEDIUM, HEAVY };

    // Структура для передачи информации о штрафе
    struct FineInfo {
        int    id;
        double amount;
        std::string type;
        Severity severity;
    };

    // Конструктор / деструктор
    FineTable();
    ~FineTable();

    // Основные операции
    void loadFromFile();
    void saveToFile() const;
    void addFine(const std::string& type, double amount,
        Severity severity = Severity::LIGHT);
    void deleteFine(const std::string& type);

    // Итератор по списку штрафов
    void fineIteratorReset() const;
    bool fineIteratorHasNext() const;
    FineInfo fineIteratorNext() const;

    // Геттеры
    int getFineIdByType(const std::string& type) const;
    double getAmountById(int id) const;

    // Преобразование Severity в строку
    static std::string severityToString(Severity severity);

    std::string getFineTypeById(int id) const;

private:
    // Узел списка штрафов
    struct FineNode {
        int    id;
        double amount;
        std::string type;
        Severity severity;
        FineNode* next;
        FineNode(int id, double amount, const std::string& type,
            Severity severity, FineNode* next)
            : id(id), amount(amount), type(type),
            severity(severity), next(next) {
        }
    };

    // Основные поля
    FineNode* head;                 // заголовочный узел
    IntHashMap idToFineMap;         // хеш-таблица для быстрого поиска по ID
    std::map<std::string, int> typeToIdMap; // карта «тип → ID»
    mutable FineNode* currentIterator; // итератор

    // Ширины колонок (для форматированного вывода)
    int idWidth, amountWidth, typeWidth, severityWidth;

    // Вспомогательные методы
    void parseLine(const std::string& line);
    void addFineNode(int id, double amount, const std::string& type,
        Severity severity);
};