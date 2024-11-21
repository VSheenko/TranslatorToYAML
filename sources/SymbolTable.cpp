#include "../headers/SymbolTable.h"
SymbolTable* SymbolTable::instance = nullptr;

SymbolTable *SymbolTable::GetTable() {
    if (instance == nullptr) {
        instance = new SymbolTable();
    }
    return instance;
}

size_t SymbolTable::AddValue(const std::string& key, double value) {
    if (table.contains(key)) {
        throw std::runtime_error("Variable already exists");
    }

    size_t index = AddValue(value);
    table[key] = index;

    return index;
}

void SymbolTable::SetValue(size_t index, double value) {
    if (index >= values.size()) {
        throw std::runtime_error("Index out of range");
    }

    values[index] = value;
}

size_t SymbolTable::AddValue(double value) {
    values.push_back(value);
    return values.size() - 1;
}

double SymbolTable::GetValue(size_t index) {
    if (index >= values.size()) {
        throw std::runtime_error("Index out of range");
    }

    return values[index];
}

size_t SymbolTable::GetInd(const std::string &key) {
    if (!table.contains(key)) {
        throw std::runtime_error("Variable not found");
    }

    return table[key];
}

