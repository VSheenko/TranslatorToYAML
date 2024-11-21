#ifndef TRANSLATORTOYAML_ARRAY_H
#define TRANSLATORTOYAML_ARRAY_H

#include "Container.h"

class Array : public Container {
public:
    explicit Array(const std::string& name) : Container(name) {};

    void Add(Object* object) override {
        children.push_back(object);
    }

    void TranslateToYaml(std::ostream& out, const std::string& prefix) override {
        out << prefix << name << ":" << std::endl;
        for (auto child : children) {
            child->TranslateToYaml(out, prefix + "  ");
        }
    }
};

#endif //TRANSLATORTOYAML_ARRAY_H
