#include "CityTable.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <iomanip>
using namespace std;

// Êîíñòðóêòîð: èíèöèàëèçàöèÿ çàãîëîâî÷íîãî óçëà è çàãðóçêà äàííûõ
CityTable::CityTable()
    : head(new CityNode(-1, "", 0, PopulationGrade::SMALL, SettlementType::CITY, nullptr)),
    currentFilter(nullptr),
    idWidth(5),
    nameWidth(20),
    populationWidth(12),
    typeWidth(10)
{
    loadFromFile();
    updateColumnWidths(); // Ïåðâîíà÷àëüíûé ðàñ÷åò øèðèíû ñòîëáöîâ
}

// Äåñòðóêòîð: î÷èñòêà ñïèñêà è ôèëüòðîâ
CityTable::~CityTable() {
    // Óäàëÿåì âñå óçëû ñïèñêà
    CityNode* curr = head->next;
    while (curr) {
        CityNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
    delete head;
    // Î÷èùàåì ôèëüòðû
    clearFilters();
}

// Çàãðóçêà äàííûõ èç ôàéëà
void CityTable::loadFromFile() {
    std::ifstream file("cities.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening cities file!" << std::endl;
        return;
    }
    // Î÷èùàåì ñóùåñòâóþùèå äàííûå
    CityNode* current = head->next;
    while (current) {
        CityNode* temp = current;
        current = current->next;
        delete temp;
    }
    head->next = nullptr;
    idToCityMap.clear();
    nameToIdMap.clear();

    std::string line;
    while (std::getline(file, line)) {
        parseLine(line);
    }
    updateColumnWidths(); // Îáíîâëÿåì øèðèíó ïîñëå çàãðóçêè
}

// Ïàðñèíã ñòðîêè: "id \"name\" population \"grade\" \"type\""
void CityTable::parseLine(const std::string& line) {
    if (line.empty()) return;
    std::istringstream iss(line);
    int id, population;
    std::string name, gradeStr, typeStr;
    iss >> id >> std::quoted(name) >> population >> std::quoted(gradeStr) >> std::quoted(typeStr);

    // Êîíâåðòàöèÿ ñòðîêè â enum
    PopulationGrade grade = PopulationGrade::SMALL;
    if (gradeStr == "Medium")  grade = PopulationGrade::MEDIUM;
    else if (gradeStr == "Large") grade = PopulationGrade::LARGE;

    SettlementType type = SettlementType::CITY;
    if (typeStr == "Town")    type = SettlementType::TOWN;
    else if (typeStr == "Village") type = SettlementType::VILLAGE;

    addCityNode(id, name, population, grade, type);
}

// Äîáàâëåíèå óçëà â ñïèñîê è â õåø-òàáëèöó
void CityTable::addCityNode(int id, const std::string& name, int population,
    PopulationGrade grade, SettlementType type)
{
    CityNode* newNode = new CityNode(id, name, population, grade, type, head->next);
    head->next = newNode;
    idToCityMap.insert(id, newNode);
    nameToIdMap[name] = id;
}

// Äîáàâëåíèå íîâîãî ãîðîäà (ñ ãåíåðàöèåé ID)
void CityTable::addCity(const std::string& name, int population,
    PopulationGrade grade, SettlementType type)
{
    // Ãåíåðàöèÿ ID: ìàêñèìóì ñóùåñòâóþùåãî +1
    int newId = 1;
    for (const auto& pair : nameToIdMap) {
        if (pair.second >= newId) newId = pair.second + 1;
    }
    addCityNode(newId, name, population, grade, type);
}

// Îáíîâëåíèå øèðèíû ñòîëáöîâ (ID è name) íà îñíîâå äàííûõ
void CityTable::updateColumnWidths() {
    CityNode* curr = head->next;
    int maxIdLen = 1;
    size_t maxNameLen = 0;
    while (curr) {
        int idLen = static_cast<int>(std::to_string(curr->id).length());
        if (idLen > maxIdLen) maxIdLen = idLen;
        if (curr->name.length() > maxNameLen) {
            maxNameLen = curr->name.length();
        }
        curr = curr->next;
    }
    idWidth = maxIdLen + 2;
    nameWidth = static_cast<int>(maxNameLen) + 4;
}

// Ôîðìàòèðîâàíèå óçëà äëÿ âûâîäà ñ ó÷åòîì øèðèíû ñòîëáöîâ
std::string CityTable::formatNode(const CityNode* node) const {
    std::ostringstream oss;
    oss << std::left
        << std::setw(idWidth) << node->id
        << std::setw(nameWidth) << node->name
        << std::setw(populationWidth) << node->population
        << std::setw(typeWidth) << settlementTypeToString(node->type);
    return oss.str();
}

// Äîáàâëåíèå ôèëüòðà (öåïî÷êà ñâÿçàííûõ List)
void CityTable::addFilter(const std::string& field, const std::string& pattern) {
    Filter* newFilter = new Filter{ field, pattern, nullptr };
    if (!currentFilter) {
        currentFilter = newFilter;
    }
    else {
        Filter* temp = currentFilter;
        while (temp->next) temp = temp->next;
        temp->next = newFilter;
    }
}

// Î÷èùàåò âñå ôèëüòðû
void CityTable::clearFilters() {
    Filter* current = currentFilter;
    while (current) {
        Filter* next = current->next;
        delete current;
        current = next;
    }
    currentFilter = nullptr;
}

// Ïðèìåíåíèå âñåõ àêòèâíûõ ôèëüòðîâ; âîçâðàùàåò íîâûé ñïèñîê îòôèëüòðîâàííûõ óçëîâ
CityTable::CityNode* CityTable::applyFilters() const {
    CityNode* filteredHead = new CityNode(-1, "", 0, PopulationGrade::SMALL, SettlementType::CITY, nullptr);
    CityNode* tail = filteredHead;
    CityNode* curr = head->next;
    while (curr) {
        bool match = true;
        Filter* filter = currentFilter;
        while (filter) {
            if (!matchField(curr, filter->field, filter->pattern)) {
                match = false;
                break;
            }
            filter = filter->next;
        }
        if (match) {
            CityNode* newNode = cloneNode(curr);
            tail->next = newNode;
            tail = newNode;
        }
        curr = curr->next;
    }
    return filteredHead->next;
}

// Ïðîâåðêà ñîîòâåòñòâèÿ îòäåëüíîãî ïîëÿ óñëîâèþ (äëÿ name è type  regex; äëÿ population  ÷èñëîâîé îïåðàòîð)
bool CityTable::matchField(const CityNode* node, const std::string& field, const std::string& pattern) const {
    if (field == "name") {
        return std::regex_match(node->name, std::regex(pattern));
    }
    else if (field == "population") {
        return checkNumeric(node->population, pattern);
    }
    else if (field == "type") {
        return std::regex_match(settlementTypeToString(node->type), std::regex(pattern));
    }
    return false;
}

// Ïðîâåðêà ÷èñëîâîãî óñëîâèÿ: ">=1000", "<500" èëè òî÷íîå ðàâåíñòâî
bool CityTable::checkNumeric(int value, const std::string& pattern) const {
    if (pattern.empty()) return true;
    if (pattern[0] == '>') {
        return value > std::stoi(pattern.substr(1));
    }
    else if (pattern[0] == '<') {
        return value < std::stoi(pattern.substr(1));
    }
    return value == std::stoi(pattern);
}

// Êëîíèðîâàíèå óçëà (áåç êîïèðîâàíèÿ next)
CityTable::CityNode* CityTable::cloneNode(const CityNode* src) const {
    return new CityNode(
        src->id,
        src->name,
        src->population,
        src->grade,
        src->type,
        nullptr
    );
}

// Ïðåîáðàçîâàíèå PopulationGrade â ñòðîêó
std::string CityTable::populationGradeToString(PopulationGrade grade) {
    switch (grade) {
    case PopulationGrade::SMALL:  return "Small";
    case PopulationGrade::MEDIUM: return "Medium";
    case PopulationGrade::LARGE:  return "Large";
    default:                      return "Unknown";
    }
}

// Ïðåîáðàçîâàíèå SettlementType â ñòðîêó
std::string CityTable::settlementTypeToString(SettlementType type) {
    switch (type) {
    case SettlementType::CITY:    return "City";
    case SettlementType::TOWN:    return "Town";
    case SettlementType::VILLAGE: return "Village";
    default:                      return "Unknown";
    }
}

// Èòåðàòîð ïî ñïèñêó ãîðîäîâ: ñáðîñ íà íà÷àëî (èëè íà ïåðåäàííûé óçåë)
void CityTable::cityIteratorReset(CityNode* start) const {
    currentIterator = start ? start : head->next;
}

// Ïðîâåðêà íàëè÷èÿ ñëåäóþùåãî ýëåìåíòà èòåðàòîðà
bool CityTable::cityIteratorHasNext() const {
    return currentIterator != nullptr;
}

// Ïîëó÷åíèå ñëåäóþùåãî ýëåìåíòà èòåðàòîðà
CityTable::CityInfo CityTable::cityIteratorNext() const {
    CityInfo info;
    if (!currentIterator) return info;
    info.id = currentIterator->id;
    info.name = currentIterator->name;
    info.population = currentIterator->population;
    info.grade = currentIterator->grade;
    info.type = currentIterator->type;
    currentIterator = currentIterator->next;
    return info;
}

// Óäàëåíèå ãîðîäà ïî èìåíè: íàõîäèì ID, óäàëÿåì óçåë èç ñïèñêà è õåø-òàáëèö
void CityTable::deleteCity(const std::string& name) {
    auto it = nameToIdMap.find(name);
    if (it == nameToIdMap.end()) return;

    int id = it->second;
    CityNode* prev = head;
    CityNode* curr = head->next;
    while (curr) {
        if (curr->id == id) {
            prev->next = curr->next;
            idToCityMap.remove(id);
            nameToIdMap.erase(name);
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// Ñîõðàíåíèå òåêóùåãî ñïèñêà ãîðîäîâ â ôàéë
void CityTable::saveToFile() const {
    std::ofstream file("cities.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening cities file for writing!" << std::endl;
        return;
    }
    CityNode* curr = head->next;
    while (curr) {
        file << curr->id << ' '
            << std::quoted(curr->name) << ' '
            << curr->population << ' '
            << std::quoted(populationGradeToString(curr->grade)) << ' '
            << std::quoted(settlementTypeToString(curr->type))
            << '\n';
        curr = curr->next;
    }
    file.close();
}

// Ãåòòåð: âîçâðàùàåò èìÿ ãîðîäà ïî åãî ID (èëè ïóñòóþ ñòðîêó, åñëè íå íàéäåí)
std::string CityTable::getCityNameById(int id) const {
    CityNode* node = idToCityMap.find<CityNode>(id);
    return node ? node->name : "";
}

bool CityTable::cityExists(int cityId) const {
    return idToCityMap.find<CityNode>(cityId) != nullptr;
}

// Ãåòòåð: âîçâðàùàåò ID ãîðîäà ïî åãî èìåíè (èëè -1, åñëè íå íàéäåí)
int CityTable::getCityIdByName(const std::string& name) const {
    auto it = nameToIdMap.find(name);
    return (it != nameToIdMap.end()) ? it->second : -1;
}