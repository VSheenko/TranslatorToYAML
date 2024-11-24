#ifndef TRANSLATORTOYAML_VALUE_H
#define TRANSLATORTOYAML_VALUE_H
#include "Object.h"

class Value : public Object {
protected:
    double value;

public:
    explicit Value(std::string name, double value) : Object(std::move(name)), value(value) {}

    double GetValueDouble() override {
        return value;
    }

    std::string GetValueStr() override {
        int int_value = (int)value;

        if (value == int_value) {
            return std::to_string(int_value);
        }
        return std::to_string(value);
    }

    void SetValue(double value) {
        this->value = value;
    }

    void TranslateToYaml(std::ostream &out, const std::string& prefix) override {
        out << prefix;
        out << name << ": ";

        if (value == (int)value) {
            out << std::to_string((int)value);
        } else {
            out << std::to_string(value);
        }

        out << std::endl;
    }


    std::string GetTypeName() override {
        return "Value";
    }
};
#endif //TRANSLATORTOYAML_VALUE_H
