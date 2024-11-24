#ifndef TRANSLATORTOYAML_OBJECT_H
#define TRANSLATORTOYAML_OBJECT_H

#include <ostream>
#include <utility>
#include <vector>

class Object {
protected:
    std::string name;

public:
    Object() = default;
    explicit Object(std::string  name) : name(std::move(name)) {}


    virtual std::string GetValueStr() {
        throw std::runtime_error(GetTypeName() + ": GetValueStr not implemented");
    }

    virtual double GetValueDouble() {
        throw std::runtime_error(GetTypeName() + ": GetValueDouble not implemented");
    }

    virtual void SetName(const std::string& s) {
        this->name = s;
    }

    virtual std::string GetName() {
        return name;
    }

    virtual std::string GetTypeName() {return "Object";}

    virtual void TranslateToYaml(std::ostream &out, const std::string& prefix) {
        throw std::runtime_error(GetTypeName() + ": TranslateToYaml mot implemented");
    };

    virtual void TranslateToYamlLine(std::ostream& out) {
        throw std::runtime_error(GetTypeName() + ": TranslateToYamlLine not implemented");
    };

    ~Object() = default;
};
#endif //TRANSLATORTOYAML_OBJECT_H
