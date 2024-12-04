#ifndef TRANSLATORTOYAML_EXPR_H
#define TRANSLATORTOYAML_EXPR_H

#include "ExprStr.h"
#include "ExprValue.h"

class ExprCreator {
private:
    ExprCreator() = default;

    static bool isValueExpr(const std::string& s);
    static bool isStrExpr(const std::string& s);
public:
    static ExprObj* ExprCreate(std::string name, std::string& expr);
};


#endif //TRANSLATORTOYAML_EXPR_H
