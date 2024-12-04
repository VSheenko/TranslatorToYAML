#ifndef TRANSLATORTOYAML_DICT_H
#define TRANSLATORTOYAML_DICT_H

#include "Container.h"

class Dict : public Container {
public:
    explicit Dict(const std::string&  name);

    void TranslateToYaml(std::ostream& out, const std::string& prefix) override;
    void TranslateToYamlLine(std::ostream& out) override;
    std::string GetTypeName() override;
    Object* copy() override;

    ~Dict();
};

#endif //TRANSLATORTOYAML_DICT_H
