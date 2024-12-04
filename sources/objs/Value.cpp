#include "Value.h"

Value::Value(std::string name, double value) : Object(std::move(name)), value(value) {}
Value::Value(double value) : value(value) {}

double Value::GetValueDouble() {
    return value;
}

std::string Value::GetValueStr() {
    int int_value = (int)value;

    if (value == int_value) {
        return std::to_string(int_value);
    }
    return std::to_string(value);
}

void Value::SetValue(double value) {
    this->value = value;
}

void Value::TranslateToYaml(std::ostream &out, const std::string &prefix) {
    out << prefix;
    out << name << ": " << GetValueStr();
    out << std::endl;
}

std::string Value::GetTypeName() {
    return "Value";
}

Object *Value::copy() {
    return new Value(name, value);
}
