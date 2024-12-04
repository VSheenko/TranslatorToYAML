#include "ExprObj.h"
#include <utility>
#include <regex>

ExprObj::ExprObj(std::string sExpr) : sExpr(std::move(sExpr)) {}

void ExprObj::Calculate() {
    throw std::runtime_error("Not implemented");
}

std::vector<std::string> ExprObj::Split(std::string s) {
    std::vector<std::string> parts;
    std::string current;

    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '"') {
            current += '"';
            i++;
            while (s[i] != '"') {
                current += s[i];
                i++;
            }
            current += '"';
        } else if (s[i] == ' ') {
            if (!current.empty()) {
                parts.push_back(current);
                current.clear();
            }
        } else {
            current += s[i];
        }
    }

    if (!current.empty()) {
        parts.push_back(current);
    }

    return parts;
}



std::string ExprObj::GetTypeName() {
    return "ExprObj";
}

std::string ExprObj::GetSExpr() {
    return sExpr;
}

ExprObj::ExprObj(Object *obj) {
    this->obj = obj;
    isIdentifier = true;
}
