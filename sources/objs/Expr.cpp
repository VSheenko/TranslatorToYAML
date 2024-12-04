#include "Expr.h"
#include <regex>

ExprObj *ExprCreator::ExprCreate(const std::string& name, std::string &expr) {
    if (isValueExpr(expr)) {
        return new ExprValue(name, expr);
    } else if (isStrExpr(expr)) {
        return new ExprStr(name, expr);
    } else {
        throw std::runtime_error("Invalid expression: " + expr);
    }
}

bool ExprCreator::isValueExpr(const std::string &s) {
    std::vector<std::string> parts = ExprObj::Split(s);


    std::vector<std::regex> acceptableSymbols = {
            std::regex(R"(\+|\-|\*|\/|sqrt|max)"),
            std::regex(R"(^[+-]?\d+(\.\d+)?$)"),
    };

    for (const auto& part : parts) {
        bool flag = false;
        for (const auto& symbol : acceptableSymbols) {
            flag = flag || std::regex_match(part, symbol);
        }

        if (!flag) {
            return false;
        }
    }

    return true;
}

bool ExprCreator::isStrExpr(const std::string &s) {
    std::vector<std::string> parts = ExprObj::Split(s);

    std::vector<std::regex> acceptableSymbols = {
            std::regex(R"(^\+)"),
            std::regex(R"(^".*?"$)"),
    };

    for (const auto& part : parts) {
        bool flag = false;
        for (const auto& symbol : acceptableSymbols) {
            flag = flag || std::regex_match(part, symbol);
        }

        if (!flag) {
            return false;
        }
    }

    return true;
}

