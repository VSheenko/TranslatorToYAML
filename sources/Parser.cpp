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
        } else if (token.tag == TAG::VAR) {
            root->Add(CreateVar());
        } else if (token.tag == TAG::DICT_START) {
            root->Add(CreateDict("dict"));
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

        if (token.tag == TAG::NUMBER | token.tag == TAG::STRING) {
            array->Add(SymbolTable::GetTable()->GetByInd(token.atr));
        } else if (token.tag == TAG::EXPR_START) {
            array->Add(CreateExpr());
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

Value *Parser::CreateVar() {
    return nullptr;
}

Dict *Parser::CreateDict(const std::string &name) {
    return nullptr;
}
