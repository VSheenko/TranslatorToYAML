#include "Array.h"

Array::Array(const std::string &name) : Container(name) {}

void Array::TranslateToYaml(std::ostream &out, const std::string &prefix) {
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

void Array::TranslateToYamlLine(std::ostream &out) {
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

std::string Array::GetTypeName() {
    return "Array";
}

Object *Array::copy() {
    auto* copy = new Array(name);
    for (auto child : children) {
        copy->Add(child->copy());
    }
    return copy;
}

Array::~Array()  {
    for (auto child : children) {
        if (!child)
            continue;
        delete child;
        child = nullptr;
    }
}



