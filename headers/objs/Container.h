#ifndef TRANSLATORTOYAML_CONTAINER_H
#define TRANSLATORTOYAML_CONTAINER_H

#include "Object.h"

class Container : public Object {
protected:
    std::vector<Object*> children;

public:
    virtual void Add(Object* value) {
        children.push_back(value);
    };

    void TranslateToYaml(std::ostream& out, const std::string& prefix) override {
        for (auto child : children) {
            child->TranslateToYaml(out, prefix);
            if (child != children.back()) {
                out << std::endl;
            }
        }
    }

    Container() = default;
    explicit Container(const std::string& name) : Object(name) {};

    std::string GetTypeName() override {
        return "Container";
    }

    ~Container() {
        for (auto child : children) {
            if (!child)
                continue;
            delete child;
            child = nullptr;
        }
    }
};

#endif //TRANSLATORTOYAML_CONTAINER_H
