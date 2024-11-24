#ifndef TRANSLATORTOYAML_EXPR_H
#define TRANSLATORTOYAML_EXPR_H

#include "Value.h"
#include <stack>
#include <utility>

class Expr : public Value {
private:
    std::string sExpr;
    std::vector<std::string> parts;

    static double add(double a, double b);
    static double sub(double a, double b);
    static double mul(double a, double b);
    static double div(double a, double b);
    static double calcOp(std::stack<std::string>& stack, double (*op)(double, double));

    std::vector<std::string> Split(std::string s, const std::string& separator);

public:
    Expr(const std::string& name, std::string  sExpr) : Value(name, 0), sExpr(std::move(sExpr)) {
        value = Calculate();
    }

    std::string GetSExpr();
    double Calculate();
};
#endif //TRANSLATORTOYAML_EXPR_H
