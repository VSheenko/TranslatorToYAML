#ifndef TRANSLATORTOYAML_CONTAINER_H
#define TRANSLATORTOYAML_CONTAINER_H

#include "Object.h"

class Container : public Object {
protected:
    std::vector<Object*> children;

public:
    Container() = default;
    explicit Container(const std::string& name);

    virtual void Add(Object* value);
    void TranslateToYaml(std::ostream& out, const std::string& prefix) override;
    std::string GetTypeName() override;
    ~Container();
};

#endif //TRANSLATORTOYAML_CONTAINER_H
