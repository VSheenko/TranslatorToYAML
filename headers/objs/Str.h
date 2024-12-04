#ifndef TRANSLATORTOYAML_STR_H
#define TRANSLATORTOYAML_STR_H

#include "Object.h"

class Str : public Object {
protected:
    std::string value;

public:
    explicit Str(std::string name, std::string s);

    std::string GetValueStr () override;
    void SetValue(const std::string& s);
    void TranslateToYaml(std::ostream &out, const std::string& prefix) override;
    std::string GetTypeName() override;
    Object* copy() override;
};

#endif //TRANSLATORTOYAML_STR_H
