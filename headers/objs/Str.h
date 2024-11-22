#ifndef TRANSLATORTOYAML_STR_H
#define TRANSLATORTOYAML_STR_H

#include "Object.h"

class Str : public Object {
protected:
    std::string sValue;

public:
    explicit Str(std::string name, std::string s) : Object(std::move(name)), sValue(std::move(s)) {}

    std::string GetValue() {
        return sValue;
    }

    void SetValue(const std::string& s) {
        sValue = s;
    }

    void TranslateToYaml(std::ostream &out, const std::string& prefix) override {
        out << prefix;

        if (!name.empty())
            out << name << ": ";
        else
            out << "- ";

        out << sValue << std::endl;
    }

    std::string GetTypeName() override {
        return "Str";
    }
};

#endif //TRANSLATORTOYAML_STR_H
