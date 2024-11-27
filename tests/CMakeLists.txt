cmake_minimum_required(VERSION 3.29)
project(TranslatorTest)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB MANAGER
        ../headers/Token.h
        ../headers/Lexer.h
        ../headers/Parser.h
        ../sources/Lexer.cpp
        ../headers/objs/Object.h
        ../headers/objs/Value.h
        ../headers/objs/Container.h
        ../headers/SymbolTable.h
        ../sources/SymbolTable.cpp
        ../headers/TranslatorToYAML.h
        ../sources/Parser.cpp
        ../sources/TranslatorToYAML.cpp
        ../headers/objs/objs.h
        ../headers/objs/Array.h
        ../headers/objs/Expr.h
        ../headers/objs/Dict.h
        ../headers/objs/Str.h
        ../sources/Expr.cpp
)

add_executable(${PROJECT_NAME}
        TranslatorTest.cpp
        ${MANAGER}
)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
