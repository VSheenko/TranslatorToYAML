#ifndef TRANSLATORTOYAML_SYMBOLTABLE_H
#define TRANSLATORTOYAML_SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <vector>
#include <variant>
#include "objs/objs.h"

class SymbolTable {
private:
    SymbolTable() = default;
    static SymbolTable* instance;

    std::vector<Object*> values;
    std::unordered_map<std::string, size_t> table;

public:
    static SymbolTable* GetTable();
    ~SymbolTable();
    size_t Add(const std::string& key, Object* obj);
    Object* GetByInd(size_t index);
    size_t GetInd(const std::string& key);
};



#endif //TRANSLATORTOYAML_SYMBOLTABLE_H
