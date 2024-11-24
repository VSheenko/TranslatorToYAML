#ifndef TRANSLATORTOYAML_PARSER_H
#define TRANSLATORTOYAML_PARSER_H

#include "Lexer.h"
#include "objs/objs.h"

class Parser {
private:
    Container* root;
    Lexer* lexer = nullptr;

    Array* CreateArray(const std::string& name);
    Dict* CreateDict(const std::string& name);
    Object* CreateVar();
    Expr* CreateExpr();
public:
    explicit Parser(std::ifstream *input);
    ~Parser();
    void Parse();
    Container* GetRoot();
};

#endif //TRANSLATORTOYAML_PARSER_H
