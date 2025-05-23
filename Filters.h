//#pragma once
//#include <vector>
//#include <string>
//#include <functional>
//
//struct Filter {
//    std::string entity_type; // "city", "driver", "violation"
//    std::string field;       // Поле для фильтрации
//    std::string operation;   // Операция (contains, >, <, ==)
//    std::string value;       // Значение фильтра
//};
//
//class FilterManager {
//    std::vector<Filter> active_filters;
//
//public:
//    void add_filter(const Filter& f);
//    bool remove_filter(const std::string& entity_type, size_t index);
//    void clear_entity(const std::string& entity_type);
//    void clear_all();
//    const std::vector<Filter>& get_filters() const;
//};