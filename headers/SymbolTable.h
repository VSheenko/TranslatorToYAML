#ifndef TRANSLATORTOYAML_SYMBOLTABLE_H
#define TRANSLATORTOYAML_SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <vector>

class SymbolTable {
private:
    SymbolTable() = default;
    static SymbolTable* instance;


    std::vector<double> values;
    std::unordered_map<std::string, size_t> table;

public:
    static SymbolTable* GetTable();
    size_t AddValue(const std::string& key, double value);
    size_t AddValue(double value);
    double GetValue(size_t index);
    size_t GetInd(const std::string& key);
    void SetValue(size_t index, double value);
};

#endif //TRANSLATORTOYAML_SYMBOLTABLE_H
