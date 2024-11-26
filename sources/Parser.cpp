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

void Parser::Parse() {
    Token token = lexer->GetNextToken(static_cast<TAG>(base_level));
    while (token.tag != TAG::END) {
        if (token.tag == TAG::ARRAY_START) {
            root->Add(CreateArray("array"));
        } else if (token.tag == TAG::VAR) {
            root->Add(CreateVar());
        } else if (token.tag == TAG::DICT_START) {
            root->Add(CreateDict("dict"));
        } else if (token.tag == EXPR_START) {
            root->Add(CreateExpr("expr"));
        } else if (token.tag == TAG::ID) {
            root->Add(SymbolTable::GetTable()->GetObjByInd(token.atr));
        }

        token = lexer->GetNextToken(static_cast<TAG>(base_level));
    }

}


#define array_level (TAG::NUMBER | TAG::ID | TAG::EXPR_START | \
                     TAG::RPAREN | TAG::STRING | TAG::ARRAY_START | TAG::DICT_START)
Array *Parser::CreateArray(const std::string& name) {
    auto* array = new Array(name);
    Token token = lexer->GetNextToken(static_cast<TAG>(array_level));
    while (token.tag != TAG::RPAREN) {
        if (token.tag == TAG::END) {
            lexer->CallError("Expected ')'");
        }

        if (token.tag == TAG::NUMBER | token.tag == TAG::STRING | token.tag == TAG::ID) {
            array->Add(SymbolTable::GetTable()->GetObjByInd(token.atr));
        } else if (token.tag == TAG::EXPR_START) {
            array->Add(CreateExpr("expr"));
        } else if (token.tag == TAG::ARRAY_START) {
            array->Add(CreateArray("array"));
        } else if (token.tag == TAG::DICT_START) {
            array->Add(CreateDict("dict"));
        }

        token = lexer->GetNextToken(static_cast<TAG>(TAG::COMMA | TAG::RPAREN));

        if (token.tag == TAG::COMMA) {
            token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID | TAG::STRING | TAG::ARRAY_START | TAG::DICT_START));
        }
    }

    return array;


}

Expr *Parser::CreateExpr(const std::string& name) {
    Token token = lexer->GetNextToken(static_cast<TAG>(TAG::PLUS | TAG::MINUS | TAG::MUL | TAG::DIV |
                  TAG::RBRACKET | TAG::NUMBER | TAG::ID | TAG::EXPR_START));
    std::string expr;

    while (token.tag != TAG::RBRACKET) {
        if (token.tag == TAG::END) {
            lexer->CallError("Expected ']'");
        }

        if (token.tag == TAG::NUMBER) {
            expr += SymbolTable::GetTable()->GetObjByInd(token.atr)->GetValueStr() + " ";
        } else if (token.tag == TAG::ID) {
            if (SymbolTable::GetTable()->GetObjByInd(token.atr)->GetTypeName() != "Value") {
                lexer->CallError("Expected number");
            }

            expr += SymbolTable::GetTable()->GetObjByInd(token.atr)->GetValueStr();
        } else if (token.tag == TAG::EXPR_START) {
            auto *expr_obj = CreateExpr("array");
            expr += expr_obj->GetSExpr() + " ";
            delete expr_obj;
        } else if (token.tag == TAG::PLUS || token.tag == TAG::MINUS || token.tag == TAG::MUL || token.tag == TAG::DIV) {
            expr += lexer->GetTagName(token.tag) + " ";
        }

        token = lexer->GetNextToken(static_cast<TAG>(TAG::PLUS | TAG::MINUS | TAG::MUL | TAG::DIV |
                TAG::RBRACKET | TAG::NUMBER | TAG::ID | TAG::EXPR_START));

    }
    return new Expr(name, expr);
}

Container *Parser::GetRoot() {
    return root;
}

Object *Parser::CreateVar() {
    Token token = lexer->GetNextToken(TAG::NEW_ID);
    size_t ind = token.atr;
    std::string var_name = SymbolTable::GetTable()->GetObjByInd(ind)->GetValueStr();
    SymbolTable::GetTable()->DelObjByInd(ind);


    lexer->GetNextToken(TAG::VAR_ASSIGN);
    token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID | TAG::STRING |
            TAG::ARRAY_START | TAG::DICT_START));

    if (token.tag == TAG::EXPR_START) {
        SymbolTable::GetTable()->SetObjByInd(ind, CreateExpr(var_name));
    } else if (token.tag == TAG::NUMBER || token.tag == TAG::STRING || token.tag == TAG::ID) {
        SymbolTable::GetTable()->GetObjByInd(token.atr)->SetName(var_name);
        SymbolTable::GetTable()->SetObjByInd(ind, SymbolTable::GetTable()->GetObjByInd(token.atr));
    } else if (token.tag == TAG::ARRAY_START) {
        SymbolTable::GetTable()->SetObjByInd(ind, CreateArray(var_name));
    } else if (token.tag == TAG::DICT_START) {
        SymbolTable::GetTable()->SetObjByInd(ind, CreateDict(var_name));
    }

    return SymbolTable::GetTable()->GetObjByInd(ind);
}

Dict *Parser::CreateDict(const std::string &name) {
    auto* dict = new Dict(name);
    Token token = lexer->GetNextToken(static_cast<TAG>(TAG::NEW_ID | TAG::RBRACE));
    while (token.tag != TAG::RBRACE) {
        if (token.tag == TAG::END) {
            lexer->CallError("Expected '}'");
        }

        size_t ind = token.atr;
        std::string var_name = SymbolTable::GetTable()->GetObjByInd(ind)->GetValueStr();

        lexer->GetNextToken(TAG::ASSIGN);
        token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID | TAG::STRING |
                TAG::ARRAY_START | TAG::DICT_START));

        if (token.tag == TAG::NUMBER || token.tag == TAG::STRING || token.tag == TAG::ID) {
            SymbolTable::GetTable()->GetObjByInd(token.atr)->SetName(var_name);
            dict->Add(SymbolTable::GetTable()->GetObjByInd(token.atr));
        } else if (token.tag == TAG::EXPR_START) {
            dict->Add(CreateExpr(var_name));
        } else if (token.tag == TAG::ARRAY_START) {
            dict->Add(CreateArray(var_name));
        } else if (token.tag == TAG::DICT_START) {
            dict->Add(CreateDict(var_name));
        }

        lexer->GetNextToken(static_cast<TAG>(TAG::SEMICOLON));
        token = lexer->GetNextToken(static_cast<TAG>(TAG::NEW_ID | TAG::RBRACE));
    }

    return dict;
}
