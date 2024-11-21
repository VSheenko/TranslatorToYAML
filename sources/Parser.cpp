#include "../headers/Parser.h"

#define base_level (TAG::ARRAY_START | TAG::VAR | TAG::DICT_START | TAG::BLOCK_COMMENT_START | \
                    TAG::LINE_COMMENT | TAG::ID | TAG::NUMBER | TAG::EXPR_START)

Parser::Parser(std::ifstream *input) {
    lexer = new Lexer(input);
    root = new Container();
}

Parser::~Parser() {
    delete root;
    delete lexer;
}

void Parser::Parse(std::ifstream* input) {


    Token token = lexer->GetNextToken(static_cast<TAG>(base_level));
    while (token.tag != TAG::END) {
        std::cout << lexer->GetTagName(token.tag) << " " << token.atr << std::endl;

        if (token.tag == TAG::ARRAY_START) {
            root->Add(CreateArray("array"));
        }

        token = lexer->GetNextToken(static_cast<TAG>(base_level));
    }

}


#define array_level (TAG::NUMBER | TAG::ID | TAG::EXPR_START | \
                     TAG::RPAREN | TAG::STRING)
Array *Parser::CreateArray(const std::string& name) {
    auto* array = new Array(name);
    Token token = lexer->GetNextToken(static_cast<TAG>(array_level));
    while (token.tag != TAG::RPAREN) {
        if (token.tag == TAG::END) {
            lexer->CallError("Expected ')'");
        }

        if (token.tag == TAG::NUMBER) {
            array->Add(new Value("value", token.atr));
        } else if (token.tag == TAG::EXPR_START) {
            array->Add(CreateExpr());
        } else if (token.tag == TAG::STRING) {
            array->Add(new Str("string", token.atr));
        }

        token = lexer->GetNextToken(static_cast<TAG>(TAG::COMMA | TAG::RPAREN));

        if (token.tag == TAG::COMMA) {
            token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID | TAG::STRING));
        }
    }

    return array;


}

Expr *Parser::CreateExpr() {
    return nullptr;
}

Container *Parser::GetRoot() {
    return root;
}
