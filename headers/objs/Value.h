#ifndef TRANSLATORTOYAML_VALUE_H
#define TRANSLATORTOYAML_VALUE_H
#include "Object.h"

class Value : public Object {
protected:
    double value;

public:
    Value(std::string name, double value);
    explicit Value(double value);

    double GetValueDouble() override;

    std::string GetValueStr() override;

    void SetValue(double value);

    void TranslateToYaml(std::ostream &out, const std::string& prefix) override;


    std::string GetTypeName() override;

    Object* copy() override;
};
#endif //TRANSLATORTOYAML_VALUE_H
