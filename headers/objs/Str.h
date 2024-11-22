#ifndef TRANSLATORTOYAML_STR_H
#define TRANSLATORTOYAML_STR_H

#include "Object.h"

class Str : public Object {
protected:
    std::string value;

public:
    explicit Str(std::string name, std::string s) : Object(std::move(name)), value(std::move(s)) {}

    std::string GetValueStr () override {
        return value;
    }

    void SetValue(const std::string& s) {
        value = s;
    }

    void TranslateToYaml(std::ostream &out, const std::string& prefix) override {
        out << prefix;

        if (!name.empty())
            out << name << ": ";
        else
            out << "- ";

        out << value << std::endl;
    }

    std::string GetTypeName() override {
        return "Str";
    }
};

#endif //TRANSLATORTOYAML_STR_H
