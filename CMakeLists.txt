cmake_minimum_required(VERSION 3.29)
project(TranslatorToYAML)

set(CMAKE_CXX_STANDARD 23)

add_executable(TranslatorToYAML main.cpp
        headers/Token.h
        headers/Lexer.h
        headers/Parser.h
        headers/operations.h
        sources/Lexer.cpp
        headers/objs/Object.h
        headers/objs/Value.h
        headers/objs/Container.h
        headers/SymbolTable.h
        sources/SymbolTable.cpp
        headers/TranslatorToYAML.h
        sources/Parser.cpp
        sources/TranslatorToYAML.cpp
        headers/objs/objs.h
        headers/objs/Array.h
        headers/objs/Expr.h
        headers/objs/Dict.h
        headers/objs/Str.h
)
