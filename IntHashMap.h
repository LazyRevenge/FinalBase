#pragma once

#include <vector>

// Простая реализация хеш-таблицы для целых ключей
class IntHashMap {
public:
    // Конструктор: начальный размер
    IntHashMap(size_t initialSize = 101);
    ~IntHashMap();

    // Вставка пары (ключ → указатель на узел)
    void insert(int key, void* ptr);

    // Поиск по ключу; возвращает указатель (или nullptr)
    template <typename T>
    T* find(int key) const {
        size_t index = hashKey(key) % capacity;
        size_t start = index;
        while (table[index].inUse) {
            if (table[index].key == key)
                return static_cast<T*>(table[index].ptr);
            index = (index + 1) % capacity;
            if (index == start) break;
        }
        return nullptr;
    }

    // Удаление по ключу
    void remove(int key);

    // Очистка всей таблицы
    void clear();

private:
    struct Entry {
        int key;
        void* ptr;
        bool inUse;
        Entry() : key(0), ptr(nullptr), inUse(false) {}
    };

    std::vector<Entry> table;
    size_t capacity;
    size_t size;

    // Хеш-функция (двойное хеширование)
    size_t hashKey(int key) const;
    size_t hashStep(int key) const;

    // Расширение таблицы при заполнении на 75%
    void rehash();
};