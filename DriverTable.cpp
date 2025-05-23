#include "DriverTable.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
using namespace std;

// Конструктор: инициализация заголовочного узла и загрузка данных
DriverTable::DriverTable()
    : head(new DriverNode(-1, "", "", -1, nullptr)),
    currentIterator(nullptr),
    idWidth(5),
    nameWidth(30),
    birthDateWidth(12),
    cityIdWidth(5)
{
    loadFromFile();
}

// Деструктор: очистка списка и хеш-таблицы
DriverTable::~DriverTable() {
    DriverNode* current = head->next;
    while (current) {
        DriverNode* temp = current;
        current = current->next;
        delete temp;
    }
    delete head;
    idToDriverMap.clear();
    nameToIdMap.clear();
}

// Загрузка данных из файла
void DriverTable::loadFromFile() {
    ifstream file("drivers.txt");
    if (!file.is_open()) {
        cerr << "Error opening drivers file!" << endl;
        return;
    }
    // Очищаем существующие данные
    DriverNode* current = head->next;
    while (current) {
        DriverNode* temp = current;
        current = current->next;
        delete temp;
    }
    head->next = nullptr;
    idToDriverMap.clear();
    nameToIdMap.clear();

    string line;
    while (getline(file, line)) {
        parseLine(line);
    }
    file.close();
}

// Парсинг строки: "id \"fullName\" \"birthDate\" cityId"
void DriverTable::parseLine(const std::string& line) {
    if (line.empty()) return;
    istringstream iss(line);
    int id, cityId;
    string fullName, birthDate;
    iss >> id >> quoted(fullName) >> quoted(birthDate) >> cityId;

    addDriverNode(id, fullName, birthDate, cityId);
}

// Добавление узла в список и в хеш-таблицу
void DriverTable::addDriverNode(int id, const std::string& fullName,
    const std::string& birthDate, int cityId)
{
    DriverNode* newNode = new DriverNode(id, fullName, birthDate, cityId, head->next);
    head->next = newNode;
    idToDriverMap.insert(id, newNode);
    nameToIdMap[fullName] = id;
}

// Валидация ФИО (только буквы и пробелы)
bool DriverTable::validateName(const std::string& name) const {
    static const regex pattern("^[А-Яа-яA-Za-z\\s]+$");
    return regex_match(name, pattern);
}

// Валидация даты (формат ДД.MM.ГГГГ)
bool DriverTable::validateDate(const std::string& date) const {
    static const regex pattern("^\\d{2}\\.\\d{2}\\.\\d{4}$");
    return regex_match(date, pattern);
}

// Добавление водителя (с валидацией)
void DriverTable::addDriver(const std::string& fullName,
    const std::string& birthDate, int cityId)
{
    if (!validateName(fullName))
        throw invalid_argument("Недопустимые символы в ФИО");
    if (!validateDate(birthDate))
        throw invalid_argument("Неправильный формат даты");

    // Генерация ID: максимум существующего +1
    int newId = 1;
    for (auto pair : nameToIdMap) {
        if (pair.second >= newId) newId = pair.second + 1;
    }

    addDriverNode(newId, fullName, birthDate, cityId);
}

// Новый метод: удаление водителя по ID
void DriverTable::deleteDriverById(int id) {
    DriverNode* prev = head;
    DriverNode* curr = head->next;
    while (curr) {
        if (curr->id == id) {
            prev->next = curr->next;
            idToDriverMap.remove(id);
            nameToIdMap.erase(curr->fullName);
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// Итератор: сброс на начало
void DriverTable::driverIteratorReset() const {
    currentIterator = head->next;
}

// Есть ли следующий?
bool DriverTable::driverIteratorHasNext() const {
    return currentIterator != nullptr;
}

// Получение следующего
DriverTable::DriverInfo DriverTable::driverIteratorNext() const {
    DriverInfo info;
    if (!currentIterator) return info;
    info.id = currentIterator->id;
    info.fullName = currentIterator->fullName;
    info.birthDate = currentIterator->birthDate;
    info.cityId = currentIterator->cityId;
    currentIterator = currentIterator->next;
    return info;
}

// Геттер: получить ID по ФИО
int DriverTable::getDriverId(const std::string& fullName) const {
    auto it = nameToIdMap.find(fullName);
    return (it != nameToIdMap.end()) ? it->second : -1;
}

// Геттер: получить ID города для водителя
int DriverTable::getCityIdForDriver(const std::string& fullName) const {
    int id = getDriverId(fullName);
    if (id == -1) return -1;
    DriverNode* node = idToDriverMap.find<DriverNode>(id);
    return node ? node->cityId : -1;
}

std::string DriverTable::getDriverNameById(int id) const {
    DriverNode* node = idToDriverMap.find<DriverNode>(id);
    return node ? node->fullName : "";
}

// Удаление водителя по ФИО
void DriverTable::deleteDriver(const std::string& fullName) {
    auto it = nameToIdMap.find(fullName);
    if (it == nameToIdMap.end()) return;

    int id = it->second;
    deleteDriverById(id);
}

// Обновление ссылок при удалении города (устанавливаем cityId = -1)
void DriverTable::updateCityReferences(int deletedCityId) {
    DriverNode* curr = head->next;
    while (curr) {
        if (curr->cityId == deletedCityId) {
            curr->cityId = -1;
        }
        curr = curr->next;
    }
}

bool DriverTable::updateDriverCity(const std::string& fullName, int newCityId) {
    bool updated = false;
    DriverNode* current = head->next;
    while (current) {
        if (current->fullName == fullName) {
            current->cityId = newCityId;
            updated = true;
        }
        current = current->next;
    }
    return updated;
}

// Сохранение данных в файл
void DriverTable::saveToFile() const {
    ofstream file("drivers.txt");
    if (!file.is_open()) {
        cerr << "Error opening drivers file for writing!" << endl;
        return;
    }
    DriverNode* curr = head->next;
    while (curr) {
        file << curr->id << ' '
            << quoted(curr->fullName) << ' '
            << quoted(curr->birthDate) << ' '
            << curr->cityId << '\n';
        curr = curr->next;
    }
    file.close();
}