#include "DataBaseManager.h"
#include <stdexcept>

void DatabaseManager::loadAll() {
    cities.loadFromFile();
    drivers.loadFromFile();
    fines.loadFromFile();
    registry.loadFromFile();
}

void DatabaseManager::saveAll() {
    cities.saveToFile();
    drivers.saveToFile();
    fines.saveToFile();
    registry.saveToFile();
}

void DatabaseManager::addCity(const std::string& name,
    int population,
    CityTable::PopulationGrade grade,
    CityTable::SettlementType type)
{
    cities.addCity(name, population, grade, type);
}

void DatabaseManager::deleteCity(const std::string& name) {
    int cityId = cities.getCityIdByName(name);
    if (cityId == -1) return;

    cities.deleteCity(name);
    drivers.updateCityReferences(cityId);
    registry.updateCityReferences(cityId);
    saveAll();
}

void DatabaseManager::addDriver(const std::string& fullName,
    const std::string& birthDate,
    const std::string& cityName)
{
    int cityId = cities.getCityIdByName(cityName);
    if (cityId == -1) {
        throw std::invalid_argument("Город отсутствует");
    }

    drivers.addDriver(fullName, birthDate, cityId);
    saveAll();
}

void DatabaseManager::deleteDriver(const std::string& name) {
    int driverId = drivers.getDriverId(name);
    if (driverId == -1) return;

    drivers.deleteDriver(name);
    registry.updateDriverReferences(driverId);
    saveAll();
}

void DatabaseManager::deleteDriverById(int driverId) {
    drivers.deleteDriverById(driverId);
    registry.updateDriverReferences(driverId);
    saveAll();
}

void DatabaseManager::addFine(const std::string& type,
    double amount,
    FineTable::Severity severity)
{
    fines.addFine(type, amount, severity);
}

void DatabaseManager::addViolation(const std::string& driverName,
    const std::string& fineType,
    const std::string& date)
{
    int driverId = drivers.getDriverId(driverName);
    int cityId = drivers.getCityIdForDriver(driverName);
    int fineId = fines.getFineIdByType(fineType);

    if (driverId == -1 || cityId == -1 || fineId == -1)
        throw std::invalid_argument("Неверные данные для нарушения");

    registry.addViolation(driverId, cityId, fineId, date);
}

void DatabaseManager::markFineAsPaid(int recordId) {
    registry.markAsPaid(recordId);
}

std::vector<FineRegistry::ViolationInfo> DatabaseManager::getAllViolations() {
    std::vector<FineRegistry::ViolationInfo> violations;
    registry.violationIteratorReset();
    while (registry.violationIteratorHasNext()) {
        violations.push_back(
            registry.violationIteratorNext(drivers, cities, fines)
        );
    }
    return violations;
}
