#include "Str.h"

Str::Str(std::string name, std::string s) : Object(std::move(name)), value(std::move(s)) {}

std::string Str::GetValueStr() {
    return value;
}

void Str::SetValue(const std::string &s) {
    value = s;
}

void Str::TranslateToYaml(std::ostream &out, const std::string &prefix) {
    out << prefix;

    if (!name.empty())
        out << name << ": ";
    else
        out << "- ";

    out << value << std::endl;
}

std::string Str::GetTypeName() {
    return "Str";
}

Object *Str::copy() {
    return new Str(name, value);
}



