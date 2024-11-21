#ifndef TRANSLATORTOYAML_OBJECT_H
#define TRANSLATORTOYAML_OBJECT_H

#include "../SymbolTable.h"
#include <ostream>
#include <utility>

class Object {
protected:
    std::string name;
public:
    Object() = default;
    explicit Object(std::string  name) : name(std::move(name)) {}

    virtual void TranslateToYaml(std::ostream &out, const std::string& prefix) = 0;
    virtual ~Object() = default;
};
#endif //TRANSLATORTOYAML_OBJECT_H
