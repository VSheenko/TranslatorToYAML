#ifndef TRANSLATORTOYAML_CONTAINER_H
#define TRANSLATORTOYAML_CONTAINER_H

#include "Object.h"
#include "Value.h"

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
            out << std::endl;
        }
    }

    Container() = default;
    explicit Container(const std::string& name) : Object(name) {};
    ~Container() {
        for (auto child : children) {
            delete child;
        }
    }

    std::string GetTypeName() override {
        return "Container";
    }
};

#endif //TRANSLATORTOYAML_CONTAINER_H
