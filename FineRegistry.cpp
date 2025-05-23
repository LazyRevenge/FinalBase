#include "FineRegistry.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
using namespace std;

// Êîíñòðóêòîð: èíèöèàëèçàöèÿ çàãîëîâî÷íîãî óçëà è çàãðóçêà äàííûõ
FineRegistry::FineRegistry()
    : head(new ViolationNode(-1, -1, -1, -1, false, "", nullptr)),
    currentIterator(nullptr),
    recordIdWidth(5),
    driverIdWidth(5),
    cityIdWidth(5),
    fineIdWidth(5),
    paidWidth(8),
    dateWidth(12)
{
    loadFromFile();
}

// Äåñòðóêòîð: î÷èñòêà ñïèñêà è õåø-òàáëèöû
FineRegistry::~FineRegistry() {
    ViolationNode* current = head->next;
    while (current) {
        ViolationNode* temp = current;
        current = current->next;
        delete temp;
    }
    delete head;
    recordToNodeMap.clear();
}

// Çàãðóçêà äàííûõ èç ôàéëà
void FineRegistry::loadFromFile() {
    ifstream file("registry.txt");
    if (!file.is_open()) {
        cerr << "Error opening registry file!" << endl;
        return;
    }
    // Î÷èùàåì ñóùåñòâóþùèå äàííûå
    ViolationNode* current = head->next;
    while (current) {
        ViolationNode* temp = current;
        current = current->next;
        delete temp;
    }
    head->next = nullptr;
    recordToNodeMap.clear();

    string line;
    while (getline(file, line)) {
        parseLine(line);
    }
    file.close();
}

// Ïàðñèíã ñòðîêè: "recordId driverId cityId fineId paid date"
void FineRegistry::parseLine(const std::string& line) {
    if (line.empty()) return;
    istringstream iss(line);
    int recordId, driverId, cityId, fineId;
    int paidInt;
    string date;
    iss >> recordId >> driverId >> cityId >> fineId >> paidInt >> quoted(date);

    bool paid = (paidInt == 1);
    addViolationNode(recordId, driverId, cityId, fineId, paid, date);
}

// Äîáàâëåíèå óçëà â ñïèñîê è â õåø-òàáëèöó
void FineRegistry::addViolationNode(int recordId, int driverId, int cityId,
    int fineId, bool paid, const std::string& date)
{
    ViolationNode* newNode = new ViolationNode(
        recordId, driverId, cityId, fineId, paid, date, head->next
    );
    head->next = newNode;
    recordToNodeMap.insert(recordId, newNode);
}

// Äîáàâëåíèå íîâîãî íàðóøåíèÿ (ñ ãåíåðàöèåé recordId)
void FineRegistry::addViolation(int driverId, int cityId, int fineId, const std::string& date) {
    // Ãåíåðàöèÿ recordId: ìàêñèìóì ñóùåñòâóþùåãî +1
    int newId = 1;
    ViolationNode* curr = head->next;
    while (curr) {
        if (curr->recordId >= newId) newId = curr->recordId + 1;
        curr = curr->next;
    }
    addViolationNode(newId, driverId, cityId, fineId, false, date);
}

// Èòåðàòîð: ñáðîñ íà íà÷àëî
void FineRegistry::violationIteratorReset() const {
    currentIterator = head->next;
}

// Åñòü ëè ñëåäóþùèé?
bool FineRegistry::violationIteratorHasNext() const {
    return currentIterator != nullptr;
}

