#ifndef TRANSLATORTOYAML_EXPRVALUE_H
#define TRANSLATORTOYAML_EXPRVALUE_H

#include "ExprObj.h"
#include "Value.h"
#include <stack>

class ExprValue : public ExprObj, public Value {
private:
    static double add(double a, double b);
    static double sub(double a, double b);
    static double mul(double a, double b);
    static double div(double a, double b);
    static double max(double a, double b);
    static double sqrt(double a);

    double calcOp(std::stack<std::string>& stack, double (*op)(double, double));
    double calcOp(std::stack<std::string>& stack, double (*op)(double));


public:
    explicit ExprValue(const std::string& name, std::string sExpr);
    void Calculate() override;

    std::string GetTypeName() override;
    std::string GetValueStr() override;
    double GetValueDouble() override;
    std::string GetName() override;

    ~ExprValue() = default;
};
#endif //TRANSLATORTOYAML_EXPRVALUE_H
