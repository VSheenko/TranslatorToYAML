#ifndef TRANSLATORTOYAML_STR_H
#define TRANSLATORTOYAML_STR_H

#include "Object.h"

class Str : public Object {
protected:
    size_t index_table;
    SymbolTable* table = nullptr;

public:
    explicit Str(std::string name, size_t ind) : Object(std::move(name)), index_table(ind) {
        table = SymbolTable::GetTable();
    }

    std::string GetValue() {
        std::variant<double, std::string> value;
        if (!table->GetValue(index_table, value)) {
            throw std::runtime_error("Value is not a string");
        }

        return std::get<std::string>(value);
    }

    void SetValue(const std::string& value) {
        table->SetValue(index_table, value);
    }

    void TranslateToYaml(std::ostream &out, const std::string& prefix) override {
        std::string value = GetValue();

        out << prefix << "- " << value << std::endl;
    }
};

#endif //TRANSLATORTOYAML_STR_H
