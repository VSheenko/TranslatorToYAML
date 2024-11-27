#ifndef TRANSLATORTOYAML_DICT_H
#define TRANSLATORTOYAML_DICT_H

#include "Container.h"

class Dict : public Container {
public:
    explicit Dict(const std::string&  name) : Container(name) {};

    void Add(Object* object) override {
        children.push_back(object);
    }

    void TranslateToYaml(std::ostream& out, const std::string& prefix) override {
        out << prefix << name << ":" << std::endl;
        for (auto child : children) {
            out << prefix << "  ";
            if (child->GetTypeName() == "Array" || child->GetTypeName() == "Dict") {
                child->TranslateToYamlLine(out);
                out << std::endl;
                continue;
            }

            out << child->GetName() << ": " << child->GetValueStr() << std::endl;
        }
    }

    void TranslateToYamlLine(std::ostream& out) override {
        out << name << ": {";
        for (int i = 0; i < children.size(); i++) {
            if (children[i]->GetTypeName() == "Array" || children[i]->GetTypeName() == "Dict") {
                children[i]->TranslateToYamlLine(out);
                continue;
            }

            out << children[i]->GetName() << ": " << children[i]->GetValueStr() << (i == children.size() - 1 ? "" : ", ");
        }
        out << "}";
    }

    std::string GetTypeName() override {
        return "Dict";
    }

    Object* copy() override {
        auto clone = new Dict(name);
        for (auto child : children) {
            clone->Add(child->copy());
        }
        return clone;
    }

    ~Dict() {
        for (auto child : children) {
            if (!child)
                continue;
            delete child;
            child = nullptr;
        }
    }
};

#endif //TRANSLATORTOYAML_DICT_H
