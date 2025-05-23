#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <map>
#include "IntHashMap.h"

class DriverTable {
public:
    // Структура для передачи информации о водителе
    struct DriverInfo {
        int    id;
        std::string fullName;
        std::string birthDate;
        int    cityId;
    };

    // Конструктор / деструктор
    DriverTable();
    ~DriverTable();

    // Основные операции
    void loadFromFile();
    void saveToFile() const;
    void addDriver(const std::string& fullName,
        const std::string& birthDate,
        int cityId);
    void deleteDriver(const std::string& fullName);

    // Новый метод: удаление по ID
    void deleteDriverById(int id);

    // Итератор по списку водителей
    void driverIteratorReset() const;
    bool driverIteratorHasNext() const;
    DriverInfo driverIteratorNext() const;

    // Геттеры
    int getDriverId(const std::string& fullName) const;
    int getCityIdForDriver(const std::string& fullName) const;

    // Обновление ссылок при удалении города
    void updateCityReferences(int deletedCityId);

    bool updateDriverCity(const std::string& fullName, int newCityId);

    std::string getDriverNameById(int id) const;

private:
    // Узел списка водителей
    struct DriverNode {
        int    id;
        std::string fullName;
        std::string birthDate;
        int    cityId;
        DriverNode* next;
        DriverNode(int id, const std::string& fullName, const std::string& birthDate,
            int cityId, DriverNode* next)
            : id(id), fullName(fullName), birthDate(birthDate),
            cityId(cityId), next(next) {
        }
    };

    // Основные поля
    DriverNode* head;               // заголовочный узел
    IntHashMap idToDriverMap;       // хеш-таблица для быстрого поиска по ID
    std::map<std::string, int> nameToIdMap; // карта «ФИО → ID»
    mutable DriverNode* currentIterator; // итератор

    // Ширины колонок (для форматированного вывода)
    int idWidth, nameWidth, birthDateWidth, cityIdWidth;

    // Вспомогательные методы
    void parseLine(const std::string& line);
    void addDriverNode(int id, const std::string& fullName,
        const std::string& birthDate, int cityId);
    bool validateName(const std::string& name) const;
    bool validateDate(const std::string& date) const;
};