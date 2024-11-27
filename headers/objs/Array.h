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
            out << prefix << "  - ";
            if (child->GetTypeName() == "Array" || child->GetTypeName() == "Dict") {
                child->TranslateToYamlLine(out);
                out << std::endl;
                continue;
            }

            out << child->GetValueStr() << std::endl;
        }
    }

    void TranslateToYamlLine(std::ostream& out) override {
        out << name << ": [";
        for (int i = 0; i < children.size(); i++) {
            if (children[i]->GetTypeName() == "Array" || children[i]->GetTypeName() == "Dict") {
                children[i]->TranslateToYamlLine(out);
                continue;
            }

            out << children[i]->GetValueStr() << (i == children.size() - 1 ? "" : ", ");
        }
        out << "]";
    }

    std::string GetTypeName() override {
        return "Array";
    }

    Object* copy() override {
        auto* copy = new Array(name);
        for (auto child : children) {
            copy->Add(child->copy());
        }
        return copy;
    }

    ~Array() {
        for (auto child : children) {
            if (!child)
                continue;
            delete child;
            child = nullptr;
        }
    }
};

#endif //TRANSLATORTOYAML_ARRAY_H
