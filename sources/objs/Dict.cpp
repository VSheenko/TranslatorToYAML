#include "Dict.h"

Dict::Dict(const std::string &name) : Container(name) {}


void Dict::TranslateToYaml(std::ostream &out, const std::string &prefix) {
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

void Dict::TranslateToYamlLine(std::ostream &out) {
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

std::string Dict::GetTypeName() {
    return "Dict";
}

Object *Dict::copy() {
    auto clone = new Dict(name);
    for (auto child : children) {
        clone->Add(child->copy());
    }
    return clone;
}

Dict::~Dict() {
    for (auto child : children) {
        if (!child)
            continue;
        delete child;
        child = nullptr;
    }
}


