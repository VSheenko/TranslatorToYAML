#include "Object.h"


Object::Object(std::string name)  : name(std::move(name)) {}

void Object::SetName(const std::string &s) {
    this->name = s;
}

std::string Object::GetName() {
    return name;
}

std::string Object::GetTypeName()  {
    return "Object";
}

std::string Object::GetValueStr() {
    throw std::runtime_error(GetTypeName() + ": GetValueStr not implemented");
}

double Object::GetValueDouble() {
    throw std::runtime_error(GetTypeName() + ": GetValueDouble not implemented");
}

void Object::TranslateToYaml(std::ostream &out, const std::string &prefix) {
    throw std::runtime_error(GetTypeName() + ": TranslateToYaml not implemented");
}

void Object::TranslateToYamlLine(std::ostream &out) {
    throw std::runtime_error(GetTypeName() + ": TranslateToYamlLine not implemented");
}


Object *Object::copy() {
    throw std::runtime_error(GetTypeName() + ": copy not implemented");
}

std::string Object::GetSExpr() {
    throw std::runtime_error(GetTypeName() + ": GetSExpr not implemented");
}


