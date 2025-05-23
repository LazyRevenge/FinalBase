#pragma once
#include "CityTable.h"
#include "DriverTable.h"
#include "FineTable.h"
#include "FineRegistry.h"

#include <string>
#include <vector>

class DatabaseManager {
    CityTable      cities;
    DriverTable    drivers;
    FineTable      fines;
    FineRegistry   registry;

public:
    void loadAll();
    void saveAll();

    void addCity(const std::string& name,
        int population,
        CityTable::PopulationGrade grade,
        CityTable::SettlementType type);
    void deleteCity(const std::string& name);

    void addDriver(const std::string& fullName,
        const std::string& birthDate,
        const std::string& cityName);
    void deleteDriver(const std::string& name);

    // Íîâûé ìåòîä: óäàëåíèå ïî ID
    void deleteDriverById(int driverId);

    void addFine(const std::string& type,
        double amount,
        FineTable::Severity severity = FineTable::Severity::LIGHT);
    void addViolation(const std::string& driverName,
        const std::string& fineType,
        const std::string& date);

    void markFineAsPaid(int recordId);

    std::vector<FineRegistry::ViolationInfo> getAllViolations();

    CityTable& getCities() { return cities; }
    DriverTable& getDrivers() { return drivers; }
    FineTable& getFines() { return fines; }
    FineRegistry& getRegistry() { return registry; }
}﻿;