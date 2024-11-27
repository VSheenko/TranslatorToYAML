#include "../headers/Parser.h"
#include <cmath>

#define base_level (TAG::ARRAY_START | TAG::VAR | TAG::DICT_START | TAG::BLOCK_COMMENT_START | \
                    TAG::LINE_COMMENT | TAG::ID | TAG::EXPR_START | TAG::PLUS)

Parser::Parser(std::ifstream *input) {
    lexer = new Lexer(input);
    root = new Container();
}

Parser::~Parser() {
    delete root;
    delete lexer;
}

void Parser::Parse() {
    Object* lastObj = nullptr;

    Token token = lexer->GetNextToken(static_cast<TAG>(base_level));
    while (token.tag != TAG::END) {
        if (token.tag == TAG::ARRAY_START) {
            root->Add(CreateArray("array"));
        } else if (token.tag == TAG::VAR) {
            lastObj = CreateVar();
            root->Add(lastObj);
        } else if (token.tag == TAG::DICT_START) {
            root->Add(CreateDict("dict"));
            lastObj = nullptr;
        } else if (token.tag == EXPR_START) {
            lastObj = CreateExpr("expr");
            root->Add(lastObj);
        } else if (token.tag == TAG::ID) {
            lastObj = SymbolTable::GetTable()->GetObjByInd(token.atr);
            root->Add(lastObj);
        } else if (token.tag == TAG::PLUS) {
            AddToLast(lastObj);
        }

        token = lexer->GetNextToken(static_cast<TAG>(base_level));
    }
}


#define array_level (TAG::NUMBER | TAG::ID | TAG::EXPR_START | \
                     TAG::RPAREN | TAG::STRING | TAG::ARRAY_START | TAG::DICT_START | TAG::F_SQRT | TAG::F_MAX)
Array *Parser::CreateArray(const std::string& name) {
    Object* lastObj = nullptr;

    auto* array = new Array(name);
    Token token = lexer->GetNextToken(static_cast<TAG>(array_level));
    while (token.tag != TAG::RPAREN) {
        if (token.tag == TAG::END) {
            lexer->CallError("Expected ')'");
        }

        if (token.tag == TAG::NUMBER | token.tag == TAG::STRING | token.tag == TAG::ID) {
            lastObj = SymbolTable::GetTable()->GetObjByInd(token.atr)->copy();
            SymbolTable::GetTable()->Add(lastObj);
            array->Add(lastObj);
        } else if (token.tag == TAG::EXPR_START) {
            lastObj = CreateExpr("expr");
            array->Add(lastObj);
        } else if (token.tag == TAG::ARRAY_START) {
            array->Add(CreateArray("array"));
        } else if (token.tag == TAG::DICT_START) {
            array->Add(CreateDict("dict"));
        } else if (token.tag == TAG::F_SQRT) {
            array->Add(Sqrt("sqrt"));
        } else if (token.tag == TAG::F_MAX) {
            array->Add(Max("max"));
        }

        token = lexer->GetNextToken(static_cast<TAG>(TAG::COMMA | TAG::RPAREN | TAG::PLUS));

        if (token.tag == TAG::COMMA) {
            token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID | TAG::STRING |
                    TAG::ARRAY_START | TAG::DICT_START | TAG::F_SQRT | TAG::F_MAX));
        } else if (token.tag == TAG::PLUS) {
            AddToLast(lastObj);
        }
    }

    return array;
}

Expr *Parser::CreateExpr(const std::string& name) {
    Token token = lexer->GetNextToken(static_cast<TAG>(TAG::PLUS | TAG::MINUS | TAG::MUL | TAG::DIV |
                  TAG::RBRACKET | TAG::NUMBER | TAG::ID | TAG::EXPR_START | TAG::F_SQRT));
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
        } else if (token.tag == TAG::F_SQRT) {
            expr += Sqrt("")->GetValueStr() + " ";
        } else if (token.tag == TAG::F_MAX) {
            expr += Max("")->GetValueStr() + " ";
        }

        token = lexer->GetNextToken(static_cast<TAG>(TAG::PLUS | TAG::MINUS | TAG::MUL | TAG::DIV |
                TAG::RBRACKET | TAG::NUMBER | TAG::ID | TAG::EXPR_START | TAG::F_SQRT));

    }

    auto* obj = new Expr(name, expr);
    SymbolTable::GetTable()->Add(obj);
    return obj;
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
            TAG::ARRAY_START | TAG::DICT_START | TAG::F_SQRT | TAG::F_MAX));

    if (token.tag == TAG::EXPR_START) {
        SymbolTable::GetTable()->SetObjByInd(ind, CreateExpr(var_name));
    } else if (token.tag == TAG::NUMBER || token.tag == TAG::STRING || token.tag == TAG::ID) {
        SymbolTable::GetTable()->SetObjByInd(ind, SymbolTable::GetTable()->GetObjByInd(token.atr)->copy());
        SymbolTable::GetTable()->GetObjByInd(ind)->SetName(var_name);
    } else if (token.tag == TAG::ARRAY_START) {
        SymbolTable::GetTable()->SetObjByInd(ind, CreateArray(var_name));
    } else if (token.tag == TAG::DICT_START) {
        SymbolTable::GetTable()->SetObjByInd(ind, CreateDict(var_name));
    } else if (token.tag == TAG::F_SQRT) {
        SymbolTable::GetTable()->SetObjByInd(ind, Sqrt(var_name));
    } else if (token.tag == TAG::F_MAX) {
        SymbolTable::GetTable()->SetObjByInd(ind, Max(var_name));
    }

    return SymbolTable::GetTable()->GetObjByInd(ind);
}

