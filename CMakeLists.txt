cmake_minimum_required(VERSION 3.29)
project(TranslatorToYAML)

set(CMAKE_CXX_STANDARD 23)

add_executable(TranslatorToYAML main.cpp
        headers/Token.h
        headers/Lexer.h
        headers/Parser.h
        headers/operations.h)
