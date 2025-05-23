#include "UserInterface.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <clocale>
#include <iomanip>
using namespace std;

// Ширины колонок (без ID)
static constexpr int NAME_WIDTH = 20;
static constexpr int POPULATION_WIDTH = 12;
static constexpr int TYPE_WIDTH = 10;
static constexpr int DRIVER_NAME_WIDTH = 40;
static constexpr int BIRTH_WIDTH = 12;
static constexpr int FINE_TYPE_WIDTH = 20;
static constexpr int AMOUNT_WIDTH = 10;
static constexpr int SEVERITY_WIDTH = 10;
static constexpr int DATE_WIDTH = 12;
static constexpr int PAID_WIDTH = 6;

// Печать разделительной строки для таблицы городов
static void printCitiesBorder() {
    cout << '+'
        << string(NAME_WIDTH + 2, '-') << '+'
        << string(POPULATION_WIDTH + 2, '-') << '+'
        << string(TYPE_WIDTH + 2, '-') << '+'
        << "\n";
}

// Печать заголовка таблицы городов
static void printCitiesHeader() {
    printCitiesBorder();
    cout << "| " << left << setw(NAME_WIDTH) << "Название" << " | "
        << left << setw(POPULATION_WIDTH) << "Население" << " | "
        << left << setw(TYPE_WIDTH) << "Тип" << " |\n";
    printCitiesBorder();
}

// Печать строки с данными о городе
static void printCityRow(const string& name, int population, const string& type) {
    cout << "| " << left << setw(NAME_WIDTH) << name << " | "
        << right << setw(POPULATION_WIDTH) << population << " | "
        << left << setw(TYPE_WIDTH) << type << " |\n";
}

// Печать разделительной строки для таблицы водителей
static void printDriversBorder() {
    cout << '+'
        << string(DRIVER_NAME_WIDTH + 2, '-') << '+'
        << string(BIRTH_WIDTH + 3, '-') << '+'
        << string(NAME_WIDTH + 2, '-') << '+'
        << "\n";
}

// Печать заголовка таблицы водителей
static void printDriversHeader() {
    printDriversBorder();
    cout << "| " << left << setw(DRIVER_NAME_WIDTH) << "ФИО" << " | "
        << left << setw(BIRTH_WIDTH) << "Дата рождения" << " | "
        << left << setw(NAME_WIDTH) << "Город" << " |\n";
    printDriversBorder();
}

// Печать строки с данными о водителе
static void printDriverRow(const string& fullName, const string& birthDate, const string& cityName) {
    cout << "| " << left << setw(DRIVER_NAME_WIDTH) << fullName << " | "
        << left << setw(BIRTH_WIDTH) << birthDate << "  | "
        << left << setw(NAME_WIDTH) << cityName << " |\n";
}

// Печать разделительной строки для таблицы штрафов
static void printFinesBorder() {
    cout << '+'
        << string(FINE_TYPE_WIDTH + 2, '-') << '+'
        << string(AMOUNT_WIDTH + 2, '-') << '+'
        << string(SEVERITY_WIDTH + 2, '-') << '+'
        << "\n";
}

// Печать заголовка таблицы штрафов
static void printFinesHeader() {
    printFinesBorder();
    cout << "| " << left << setw(FINE_TYPE_WIDTH) << "Тип" << " | "
        << left << setw(AMOUNT_WIDTH) << "Сумма" << " | "
        << left << setw(SEVERITY_WIDTH) << "Уровень" << " |\n";
    printFinesBorder();
}

// Печать строки с данными о штрафе
static void printFineRow(const string& type, double amount, const string& severity) {
    cout << "| " << left << setw(FINE_TYPE_WIDTH) << type << " | "
        << left << setw(AMOUNT_WIDTH) << amount << " | "
        << left << setw(SEVERITY_WIDTH) << severity << " |\n";
}

// Печать разделительной строки для таблицы нарушений
static void printViolationsBorder() {
    cout << '+'
        << string(DRIVER_NAME_WIDTH + 2, '-') << '+'
        << string(NAME_WIDTH + 2, '-') << '+'
        << string(FINE_TYPE_WIDTH + 2, '-') << '+'
        << string(AMOUNT_WIDTH + 2, '-') << '+'
        << string(DATE_WIDTH + 2, '-') << '+'
        << string(PAID_WIDTH + 3, '-') << '+'
        << "\n";
}

