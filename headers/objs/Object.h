#ifndef TRANSLATORTOYAML_OBJECT_H
#define TRANSLATORTOYAML_OBJECT_H

#include <ostream>
#include <utility>
#include <vector>

class Object {
protected:
    std::string name;

public:
    Object() = default;
    explicit Object(std::string  name);

    virtual void SetName(const std::string& s);
    virtual std::string GetName();
    virtual std::string GetTypeName();

    virtual std::string GetValueStr();
    virtual double GetValueDouble();
    virtual void TranslateToYaml(std::ostream &out, const std::string& prefix);
    virtual void TranslateToYamlLine(std::ostream& out);
    virtual Object* copy();
    virtual std::string GetSExpr();

    ~Object() = default;
};
#endif //TRANSLATORTOYAML_OBJECT_H