// Ïîëó÷åíèå ñëåäóþùåãî ñ äåòàëèçàöèåé (driverName, cityName, fineType, fineAmount)
FineRegistry::ViolationInfo FineRegistry::violationIteratorNext(
    const DriverTable& drivers, const CityTable& cities, const FineTable& fines) const
{
    ViolationInfo info;
    if (!currentIterator) return info;
    info.recordId = currentIterator->recordId;
    info.driverId = currentIterator->driverId;
    info.cityId = currentIterator->cityId;
    info.fineId = currentIterator->fineId;
    info.paid = currentIterator->paid;
    info.date = currentIterator->date;

    // Ïîëó÷àåì èìåíà è ñóììû
    info.driverName = "";
    info.cityName = "";
    info.fineType = "";
    info.fineAmount = 0.0;

    int dId = info.driverId;
    int cId = info.cityId;
    int fId = info.fineId;

    // Ïîëó÷åíèå driverName
    DriverTable::DriverInfo dInfo;
    drivers.driverIteratorReset();
    while (drivers.driverIteratorHasNext()) {
        dInfo = drivers.driverIteratorNext();
        if (dInfo.id == dId) {
            info.driverName = dInfo.fullName;
            break;
        }
    }

    // Ïîëó÷åíèå cityName
    CityTable::CityInfo cInfo;
    cities.cityIteratorReset();
    while (cities.cityIteratorHasNext()) {
        cInfo = cities.cityIteratorNext();
        if (cInfo.id == cId) {
            info.cityName = cInfo.name;
            break;
        }
    }

    // Ïîëó÷åíèå fineType è fineAmount
    FineTable::FineInfo fInfo;
    fines.fineIteratorReset();
    while (fines.fineIteratorHasNext()) {
        fInfo = fines.fineIteratorNext();
        if (fInfo.id == fId) {
            info.fineType = fInfo.type;
            info.fineAmount = fInfo.amount;
            break;
        }
    }

    currentIterator = currentIterator->next;
    return info;
}

// Ïîìåòêà îá îïëàòå
void FineRegistry::markAsPaid(int recordId) {
    ViolationNode* node = recordToNodeMap.find<ViolationNode>(recordId);
    if (node) {
        node->paid = true;
    }
}

// Îáíîâëåíèå ññûëîê ïðè óäàëåíèè âîäèòåëÿ (óñòàíàâëèâàåì driverId = -1)
void FineRegistry::updateDriverReferences(int deletedDriverId) {
    ViolationNode* curr = head->next;
    while (curr) {
        if (curr->driverId == deletedDriverId) {
            curr->driverId = -1;
        }
        curr = curr->next;
    }
}

// Îáíîâëåíèå ññûëîê ïðè óäàëåíèè ãîðîäà (óñòàíàâëèâàåì cityId = -1)
void FineRegistry::updateCityReferences(int deletedCityId) {
    ViolationNode* curr = head->next;
    while (curr) {
        if (curr->cityId == deletedCityId) {
            curr->cityId = -1;
        }
        curr = curr->next;
    }
}

FineRegistry::ViolationInfo FineRegistry::getViolationById(int recordId,
    const DriverTable& drivers,
    const CityTable& cities,
    const FineTable& fines) const
{
    ViolationNode* current = head->next;
    while (current) {
        if (current->recordId == recordId) {
            ViolationInfo info;
            info.recordId = current->recordId;
            info.driverId = current->driverId;
            info.cityId = current->cityId;
            info.fineId = current->fineId;
            info.paid = current->paid;
            info.date = current->date;

            // Получаем детали через публичные методы
            info.driverName = drivers.getDriverNameById(info.driverId);
            info.cityName = cities.getCityNameById(info.cityId);
            info.fineType = fines.getFineTypeById(info.fineId);
            info.fineAmount = fines.getAmountById(info.fineId);

            return info;
        }
        current = current->next;
    }
    return ViolationInfo{};
}

void FineRegistry::updateViolationsCity(int driverId, int newCityId) {
    ViolationNode* current = head->next;
    while (current) {
        if (current->driverId == driverId) {
            current->cityId = newCityId;
        }
        current = current->next;
    }
}

// Ñîõðàíåíèå äàííûõ â ôàéë
void FineRegistry::saveToFile() const {
    ofstream file("registry.txt");
    if (!file.is_open()) {
        cerr << "Error opening registry file for writing!" << endl;
        return;
    }
    ViolationNode* curr = head->next;
    while (curr) {
        file << curr->recordId << ' '
            << curr->driverId << ' '
            << curr->cityId << ' '
            << curr->fineId << ' '
            << (curr->paid ? 1 : 0) << ' '
            << quoted(curr->date) << '\n';
        curr = curr->next;
    }
    file.close();
}