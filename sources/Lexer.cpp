#include "Lexer.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <map>

Lexer::Lexer(std::ifstream* file) {
    SymbolTable::GetTable(); // Чтобы наверняка был

    std::ostringstream ss;
    ss << file->rdbuf();
    input = ss.str();

    tokenPatterns = {
            {std::regex(R"(^sqrt)"), TAG::F_SQRT},
            {std::regex(R"(^max)"), TAG::F_MAX},
            {std::regex(R"(^array\()"), TAG::ARRAY_START},
            {std::regex(R"(^var\s)"), TAG::VAR},
            {std::regex(R"(^\*)"), TAG::MUL},
            {std::regex(R"(^\/)"), TAG::DIV},
            {std::regex(R"(^\))"), TAG::RPAREN},
            {std::regex(R"(^\})"), TAG::RBRACE},
            {std::regex(R"(^\])"), TAG::RBRACKET},
            {std::regex(R"(^;)"), TAG::SEMICOLON},
            {std::regex(R"(^,)"), TAG::COMMA},
            {std::regex(R"(^\?\[)"), TAG::EXPR_START},
            {std::regex(R"(^array\()"), TAG::ARRAY_START},
            {std::regex(R"(^!)"), TAG::LINE_COMMENT},
            {std::regex(R"(^\{\{!--)"), TAG::BLOCK_COMMENT_START},
            {std::regex(R"(^--\}\})"), TAG::BLOCK_COMMENT_END},
            {std::regex(R"(^var\b)"), TAG::VAR},
            {std::regex(R"(^$)"), TAG::END},
            {std::regex(R"(^:=)"), TAG::VAR_ASSIGN},
            {std::regex(R"(^=)"), TAG::ASSIGN},
            {std::regex(R"(^\s+)"), TAG::SPACE},
            {std::regex(R"(^[-+]?\d+(\.\d+)?)"), TAG::NUMBER},
            {std::regex(R"(^\+)"), TAG::PLUS},
            {std::regex(R"(^\-)"), TAG::MINUS},
            {std::regex(R"(^[a-z][a-z0-9]*)"), TAG::ID},
            {std::regex(R"(^\"[^\"]*\")"), TAG::STRING},
            {std::regex(R"(^@\{)"), TAG::DICT_START},
    };

    tagNames = {
            {TAG::STRING, "STRING"},
            {TAG::ID, "ID"},
            {TAG::NUMBER, "NUMBER"},

            {TAG::PLUS, "+"},
            {TAG::MINUS, "-"},
            {TAG::MUL, "*"},
            {TAG::DIV, "/"},
            {TAG::F_MAX, "max"},
            {TAG::F_SQRT, "sqrt"},

            {TAG::RPAREN, "RPAREN"},
            {TAG::RBRACE, "RBRACE"},
            {TAG::RBRACKET, "RBRACKET"},
            {TAG::SEMICOLON, "SEMICOLON"},
            {TAG::COMMA, "COMMA"},
            {TAG::EXPR_START, "EXPR_START"},
            {TAG::ARRAY_START, "ARRAY_START"},
            {TAG::LINE_COMMENT, "LINE_COMMENT"},
            {TAG::BLOCK_COMMENT_START, "BLOCK_COMMENT_START"},
            {TAG::BLOCK_COMMENT_END, "BLOCK_COMMENT_END"},
            {TAG::VAR, "VAR"},
            {TAG::END, "END"},
            {TAG::VAR_ASSIGN, "VAR_ASSIGN"},
            {TAG::ASSIGN, "ASSIGN"},
            {TAG::SPACE, "SPACE"},
            {TAG::UNKNOWN, "UNKNOWN"},
    };
}

Token Lexer::GetNextToken(TAG expected_tags) {
    if (currentIndex >= input.size()) {
        return {TAG::END};
    }

    SkipNewLine();
    SkipSpaces();

    std::string remainingInput = input.substr(currentIndex);

    if (remainingInput.empty()) {
        return {TAG::END, 0};
    }

    for (auto [pattern, type] : tokenPatterns) {
        std::smatch match;
        if (std::regex_search(remainingInput, match, pattern)) {
            std::string matchedToken = match.str();
            currentIndex += matchedToken.size();
            if ((type & TAG::ID) && !SymbolTable::GetTable()->Contains(matchedToken)) {
                type = TAG::NEW_ID;
            }

            if (type == TAG::ID && (expected_tags & TAG::NEW_ID)) {
                CallError("The variable already exists: " + matchedToken);
            }

            if(type == TAG::NEW_ID && (expected_tags & TAG::ID)) {
                CallError("The variable does not exist: " + matchedToken);
            }

            if (!(type & expected_tags)) {
                CallError("Unexpected token: " + matchedToken);
            }

            if (type == TAG::LINE_COMMENT) {
                SkipLineComment();
                return GetNextToken(expected_tags);
            }

            if (type == TAG::BLOCK_COMMENT_START) {
                SkipBlockComment();
                return GetNextToken(expected_tags);
            }

            if (type == TAG::NEW_LINE) {
                ++curLine;
                return GetNextToken(expected_tags);
            }
            // ----------------------------------------
            return InitToken(expected_tags, type, matchedToken);
        }
    }

    return CallError("Unexpected token: " + input.substr(currentIndex, 1));
}

Token Lexer::CallError(const std::string &message) {
    throw std::runtime_error("Error at line " + std::to_string(curLine + 1) + ": " + message);
}

void Lexer::SkipLineComment() {
    while (input[currentIndex] != '\n')
        ++currentIndex;
    ++currentIndex;
    ++curLine;
}


std::string Lexer::GetTagName(TAG tag) {
    if (!tagNames.contains(tag)) {
        return "NOT_FOUND";
    }
    return tagNames[tag];
}

void Lexer::SkipBlockComment() {
    while (input.substr(currentIndex, 4) != "--}}")
        if (input[currentIndex++] == '\n')
            ++curLine;

    currentIndex += 4;
}

void Lexer::SkipSpaces() {
    while (currentIndex < input.size() && std::isspace(input[currentIndex])) {
        ++currentIndex;
    }
}

void Lexer::SkipNewLine() {
    while (currentIndex < input.size() && input[currentIndex] == '\n') {
        ++currentIndex;
        ++curLine;
    }
}

Token Lexer::InitToken(TAG expected_tags, TAG tag, const std::string &name) {
    size_t index = 0;

    try {
        if (tag == TAG::NUMBER) {
            index = SymbolTable::GetTable()->Add(new Value("", std::stod(name)));
        } else if (tag == TAG::STRING) {
            index = SymbolTable::GetTable()->Add(new Str("", name));
        } else if (tag == TAG::NEW_ID) {
            if (SymbolTable::GetTable()->Contains(name)) {
                throw std::runtime_error("Variable already exists");
            }

            index = SymbolTable::GetTable()->Add(name, new Str("", name));
        } else if (tag == TAG::ID) {
            index = SymbolTable::GetTable()->GetInd(name);
        }
    } catch (std::exception& e) {
        return CallError(e.what());
    }

    return {tag, index};
}

