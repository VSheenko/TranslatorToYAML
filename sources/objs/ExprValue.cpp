#include "ExprValue.h"
#include <cmath>

ExprValue::ExprValue(const std::string &name, std::string sExpr) : ExprObj(sExpr), Value(name, 0) {
    parts = Split(sExpr);
    Calculate();
}

double ExprValue::add(double a, double b) {
    return a + b;
}

double ExprValue::sub(double a, double b) {
    return a - b;
}

double ExprValue::mul(double a, double b) {
    return a * b;
}

double ExprValue::div(double a, double b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero");
    }

    return a / b;
}

double ExprValue::max(double a, double b) {
    return std::max(a, b);
}

double ExprValue::sqrt(double a) {
    return std::sqrt(a);
}

double ExprValue::calcOp(std::stack<std::string>& stack, double (*op)(double, double)) {
    if (stack.size() < 2) {
        throw std::runtime_error("Invalid expression: " + sExpr);
    }

    double a = std::stod(stack.top());
    stack.pop();
    double b = std::stod(stack.top());
    stack.pop();


    return op(a, b);
}

double ExprValue::calcOp(std::stack<std::string> &stack, double (*op)(double)) {
    if (stack.empty()) {
        throw std::runtime_error("Invalid num expression: " + sExpr);
    }

    double a = std::stod(stack.top());
    stack.pop();

    return op(a);
}

void ExprValue::Calculate() {
    if (sExpr.empty()) {
        throw std::runtime_error("Empty num expression");
    }
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
        } else if (parts[i] == "sqrt") {
            st.push(std::to_string(calcOp(st, sqrt)));
        } else if (parts[i] == "max") {
            st.push(std::to_string(calcOp(st, max)));
        } else {
            st.push(parts[i]);
        }
    }

    if (st.size() != 1) {
        throw std::runtime_error("Invalid expression: " + sExpr);
    }

    value = std::stod(st.top());
}

std::string ExprValue::GetTypeName() {
    return "ExprValue";
}

std::string ExprValue::GetValueStr() {
    return Value::GetValueStr();
}

double ExprValue::GetValueDouble() {
    return Value::GetValueDouble();
}

std::string ExprValue::GetName() {
    return Value::GetName();
}

