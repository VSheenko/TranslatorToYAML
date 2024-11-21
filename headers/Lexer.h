#ifndef TRANSLATORTOYAML_LEXER_H
#define TRANSLATORTOYAML_LEXER_H

#include "Token.h"
#include "SymbolTable.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <map>

class Lexer {
private:
    std::string input;
    size_t currentIndex = 0;

    size_t curPos = 0;
    int curLine = 0;

    std::vector<std::pair<std::regex, TAG>> tokenPatterns;
    std::map<TAG, std::string> tagNames;


    Token InitToken(TAG expected_tags, TAG tag, const std::string& name);
    void SkipLineComment();
    void SkipBlockComment();
    void SkipSpaces();
    void SkipNewLine();
    Token ERROR(const std::string& message);
public:
    std::string GetTagName(TAG tag);
    Token GetNextToken(TAG expected_tags);
    explicit Lexer(std::ifstream* file);
};

#endif //TRANSLATORTOYAML_LEXER_H
