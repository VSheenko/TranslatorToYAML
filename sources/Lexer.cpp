#include "../headers/Lexer.h"

Lexer::Lexer(std::ifstream* file) {
    std::ostringstream ss;
    ss << file->rdbuf();
    input = ss.str();

    tokenPatterns = {
            {std::regex(R"(^array\()"), TAG::ARRAY_START},
            {std::regex(R"(^var\s)"), TAG::VAR},
            {std::regex(R"(^\+)"), TAG::PLUS},
            {std::regex(R"(^\-)"), TAG::MINUS},
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
            {std::regex(R"(^\d+)"), TAG::NUMBER},
            {std::regex(R"(^[a-z][a-z0-9]*)"), TAG::ID}
    };

    tagNames = {
            {TAG::ID, "ID"},
            {TAG::NUMBER, "NUMBER"},
            {TAG::PLUS, "PLUS"},
            {TAG::MINUS, "MINUS"},
            {TAG::MUL, "MUL"},
            {TAG::DIV, "DIV"},
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
            {TAG::ERROR, "ERROR"}
    };
}

Token Lexer::GetNextToken(TAG expected_tags) {
    if (currentIndex >= input.size()) {
        return {TAG::END};
    }

    SkipNewLine();
    SkipSpaces();

    std::string remainingInput = input.substr(currentIndex);

    for (const auto& [pattern, type] : tokenPatterns) {
        std::smatch match;
        if (std::regex_search(remainingInput, match, pattern)) {
            std::string matchedToken = match.str();
            currentIndex += matchedToken.size();
            if (!(type & expected_tags)) {
                return {TAG::ERROR, 0};
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

            if (type == TAG::ID) {
                return {type, curLine};
            }

            return {type, curLine};
        }
    }

    return {TAG::UNKNOWN, -1};
}

void Lexer::ERROR(const std::string &message) {
    std::cerr << "ERROR: " << message << std::endl;
    exit(1);
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