// Печать заголовка таблицы нарушений
static void printViolationsHeader() {
    printViolationsBorder();
    cout << "| " << left << setw(DRIVER_NAME_WIDTH) << "Водитель" << " | "
        << left << setw(NAME_WIDTH) << "Город" << " | "
        << left << setw(FINE_TYPE_WIDTH) << "Штраф" << " | "
        << left << setw(AMOUNT_WIDTH) << "Сумма" << " | "
        << left << setw(DATE_WIDTH) << "Дата" << " | "
        << left << setw(PAID_WIDTH) << "Оплачен" << " |\n";
    printViolationsBorder();
}

// Печать строки с данными о нарушении
static void printViolationRow(const string& driverName, const string& cityName,
    const string& fineType, double fineAmount,
    const string& date, bool paid) {
    cout << "| " << left << setw(DRIVER_NAME_WIDTH) << driverName << " | "
        << left << setw(NAME_WIDTH) << cityName << " | "
        << left << setw(FINE_TYPE_WIDTH) << fineType << " | "
        << left << setw(AMOUNT_WIDTH) << fineAmount << " | "
        << left << setw(DATE_WIDTH) << date << " | "
        << left << setw(PAID_WIDTH) << (paid ? "Да" : "Нет") << "  |\n";
}

void UserInterface::run() {
    setlocale(LC_ALL, ""); // Для корректного вывода кириллицы
    dbManager.loadAll();
    while (true) {
        mainMenu();
    }
}

