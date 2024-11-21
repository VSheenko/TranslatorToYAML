#include "../headers/Parser.h"

#define base_level (TAG::ARRAY_START | TAG::VAR | TAG::DICT_START | TAG::BLOCK_COMMENT_START | \
                    TAG::LINE_COMMENT | TAG::ID | TAG::NUMBER | TAG::EXPR_START)

Parser::Parser(std::ifstream* input) {
    root = new Node(new Object());
}

Parser::~Parser() {
    delete root;
}

void Parser::Parse(std::ifstream* input) {
    Lexer lexer(input);

    Token token = lexer.GetNextToken(static_cast<TAG>(base_level));
    while (token.tag != TAG::END) {
        std::cout << lexer.GetTagName(token.tag) << " " << token.atr << std::endl;

        token = lexer.GetNextToken(static_cast<TAG>(base_level));
    }

}
