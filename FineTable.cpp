#include "FineTable.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
using namespace std;

// Конструктор: инициализация заголовочного узла и загрузка данных
FineTable::FineTable()
    : head(new FineNode(-1, 0.0, "", Severity::LIGHT, nullptr)),
    currentIterator(nullptr),
    idWidth(5),
    amountWidth(10),
    typeWidth(30),
    severityWidth(8)
{
    loadFromFile();
}

// Деструктор: очистка списка и хеш-таблицы
FineTable::~FineTable() {
    FineNode* current = head->next;
    while (current) {
        FineNode* temp = current;
        current = current->next;
        delete temp;
    }
    delete head;
    idToFineMap.clear();
    typeToIdMap.clear();
}

// Загрузка данных из файла
void FineTable::loadFromFile() {
    ifstream file("fines.txt");
    if (!file.is_open()) {
        cerr << "Error opening fines file!" << endl;
        return;
    }
    // Очищаем существующие данные
    FineNode* current = head->next;
    while (current) {
        FineNode* temp = current;
        current = current->next;
        delete temp;
    }
    head->next = nullptr;
    idToFineMap.clear();
    typeToIdMap.clear();

    string line;
    while (getline(file, line)) {
        parseLine(line);
    }
    file.close();
}

// Парсинг строки: "id amount \"type\" \"severity\""
void FineTable::parseLine(const std::string& line) {
    if (line.empty()) return;
    istringstream iss(line);
    int id;
    double amount;
    string type, severityStr;
    iss >> id >> amount >> quoted(type) >> quoted(severityStr);

    Severity severity = Severity::LIGHT;
    if (severityStr == "Medium") severity = Severity::MEDIUM;
    else if (severityStr == "Heavy") severity = Severity::HEAVY;

    addFineNode(id, amount, type, severity);
}

// Добавление узла в список и в хеш-таблицу
void FineTable::addFineNode(int id, double amount, const std::string& type,
    Severity severity)
{
    FineNode* newNode = new FineNode(id, amount, type, severity, head->next);
    head->next = newNode;
    idToFineMap.insert(id, newNode);
    typeToIdMap[type] = id;
}

// Добавление нового штрафа (с генерацией ID)
void FineTable::addFine(const std::string& type, double amount,
    Severity severity)
{
    if (typeToIdMap.count(type))
        throw invalid_argument("Штраф с таким типом уже существует");

    // Генерация ID: максимум существующего +1
    int newId = 1;
    for (const auto& pair : typeToIdMap) {
        if (pair.second >= newId) newId = pair.second + 1;
    }

    addFineNode(newId, amount, type, severity);
}

// Итератор: сброс на начало
void FineTable::fineIteratorReset() const {
    currentIterator = head->next;
}

// Есть ли следующий?
bool FineTable::fineIteratorHasNext() const {
    return currentIterator != nullptr;
}

// Получение следующего
FineTable::FineInfo FineTable::fineIteratorNext() const {
    FineInfo info;
    if (!currentIterator) return info;
    info.id = currentIterator->id;
    info.amount = currentIterator->amount;
    info.type = currentIterator->type;
    info.severity = currentIterator->severity;
    currentIterator = currentIterator->next;
    return info;
}

std::string FineTable::getFineTypeById(int id) const {
    FineNode* node = idToFineMap.find<FineNode>(id);
    return node ? node->type : "";
}

// Геттер: получить ID по типу штрафа
int FineTable::getFineIdByType(const std::string& type) const {
    auto it = typeToIdMap.find(type);
    return (it != typeToIdMap.end()) ? it->second : -1;
}

// Геттер: получить сумму по ID
double FineTable::getAmountById(int id) const {
    FineNode* node = idToFineMap.find<FineNode>(id);
    return node ? node->amount : 0.0;
}

// Удаление штрафа по типу (только при отсутствии связанных нарушений)
void FineTable::deleteFine(const std::string& type) {
    auto it = typeToIdMap.find(type);
    if (it == typeToIdMap.end()) return;

    int id = it->second;
    FineNode* prev = head;
    FineNode* curr = head->next;
    while (curr) {
        if (curr->id == id) {
            prev->next = curr->next;
            idToFineMap.remove(id);
            typeToIdMap.erase(type);
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// Сохранение данных в файл
void FineTable::saveToFile() const {
    ofstream file("fines.txt");
    if (!file.is_open()) {
        cerr << "Error opening fines file for writing!" << endl;
        return;
    }
    FineNode* curr = head->next;
    while (curr) {
        file << curr->id << ' '
            << curr->amount << ' '
            << quoted(curr->type) << ' '
            << quoted(severityToString(curr->severity))
            << '\n';
        curr = curr->next;
    }
    file.close();
}

// Преобразование Severity в строку
std::string FineTable::severityToString(Severity severity) {
    switch (severity) {
    case Severity::LIGHT:  return "Light";
    case Severity::MEDIUM: return "Medium";
    case Severity::HEAVY:  return "Heavy";
    default:               return "Unknown";
    }
}