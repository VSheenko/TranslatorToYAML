#ifndef TRANSLATORTOYAML_EXPROBJ_H
#define TRANSLATORTOYAML_EXPROBJ_H

#include "Object.h"


class ExprObj : public Object {
protected:
    std::string sExpr;
    std::vector<std::string> parts;

    bool isIdentifier = false;
    Object* obj = nullptr;
public:
    std::string GetTypeName() override;
    static std::vector<std::string> Split(std::string s);
    explicit ExprObj(std::string  sExpr);
    explicit ExprObj(Object* obj);

    virtual void Calculate() = 0;
    std::string GetSExpr() override;
};

#endif //TRANSLATORTOYAML_EXPROBJ_H
