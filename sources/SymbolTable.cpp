#include "../headers/SymbolTable.h"
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


Object *SymbolTable::GetByInd(size_t index) {
    if (index >= values.size()) {
        throw std::runtime_error("Index out of range");
    }

    return values[index];
}

size_t SymbolTable::Add(const std::string &key, Object *obj) {
    if (table.contains(key)) {
        throw std::runtime_error("Variable already exists");
    }

    size_t index = values.size();
    table[key] = index;
    values.push_back(obj);

    return index;
}

SymbolTable::~SymbolTable() {
    for (auto value : values) {
        delete value;
    }
    delete instance;
}
