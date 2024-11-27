#include "../headers/SymbolTable.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <variant>

SymbolTable* SymbolTable::instance = nullptr;

SymbolTable *SymbolTable::GetTable() {
    if (instance == nullptr) {
        instance = new SymbolTable();
    }
    return instance;
}

size_t SymbolTable::GetInd(const std::string &key) {
    if (!table.contains(key)) {
        throw std::runtime_error("Variable not found");
    }

    return table[key];
}


Object *SymbolTable::GetObjByInd(size_t index) {
    if (index >= values.size()) {
        throw std::runtime_error("Index out of range");
    }

    return values[index];
}

size_t SymbolTable::Add(const std::string &key, Object *obj) {
    if (table.contains(key)) {
        size_t index = table[key];
        return index;
    }

    size_t index = values.size();
    table[key] = index;
    values.push_back(obj);

    return index;
}

SymbolTable::~SymbolTable() {
    for (auto value : values) {
        if (!value)
            continue;
        delete value;
        value = nullptr;
    }
    delete instance;
}

void SymbolTable::SetObjByInd(size_t index, Object *obj) {
    if (index >= values.size()) {
        throw std::runtime_error("Index out of range");
    }

    values[index] = obj;
}

SymbolTable::SymbolTable() {
    values.push_back(new Str("null","null"));
}

bool SymbolTable::Contains(const std::string &key) {
    return table.contains(key);
}

void SymbolTable::DelObjByInd(size_t index) {
    if (index >= values.size()) {
        throw std::runtime_error("Index out of range");
    }

    delete values[index];
    values[index] = nullptr;
}

size_t SymbolTable::Add(Object *obj) {
    size_t index = values.size();
    values.push_back(obj);

    return index;
}
