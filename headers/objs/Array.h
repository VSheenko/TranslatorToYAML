#ifndef TRANSLATORTOYAML_ARRAY_H
#define TRANSLATORTOYAML_ARRAY_H

#include "Container.h"

class Array : public Container {
public:
    explicit Array(const std::string& name);

    void TranslateToYaml(std::ostream& out, const std::string& prefix) override;
    void TranslateToYamlLine(std::ostream& out) override;
    std::string GetTypeName() override;
    Object* copy() override;

    ~Array();
};

#endif //TRANSLATORTOYAML_ARRAY_H
