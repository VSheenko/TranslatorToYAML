#ifndef TRANSLATORTOYAML_VALUE_H
#define TRANSLATORTOYAML_VALUE_H
#include "Object.h"

class Value : public Object {
protected:
    size_t index_table;
    SymbolTable* table = nullptr;

public:
    explicit Value(std::string name, size_t ind) : Object(std::move(name)), index_table(ind) {
        table = SymbolTable::GetTable();
    }

    double GetValue() {
        std::variant<double, std::string> value;
        if (table->GetValue(index_table, value)) {
            throw std::runtime_error("Value is not a number");
        }

        return std::get<double>(value);
    }

    void SetValue(double value) {
        table->SetValue(index_table, value);
    }

    void TranslateToYaml(std::ostream &out, const std::string& prefix) override {
        double value = GetValue();

        out << prefix << "- ";

        if (value == (int)value) {
            out << std::to_string((int)value);
        } else {
            out << std::to_string(value);
        }
        out << std::endl;
    }
};
#endif //TRANSLATORTOYAML_VALUE_H
