#ifndef TRANSLATORTOYAML_PARSER_H
#define TRANSLATORTOYAML_PARSER_H

#include "Node.h"
#include "Lexer.h"

class Parser {
private:
    Node* root;

public:
    explicit Parser(std::ifstream* input);
    ~Parser();
    void Parse(std::ifstream* input);
};

#endif //TRANSLATORTOYAML_PARSER_H
