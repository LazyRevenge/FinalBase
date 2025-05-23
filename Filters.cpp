//#include "filters.h"
//#include <algorithm>
//
//void FilterManager::add_filter(const Filter& f) {
//    active_filters.push_back(f);
//}
//
//bool FilterManager::remove_filter(const std::string& entity_type, size_t index) {
//    size_t count = 0;
//    for (auto it = active_filters.begin(); it != active_filters.end(); ++it) {
//        if (it->entity_type == entity_type) {
//            if (count++ == index) {
//                active_filters.erase(it);
//                return true;
//            }
//        }
//    }
//    return false;
//}
//
//void FilterManager::clear_entity(const std::string& entity_type) {
//    active_filters.erase(
//        std::remove_if(active_filters.begin(), active_filters.end(),
//            [&](const Filter& f) { return f.entity_type == entity_type; }),
//        active_filters.end()
//    );
//}
//
//void FilterManager::clear_all() {
//    active_filters.clear();
//}
//
//const std::vector<Filter>& FilterManager::get_filters() const {
//    return active_filters;
//}
//
//std::vector<Filter> FilterManager::get_entity_filters(const std::string& entity_type) const {
//    std::vector<Filter> result;
//    for (const auto& f : active_filters) {
//        if (f.entity_type == entity_type) {
//            result.push_back(f);
//        }
//    }
//    return result;
//}
//
//std::string FilterManager::create_description(const Filter& f) {
//    return f.entity_type + ": " + f.field + " " + f.operation + " '" + f.value + "'";
//#include "filters.h"
//#include <algorithm>
//
//void FilterManager::add_filter(const Filter& f) {
//    active_filters.push_back(f);
//}
//
//bool FilterManager::remove_filter(const std::string& entity_type, size_t index) {
//    size_t count = 0;
//    for (auto it = active_filters.begin(); it != active_filters.end(); ++it) {
//        if (it->entity_type == entity_type) {
//            if (count++ == index) {
//                active_filters.erase(it);
//                return true;
//            }
//        }
//    }
//    return false;
//}
//
//void FilterManager::clear_entity(const std::string& entity_type) {
//    active_filters.erase(
//        std::remove_if(active_filters.begin(), active_filters.end(),
//            [&](const Filter& f) { return f.entity_type == entity_type; }),
//        active_filters.end()
//    );
//}
//
//void FilterManager::clear_all() {
//    active_filters.clear();
//}
//
//const std::vector<Filter>& FilterManager::get_filters() const {
//    return active_filters;
//}
//
//std::vector<Filter> FilterManager::get_entity_filters(const std::string& entity_type) const {
//    std::vector<Filter> result;
//    for (const auto& f : active_filters) {
//        if (f.entity_type == entity_type) {
//            result.push_back(f);
//        }
//    }
//    return result;
//}
//
//std::string FilterManager::create_description(const Filter& f) {
//    return f.entity_type + ": " + f.field + " " + f.operation + " '" + f.value + "'";
//}