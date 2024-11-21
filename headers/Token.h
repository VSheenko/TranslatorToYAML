#ifndef TRANSLATORTOYAML_TOKEN_H
#define TRANSLATORTOYAML_TOKEN_H

#include <vector>
#include <regex>


enum TAG {
    ID                  = 1 << 0,  // Идентификатор
    NUMBER              = 1 << 1,  // Число
    PLUS                = 1 << 2,  // +
    MINUS               = 1 << 3,  // -
    MUL                 = 1 << 4,  // *
    DIV                 = 1 << 5,  // /
    RPAREN              = 1 << 6,  // )
    RBRACE              = 1 << 7,  // }
    RBRACKET            = 1 << 8,  // ]
    SEMICOLON           = 1 << 9,  // ;
    COMMA               = 1 << 10, // ,
    EXPR_START          = 1 << 11, // ?[
    ARRAY_START         = 1 << 12, // array(
    LINE_COMMENT        = 1 << 13, // !
    BLOCK_COMMENT_START = 1 << 14, // {{--
    BLOCK_COMMENT_END   = 1 << 15, // --}}
    VAR                 = 1 << 16, // var
    END                 = 1 << 17, // Конец файла
    VAR_ASSIGN          = 1 << 18, // :=
    ASSIGN              = 1 << 19, // =
    SPACE               = 1 << 20, // Пробел
    UNKNOWN             = 1 << 21, // Неизвестный токен
    ERROR               = 1 << 22, // Ошибка
    NEW_LINE            = 1 << 23, // Перенос строки
    NEW_ID              = 1 << 24, // Новый идентификатор
    ALL                 = (1 << 25) - 1 // Все теги
};

struct Token {
    TAG tag = TAG::UNKNOWN;
    size_t atr = 0;
};


#endif //TRANSLATORTOYAML_TOKEN_H