void UserInterface::mainMenu() {
    while (true){
        cout << "\n=== Главное меню ===\n";
        cout << "1. Управление городами\n";
        cout << "2. Управление водителями\n";
        cout << "3. Управление штрафами\n";
        cout << "4. Управление нарушениями\n";
        cout << "5. Статистика\n";
        cout << "6. Выход\n";
        int choice = readInt("Выберите пункт: ");
        switch (choice) {
        case 1: citiesMenu();   break;
        case 2: driversMenu();  break;
        case 3: finesMenu();    break;
        case 4: registryMenu(); break;
        case 5: statisticsMenu(); break;
        case 6: dbManager.saveAll(); exit(0);
        default: cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

void UserInterface::citiesMenu() {
    while(true){
        cout << "\n--- Города ---\n";
        cout << "1. Список городов\n";
        cout << "2. Добавить город\n";
        cout << "3. Удалить город\n";
        cout << "4. Фильтрация городов\n";
        cout << "5. Сбросить фильтры\n";
        cout << "6. Назад\n";
        int choice = readInt("Выберите пункт: ");
        switch (choice) {
        case 1: listCities();       break;
        case 2: addCity();          break;
        case 3: deleteCity();       break;
        case 4: filterCities();     break;
        case 5: clearCityFilters(); break;
        case 6: return;
        default: cout << "Неверный выбор.\n";
        }
    }
}

void UserInterface::listCities() {
    printCitiesHeader();
    CityTable& cities = dbManager.getCities();
    CityTable::CityNode* filteredHead = cities.applyFilters();
    cities.cityIteratorReset(filteredHead);
    while (cities.cityIteratorHasNext()) {
        auto ci = cities.cityIteratorNext();
        printCityRow(ci.name, ci.population,
            CityTable::settlementTypeToString(ci.type));
    }
    printCitiesBorder();

    CityTable::CityNode* tmp = filteredHead;
    while (tmp) {
        CityTable::CityNode* next = tmp->next;
        delete tmp;
        tmp = next;
    }
}

// UserInterface.cpp (замена существующего метода)
void UserInterface::addCity() {
    string name = readString("Название города: ");

    // Проверка населения
    int population;
    do {
        population = readInt("Население: ");
        if (population < 1) cout << "Население не может быть меньше 1!\n";
    } while (population < 1);

    // Ввод градации
    int gradeInput;
    do {
        cout << "Градация населения (0 - Small, 1 - Medium, 2 - Large): ";
        gradeInput = readInt("");
        if (gradeInput < 0 || gradeInput > 2) {
            cout << "Некорректный ввод! Допустимые значения: 0, 1, 2\n";
        }
    } while (gradeInput < 0 || gradeInput > 2);

    auto grade = static_cast<CityTable::PopulationGrade>(gradeInput);

    // Ввод типа поселения
    int typeInput;
    do {
        cout << "Тип поселения (0 - City, 1 - Town, 2 - Village): ";
        typeInput = readInt("");
        if (typeInput < 0 || typeInput > 2) {
            cout << "Некорректный ввод! Допустимые значения: 0, 1, 2\n";
        }
    } while (typeInput < 0 || typeInput > 2);

    auto type = static_cast<CityTable::SettlementType>(typeInput);

    dbManager.addCity(name, population, grade, type);
    dbManager.getCities().saveToFile();
    cout << "Город добавлен\n";
}

void UserInterface::deleteCity() {
    string name = readString("Название города для удаления: ");
    dbManager.deleteCity(name);
    cout << "Город удалён (если существовал).\n";
}

void UserInterface::filterCities() {
    CityTable& cities = dbManager.getCities();
    string field = readString("Введите поле (name, population, type): ");
    string pattern = readString("Введите шаблон (regex для name/type или условие для population, например \">100000\"): ");
    cities.addFilter(field, pattern);
    cout << "Фильтр добавлен.\n";
}

void UserInterface::clearCityFilters() {
    CityTable& cities = dbManager.getCities();
    cities.clearFilters();
    cout << "Все фильтры очищены.\n";
}

void UserInterface::driversMenu() {
    while(true){
        cout << "\n--- Водители ---\n";
        cout << "1. Список водителей\n";
        cout << "2. Добавить водителя\n";
        cout << "3. Удалить водителя\n";
        cout << "4. Изменить город регистрации\n"; // НОВЫЙ ПУНКТ
        cout << "5. Назад\n";

        int choice = readInt("Выберите пункт: ");
        switch (choice) {
        case 1: listDrivers(); break;
        case 2: addDriver(); break;
        case 3: deleteDriver(); break;
        case 4: editDriverCity(); break; // НОВЫЙ КЕЙС
        case 5: return;
        default: cout << "Неверный выбор.\n";
        }
    }
}

void UserInterface::listDrivers() {
    printDriversHeader();
    auto& drivers = dbManager.getDrivers();
    auto& cities = dbManager.getCities();
    drivers.driverIteratorReset();
    while (drivers.driverIteratorHasNext()) {
        auto di = drivers.driverIteratorNext();
        string cityName = cities.getCityNameById(di.cityId);
        printDriverRow(di.fullName, di.birthDate, cityName);
    }
    printDriversBorder();
}

void UserInterface::addDriver() {
    string fullName = readString("ФИО водителя: ");
    string birthDate = readString("Дата рождения (ДД.MM.ГГГГ): ");
    string cityName = readString("Город регистрации: ");

    int cityId = dbManager.getCities().getCityIdByName(cityName);
    if (cityId == -1) {
        cout << "Город \"" << cityName << "\" не найден. Введите данные для нового города.\n";
        int population = readInt("Население: ");
        cout << "Градация населения (0 - Small, 1 - Medium, 2 - Large): ";
        int gradeInput = readInt("");
        auto grade = static_cast<CityTable::PopulationGrade>(gradeInput);

        cout << "Тип поселения (0 - City, 1 - Town, 2 - Village): ";
        int typeInput = readInt("");
        auto type = static_cast<CityTable::SettlementType>(typeInput);

        dbManager.addCity(cityName, population, grade, type);
        dbManager.getCities().saveToFile();
        cout << "Город \"" << cityName << "\" успешно добавлен.\n";
    }

    try {
        dbManager.addDriver(fullName, birthDate, cityName);
        cout << "Водитель добавлен.\n";
    }
    catch (const exception& e) {
        cout << "Ошибка при добавлении водителя: " << e.what() << "\n";
    }
}

void UserInterface::deleteDriver() {
    string name = readString("ФИО водителя для удаления: ");
    // Собираем все водители с таким ФИО
    auto& drivers = dbManager.getDrivers();
    auto& cities = dbManager.getCities();
    drivers.driverIteratorReset();
    vector<DriverTable::DriverInfo> matchedByName;
    while (drivers.driverIteratorHasNext()) {
        auto di = drivers.driverIteratorNext();
        if (di.fullName == name) {
            matchedByName.push_back(di);
        }
    }
    if (matchedByName.empty()) {
        cout << "Водителей с таким ФИО не найдено.\n";
        return;
    }

    // Если только один, удаляем сразу
    if (matchedByName.size() == 1) {
        dbManager.deleteDriverById(matchedByName[0].id);
        cout << "Водитель удалён.\n";
        return;
    }

    // Несколько совпадений: уточняем по дате рождения
    vector<DriverTable::DriverInfo> matchedByDate;
    string birthDate = readString("Несколько водителей с таким ФИО. Введите дату рождения (ДД.MM.ГГГГ): ");
    for (auto& di : matchedByName) {
        if (di.birthDate == birthDate) {
            matchedByDate.push_back(di);
        }
    }
    if (matchedByDate.empty()) {
        cout << "Водителей с таким ФИО и датой рождения не найдено.\n";
        return;
    }

    // Если после даты один, удаляем
    if (matchedByDate.size() == 1) {
        dbManager.deleteDriverById(matchedByDate[0].id);
        cout << "Водитель удалён.\n";
        return;
    }

    // Несколько совпадений по дате: уточняем по городу
    vector<DriverTable::DriverInfo> matchedByCity;
    string cityName = readString("Несколько водителей с таким ФИО и датой. Введите город: ");
    for (auto& di : matchedByDate) {
        string dCity = cities.getCityNameById(di.cityId);
        if (dCity == cityName) {
            matchedByCity.push_back(di);
        }
    }
    if (matchedByCity.empty()) {
        cout << "Водителей с такими ФИО, датой и городом не найдено.\n";
        return;
    }

    // Если после всех фильтров несколько — удаляем их все
    for (auto& di : matchedByCity) {
        dbManager.deleteDriverById(di.id);
    }
    cout << "Удалено водителей с указанными данными: " << matchedByCity.size() << "\n";
}

void UserInterface::finesMenu() {
    while(true){
        cout << "\n--- Штрафы ---\n";
        cout << "1. Список штрафов\n";
        cout << "2. Добавить штраф\n";
        cout << "3. Удалить штраф\n";
        cout << "4. Назад\n";
        int choice = readInt("Выберите пункт: ");
        switch (choice) {
        case 1: listFines(); break;
        case 2: addFine();   break;
        case 3: deleteFine(); break;
        case 4: return;
        default: cout << "Неверный выбор.\n";
        }
    }
}

void UserInterface::listFines() {
    printFinesHeader();
    auto& fines = dbManager.getFines();
    fines.fineIteratorReset();
    while (fines.fineIteratorHasNext()) {
        auto fi = fines.fineIteratorNext();
        printFineRow(fi.type, fi.amount, FineTable::severityToString(fi.severity));
    }
    printFinesBorder();
}

// UserInterface.cpp (замена существующего метода)
void UserInterface::addFine() {
    string type = readString("Тип нарушения: ");
    double amount = readDouble("Сумма: ");

    // Ввод уровня тяжести
    int sev;
    do {
        cout << "Уровень тяжести (0 - Light, 1 - Medium, 2 - Heavy): ";
        sev = readInt("");
        if (sev < 0 || sev > 2) {
            cout << "Некорректный ввод! Допустимые значения: 0, 1, 2\n";
        }
    } while (sev < 0 || sev > 2);

    try {
        dbManager.addFine(type, amount,
            static_cast<FineTable::Severity>(sev));
        cout << "Штраф добавлен\n";
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << "\n";
    }
}

void UserInterface::deleteFine() {
    string type = readString("Тип штрафа для удаления: ");
    dbManager.getFines().deleteFine(type);
    cout << "Штраф удалён (если существовал).\n";
}

void UserInterface::registryMenu() {
    while(true){
        cout << "\n--- Нарушения ---\n";
        cout << "1. Список нарушений\n";
        cout << "2. Добавить нарушение\n";
        cout << "3. Отметить оплату\n";
        cout << "5. Назад\n";

        int choice = readInt("Выберите пункт: ");
        switch (choice) {
        case 1: listViolations(); break;
        case 2: addViolation();   break;
        case 3: markViolationPaid(); break; 
        case 5: return;
        default: cout << "Неверный выбор.\n";
        }
    }
}

void UserInterface::listViolations() {
    printViolationsHeader();
    auto violations = dbManager.getAllViolations();
    for (auto& vi : violations) {
        printViolationRow(
            vi.driverName,
            vi.cityName,
            vi.fineType,
            vi.fineAmount,
            vi.date,
            vi.paid
        );
    }
    printViolationsBorder();
}

// UserInterface.cpp (перед методом addViolation)
bool UserInterface::parseDate(const std::string& dateStr, Date& date) {
    if (dateStr.length() != 10 || dateStr[2] != '.' || dateStr[5] != '.')
        return false;

    try {
        date.day = std::stoi(dateStr.substr(0, 2));
        date.month = std::stoi(dateStr.substr(3, 2));
        date.year = std::stoi(dateStr.substr(6, 4));
        return isDateValid(date);
    }
    catch (const std::invalid_argument&) {
        return false;
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

bool UserInterface::isDateValid(const Date& date) {
    if (date.year < 1900 || date.year > 2100) return false;
    if (date.month < 1 || date.month > 12) return false;
    if (date.day < 1 || date.day > 31) return false;

    // Проверка количества дней в месяце
    const int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int maxDay = daysInMonth[date.month - 1];
    if (date.month == 2 && ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0)))
        maxDay = 29;
    return date.day <= maxDay;
}

int UserInterface::calculateAge(const Date& birthDate, const Date& violationDate) {
    int age = violationDate.year - birthDate.year;
    if (violationDate.month < birthDate.month ||
        (violationDate.month == birthDate.month && violationDate.day < birthDate.day)) {
        age--;
    }
    return age;
}

bool UserInterface::isDriverAdult(const std::string& birthDateStr, const std::string& violationDateStr) {
    Date birthDate, violationDate;
    if (!parseDate(birthDateStr, birthDate)) return false;
    if (!parseDate(violationDateStr, violationDate)) return false;

    return calculateAge(birthDate, violationDate) >= 18;
}

// UserInterface.cpp (замена существующего метода)
void UserInterface::addViolation() {
    string driverName = readString("ФИО водителя: ");
    string fineType = readString("Тип штрафа: ");
    string date = readString("Дата нарушения (ДД.ММ.ГГГГ): ");

    // Получаем дату рождения водителя
    int driverId = dbManager.getDrivers().getDriverId(driverName);
    if (driverId == -1) {
        cout << "Водитель не найден!\n";
        return;
    }

    string birthDate;
    DriverTable& drivers = dbManager.getDrivers();
    drivers.driverIteratorReset();
    while (drivers.driverIteratorHasNext()) {
        auto di = drivers.driverIteratorNext();
        if (di.id == driverId) {
            birthDate = di.birthDate;
            break;
        }
    }

    // Проверка возраста
    if (!isDriverAdult(birthDate, date)) {
        cout << "Водитель младше 18 лет на момент нарушения!\n";
        return;
    }

    try {
        dbManager.addViolation(driverName, fineType, date);
        cout << "Нарушение добавлено\n";
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << "\n";
    }
}

// UserInterface.cpp
void UserInterface::editDriverCity() {
    string fullName = readString("Введите ФИО водителя: ");
    string newCityName = readString("Введите новый город регистрации: ");

    int newCityId = dbManager.getCities().getCityIdByName(newCityName);
    if (newCityId == -1) {
        cout << "Город не найден!\n";
        return;
    }

    if (dbManager.getDrivers().updateDriverCity(fullName, newCityId)) {
        int driverId = dbManager.getDrivers().getDriverId(fullName);
        dbManager.getRegistry().updateViolationsCity(driverId, newCityId);
        dbManager.saveAll();
        cout << "Данные обновлены\n";
    }
    else {
        cout << "Водитель не найден\n";
    }
}

void UserInterface::markViolationPaid() {
    string driverName = readString("ФИО водителя: ");
    string fineType = readString("Тип нарушения: ");
    string date = readString("Дата (ДД.MM.ГГГГ): ");

    auto violations = dbManager.getAllViolations();
    int count = 0;
    for (auto& vi : violations) {
        if (vi.driverName == driverName && vi.fineType == fineType && vi.date == date && !vi.paid) {
            dbManager.markFineAsPaid(vi.recordId);
            ++count;
        }
    }
    if (count > 0) {
        dbManager.saveAll();
        cout << "Отмечено оплаченных нарушений: " << count << "\n";
    }
    else {
        cout << "Нарушений с такими данными не найдено или они уже оплачены.\n";
    }
}

void UserInterface::statisticsMenu() {
    while(true){
        cout << "\n--- Статистика ---\n";
        cout << "1. Нарушения по городам\n";
        cout << "2. Топ-5 водителей\n";
        cout << "3. Назад\n";
        int choice = readInt("Выберите пункт: ");
        switch (choice) {
        case 1: showViolationsByCity(); break;
        case 2: showTopDrivers();       break;
        case 3: return;
        default: cout << "Неверный выбор.\n";
        }
    }
}

void UserInterface::showViolationsByCity() {
    CityViolations stats[MAX_CITIES] = {};
    int cityCount = 0;

    collectCityStats(stats, cityCount);
    qsort(stats, cityCount, sizeof(CityViolations), compareCityStats);
    printCityViolations(stats, cityCount);
}

void UserInterface::collectCityStats(CityViolations* stats, int& cityCount) {
    auto& registry = dbManager.getRegistry();
    registry.violationIteratorReset();

    while (registry.violationIteratorHasNext()) {
        auto vi = registry.violationIteratorNext(
            dbManager.getDrivers(),
            dbManager.getCities(),
            dbManager.getFines()
        );

        // Поиск города
        int foundIdx = -1;
        for (int i = 0; i < cityCount; ++i) {
            if (stats[i].name == vi.cityName) {
                foundIdx = i;
                break;
            }
        }

        // Добавление нового города
        if (foundIdx == -1 && cityCount < MAX_CITIES) {
            stats[cityCount].name = vi.cityName;
            foundIdx = cityCount++;
        }

        // Добавление нарушения
        if (foundIdx != -1 && stats[foundIdx].count < MAX_VIOLATIONS_PER_CITY) {
            stats[foundIdx].violationIds[stats[foundIdx].count] = vi.recordId;
            stats[foundIdx].count++;
        }
    }
}

// UserInterface.cpp
int UserInterface::compareCityStats(const void* a, const void* b) {
    const CityViolations* ca = static_cast<const CityViolations*>(a);
    const CityViolations* cb = static_cast<const CityViolations*>(b);
    return cb->count - ca->count; // Сортировка по убыванию
}

void UserInterface::printCityViolations(CityViolations* stats, int cityCount) {
    printViolationsHeader();

    for (int i = 0; i < cityCount; ++i) {
        for (int j = 0; j < stats[i].count; ++j) {
            auto vi = dbManager.getRegistry().getViolationById(
                stats[i].violationIds[j],
                dbManager.getDrivers(),
                dbManager.getCities(),
                dbManager.getFines()
            );

            printViolationRow(
                vi.driverName,
                vi.cityName,
                vi.fineType,
                vi.fineAmount,
                vi.date,
                vi.paid
            );
        }
    }

    printViolationsBorder();
}

void UserInterface::showTopDrivers() {
    cout << "\nТоп-5 водителей по количеству нарушений:\n";
    auto violations = dbManager.getAllViolations();
    map<std::string, int> countMap;
    for (auto& v : violations) {
        countMap[v.driverName]++;
    }
    vector<std::pair<std::string, int>> vec(countMap.begin(), countMap.end());
    sort(vec.begin(), vec.end(), [](auto& a, auto& b) { return a.second > b.second; });
    for (size_t i = 0; i < vec.size() && i < 5; ++i) {
        cout << vec[i].first << ": " << vec[i].second << "\n";
    }
}

int UserInterface::readInt(const std::string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        cout << "Неверный ввод. Пожалуйста, введите число.\n";
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    }
}

double UserInterface::readDouble(const std::string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        cout << "Неверный ввод. Пожалуйста, введите число.\n";
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string UserInterface::readString(const std::string& prompt) {
    std::string value;
    cout << prompt;
    std::getline(cin, value);
    return value;
}