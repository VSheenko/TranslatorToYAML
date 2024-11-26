#include "../headers/objs/Expr.h"

double Expr::Calculate() {
    if (sExpr.empty()) {
        throw std::runtime_error("Empty expression");
    }

    parts = Split(sExpr, " ");
    std::stack<std::string> st;

    for (int i = (int)parts.size() - 1; i >= 0; i--) {
        if (parts[i] == "+") {
            st.push(std::to_string(calcOp(st, add)));
        } else if (parts[i] == "-") {
            st.push(std::to_string(calcOp(st, sub)));
        } else if (parts[i] == "*") {
            st.push(std::to_string(calcOp(st, mul)));
        } else if (parts[i] == "/") {
            st.push(std::to_string(calcOp(st, div)));
        } else {
            st.push(parts[i]);
        }
    }

    if (st.size() != 1) {
        throw std::runtime_error("Invalid expression: " + sExpr);
    }

    return std::stod(st.top());
}

std::vector<std::string> Expr::Split(std::string s, const std::string& separator) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(separator)) != std::string::npos) {
        token = s.substr(0, pos);

        if (!token.empty())
            tokens.push_back(token);

        s.erase(0, pos + separator.length());
    }

    if (!s.empty())
        tokens.push_back(s);

    return tokens;
}

double Expr::add(double a, double b) {
    return a + b;
}

double Expr::sub(double a, double b) {
    return a - b;
}

double Expr::mul(double a, double b) {
    return a * b;
}

double Expr::div(double a, double b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero");
    }

    return a / b;
}

double Expr::calcOp(std::stack<std::string>& stack, double (*op)(double, double)) {
    if (stack.size() < 2) {
        throw std::runtime_error("Invalid expression: " + sExpr);
    }

    double a = std::stod(stack.top());
    stack.pop();
    double b = std::stod(stack.top());
    stack.pop();


    return op(a, b);
}

std::string Expr::GetSExpr(){
    return sExpr;
}

