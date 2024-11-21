#ifndef TRANSLATORTOYAML_SYMBOLTABLE_H
#define TRANSLATORTOYAML_SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <vector>
#include <variant>

class SymbolTable {
private:
    SymbolTable() = default;
    static SymbolTable* instance;

    std::vector<bool> is_string;
    std::vector<std::variant<double, std::string>> values;
    std::unordered_map<std::string, size_t> table;

public:
    static SymbolTable* GetTable();
    size_t AddValue(const std::string& key, double value);
    size_t AddValue(const std::string& key, const std::string& value);
    size_t AddValue(double value);
    size_t AddValue(const std::string& value);
    bool GetValue(size_t index, std::variant<double, std::string>& value);
    size_t GetInd(const std::string& key);
    void SetValue(size_t index, double value);
    void SetValue(size_t index, const std::string& value);
};



#endif //TRANSLATORTOYAML_SYMBOLTABLE_H
