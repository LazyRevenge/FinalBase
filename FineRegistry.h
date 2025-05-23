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
    // ��������� ��� �������� ���������� � ���������
    struct ViolationInfo {
        int    recordId;
        int    driverId;
        int    cityId;
        int    fineId;
        bool   paid;
        std::string date;

        // ��������� ����������
        std::string driverName;
        std::string cityName;
        std::string fineType;
        double fineAmount;
    };

    ViolationInfo getViolationById(int recordId,
        const DriverTable& drivers,
        const CityTable& cities,
        const FineTable& fines) const;

    // ����������� / ����������
    FineRegistry();
    ~FineRegistry();

    // �������� ��������
    void loadFromFile();
    void saveToFile() const;
    void addViolation(int driverId, int cityId, int fineId, const std::string& date);
    void markAsPaid(int recordId);

    // �������� �� ������ ��������� (���������� ��������� ����������)
    void violationIteratorReset() const;
    bool violationIteratorHasNext() const;
    ViolationInfo violationIteratorNext(const DriverTable& drivers,
        const CityTable& cities,
        const FineTable& fines) const;

    // ���������� ������ ��� �������� �������� ��� ������
    void updateDriverReferences(int deletedDriverId);
    void updateCityReferences(int deletedCityId);

    void updateViolationsCity(int driverId, int newCityId);

private:
    // ���� ������ ���������
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

    // �������� ����
    ViolationNode* head;             // ������������ ����
    IntHashMap recordToNodeMap;      // ���-������� ��� �������� ������ �� recordId
    mutable ViolationNode* currentIterator; // ��������

    // ������ ������� (��� ���������������� ������)
    int recordIdWidth, driverIdWidth, cityIdWidth, fineIdWidth, paidWidth, dateWidth;

    // ��������������� ������
    void parseLine(const std::string& line);
    void addViolationNode(int recordId, int driverId, int cityId,
        int fineId, bool paid, const std::string& date);
};