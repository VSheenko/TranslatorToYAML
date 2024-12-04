#include "ExprStr.h"

ExprStr::ExprStr(std::string name, const std::string &sExpr) : ExprObj(sExpr), Str(name, "") {
    parts = Split(sExpr);
    Calculate();
}

std::string ExprStr::add(const std::string& a, const std::string& b) {
    return a.substr(0, a.size() - 1) + b.substr(1);
}

std::string ExprStr::calcOp(std::stack<std::string> &stack, std::string (*op)(const std::string&, const std::string&)) {
    if (stack.size() < 2) {
        throw std::runtime_error("Invalid str expression: " + sExpr);
    }

    std::string a = stack.top();
    stack.pop();
    std::string b = stack.top();
    stack.pop();

    return op(a, b);
}

void ExprStr::Calculate() {
    if (sExpr.empty()) {
        throw std::runtime_error("Empty str expression");
    }
    std::stack<std::string> st;

    for (int i = (int)parts.size() - 1; i >= 0; i--) {
        if (parts[i] == "+") {
            st.push(calcOp(st, add));
        } else {
            st.push(parts[i]);
        }
    }

    if (st.size() != 1) {
        throw std::runtime_error("Invalid str expression: " + sExpr);
    }

    this->value = st.top();
}

std::string ExprStr::GetValueStr() {
    return this->value;
}

std::string ExprStr::GetName() {
    return Str::GetName();
}

