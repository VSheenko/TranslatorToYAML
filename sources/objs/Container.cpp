#include "Container.h"

Container::Container(const std::string &name) : Object(name) {}

void Container::Add(Object *value) {
    children.push_back(value);
}

void Container::TranslateToYaml(std::ostream &out, const std::string &prefix) {
    for (auto child : children) {
        child->TranslateToYaml(out, prefix);
        if (child != children.back()) {
            out << std::endl;
        }
    }
}

std::string Container::GetTypeName() {
    return "Container";
}

Container::~Container() {
    for (auto child : children) {
        if (!child)
            continue;
        delete child;
        child = nullptr;
    }
}

