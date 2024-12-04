#ifndef TRANSLATORTOYAML_VALUE_H
#define TRANSLATORTOYAML_VALUE_H
#include "Object.h"

class Value : public Object {
protected:
    double value;

public:
    Value(std::string name, double value) : Object(std::move(name)), value(value) {}
    explicit Value(double value) : value(value) {};

    double GetValueDouble() override {
        return value;
    }

    Object& operator+=(Object& other) override {
        if (other.GetTypeName() != "Value") {
            throw std::runtime_error("Cannot add " + other.GetTypeName() + " to Value");
        }

        value += other.GetValueDouble();
        return *this;
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
        out << name << ": " << GetValueStr();
        out << std::endl;
    }


    std::string GetTypeName() override {
        return "Value";
    }

    Object* copy() override {
        return new Value(name, value);
    }
};
#endif //TRANSLATORTOYAML_VALUE_H
