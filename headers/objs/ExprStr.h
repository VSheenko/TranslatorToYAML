#ifndef TRANSLATORTOYAML_EXPRSTR_H
#define TRANSLATORTOYAML_EXPRSTR_H

#include "ExprObj.h"
#include "Str.h"
#include <stack>

class ExprStr : public ExprObj, public Str {
private:
    static std::string add(const std::string& a, const std::string& b);

    std::string calcOp(std::stack<std::string>& stack, std::string (*op)(const std::string&, const std::string&));

public:
    ExprStr(std::string name, const std::string& value);
    void Calculate() override;
    std::string GetValueStr() override;
    std::string GetName() override;


    ~ExprStr() = default;
};

#endif //TRANSLATORTOYAML_EXPRSTR_H