Dict *Parser::CreateDict(const std::string &name) {
    Object* lastObj = nullptr;

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
                TAG::ARRAY_START | TAG::DICT_START | TAG::F_SQRT | TAG::F_MAX));

        if (token.tag == TAG::NUMBER || token.tag == TAG::STRING || token.tag == TAG::ID) {
            lastObj = SymbolTable::GetTable()->GetObjByInd(token.atr)->copy();
            lastObj->SetName(var_name);
            SymbolTable::GetTable()->Add(lastObj);
            dict->Add(lastObj);
        } else if (token.tag == TAG::EXPR_START) {
            lastObj = CreateExpr(var_name);
            dict->Add(lastObj);
        } else if (token.tag == TAG::ARRAY_START) {
            dict->Add(CreateArray(var_name));
        } else if (token.tag == TAG::DICT_START) {
            dict->Add(CreateDict(var_name));
        } else if (token.tag == TAG::F_SQRT) {
            dict->Add(Sqrt(var_name));
        } else if (token.tag == TAG::F_MAX) {
            dict->Add(Max(var_name));
        }

        token = lexer->GetNextToken(static_cast<TAG>(TAG::SEMICOLON | TAG::PLUS));
        while (token.tag == TAG::PLUS) {
            AddToLast(lastObj);
            token = lexer->GetNextToken(static_cast<TAG>(TAG::SEMICOLON | TAG::PLUS));
        }

        token = lexer->GetNextToken(static_cast<TAG>(TAG::NEW_ID | TAG::RBRACE));
    }

    return dict;
}

void Parser::AddToLast(Object* lastObj) {
    if (!lastObj) {
        lexer->CallError("Unexpected token: \"+\"");
        return;
    }

    Token token;
    Object* obj = nullptr;
    if (lastObj->GetTypeName() == "Value") {
        token = lexer->GetNextToken(static_cast<TAG>(TAG::NUMBER | TAG::ID | TAG::EXPR_START));
        obj = SymbolTable::GetTable()->GetObjByInd(token.atr);
        if (obj->GetTypeName() != "Value") {
            lexer->CallError("After number expected number to add");
            return;
        }
        *lastObj += *obj;
    } else if (lastObj->GetTypeName() == "Str") {
        token = lexer->GetNextToken(static_cast<TAG>(TAG::STRING | TAG::ID));
        obj = SymbolTable::GetTable()->GetObjByInd(token.atr);
        if (obj->GetTypeName() != "Str") {
            lexer->CallError("After string expected string to concatenate");
            return;
        }
        *lastObj += *obj;
    } else {
        lexer->CallError("Unexpected token: \"+\"");
        return;
    }
}

Value* Parser::Sqrt(const std::string& obj_name) {
    Token token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID));
    double res = 0;
    if (token.tag == TAG::EXPR_START) {
        res = sqrt(CreateExpr("sqrt")->Calculate());
    } else if (token.tag == TAG::NUMBER) {
        res = sqrt(SymbolTable::GetTable()->GetObjByInd(token.atr)->GetValueDouble());
    } else if (token.tag == TAG::ID) {
        Object* obj = SymbolTable::GetTable()->GetObjByInd(token.atr);
        if (obj->GetTypeName() != "Value") {
            lexer->CallError("Expected number");
        }

        res = sqrt(obj->GetValueDouble());
    }

    lexer->GetNextToken(TAG::RPAREN);

    auto* obj = new Value(obj_name, res);
    SymbolTable::GetTable()->Add(obj);
    return obj;
}

Value* Parser::Max(const std::string& obj_name) {
    Token token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID | TAG::F_SQRT | TAG::F_MAX));

    bool success = false;
    double maxim;

    while (token.tag != TAG::RPAREN) {
        if (token.tag == TAG::END) {
            lexer->CallError("Expected ')'");
        }

        if (token.tag == TAG::EXPR_START) {
            maxim = success ? std::max(maxim, CreateExpr("max")->Calculate()) : CreateExpr("max")->Calculate();
        } else if (token.tag == TAG::NUMBER) {
            maxim = success ? std::max(maxim, SymbolTable::GetTable()->GetObjByInd(token.atr)->GetValueDouble())
                            : SymbolTable::GetTable()->GetObjByInd(token.atr)->GetValueDouble();
        } else if (token.tag == TAG::ID) {
            Object *obj = SymbolTable::GetTable()->GetObjByInd(token.atr);
            if (obj->GetTypeName() != "Value") {
                lexer->CallError("Expected number");
            }

            maxim = success ? std::max(maxim, obj->GetValueDouble()) : obj->GetValueDouble();
        } else if (token.tag == TAG::F_SQRT) {
            maxim = success ? std::max(maxim, Sqrt("")->GetValueDouble()) : Sqrt("")->GetValueDouble();
        } else if (token.tag == TAG::F_MAX) {
            maxim = success ? std::max(maxim, Max("")->GetValueDouble()) : Max("")->GetValueDouble();
        }

        success = true;
        token = lexer->GetNextToken(static_cast<TAG>(TAG::COMMA | TAG::RPAREN));
        if (token.tag == TAG::COMMA) {
            token = lexer->GetNextToken(static_cast<TAG>(TAG::EXPR_START | TAG::NUMBER | TAG::ID | TAG::F_SQRT | TAG::F_MAX));
        }
    }

    if (!success) {
        lexer->CallError("Expected number");
    }

    auto* obj = new Value(obj_name, maxim);
    SymbolTable::GetTable()->Add(obj);
    return obj;
}
