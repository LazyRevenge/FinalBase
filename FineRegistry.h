#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include "IntHashMap.h"
#include "DriverTable.h"
#include "CityTable.h"
#include "FineTable.h"

class FineRegistry {
public:
    // Структура для передачи информации о нарушении
    struct ViolationInfo {
        int    recordId;
        int    driverId;
        int    cityId;
        int    fineId;
        bool   paid;
        std::string date;

        // Детальная информация
        std::string driverName;
        std::string cityName;
        std::string fineType;
        double fineAmount;
    };

    ViolationInfo getViolationById(int recordId,
        const DriverTable& drivers,
        const CityTable& cities,
        const FineTable& fines) const;

    // Конструктор / деструктор
    FineRegistry();
    ~FineRegistry();

    // Основные операции
    void loadFromFile();
    void saveToFile() const;
    void addViolation(int driverId, int cityId, int fineId, const std::string& date);
    void markAsPaid(int recordId);

    // Итератор по списку нарушений (возвращает подробную информацию)
    void violationIteratorReset() const;
    bool violationIteratorHasNext() const;
    ViolationInfo violationIteratorNext(const DriverTable& drivers,
        const CityTable& cities,
        const FineTable& fines) const;

    // Обновление ссылок при удалении водителя или города
    void updateDriverReferences(int deletedDriverId);
    void updateCityReferences(int deletedCityId);

    void updateViolationsCity(int driverId, int newCityId);

private:
    // Узел списка нарушений
    struct ViolationNode {
        int    recordId;
        int    driverId;
        int    cityId;
        int    fineId;
        bool   paid;
        std::string date;
        ViolationNode* next;
        ViolationNode(int recordId, int driverId, int cityId,
            int fineId, bool paid, const std::string& date,
            ViolationNode* next)
            : recordId(recordId), driverId(driverId), cityId(cityId),
            fineId(fineId), paid(paid), date(date), next(next) {
        }
    };

    // Основные поля
    ViolationNode* head;             // заголовочный узел
    IntHashMap recordToNodeMap;      // хеш-таблица для быстрого поиска по recordId
    mutable ViolationNode* currentIterator; // итератор

    // Ширины колонок (для форматированного вывода)
    int recordIdWidth, driverIdWidth, cityIdWidth, fineIdWidth, paidWidth, dateWidth;

    // Вспомогательные методы
    void parseLine(const std::string& line);
    void addViolationNode(int recordId, int driverId, int cityId,
        int fineId, bool paid, const std::string& date);
};