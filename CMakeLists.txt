# author:   Suhas Vittal
# date:     4 January 2024

# Qasl: QuAntum Specification Language

cmake_minimum_required(VERSION 3.20.2)
project(Qasl VERSION 0.1)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(COMPILE_OPTIONS -Ofast -fno-strict-aliasing)
else()
    set(COMPILE_OPTIONS -ggdb3 -Wall)
endif()

set(CMAKE_CXX_STANDARD 20)

# Get data files for the grammar and lexer.
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/qasl_lexer.txt" QASL_LEXER_ABSOLUTE_PATH)
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/grammar_lexer.txt" GRAMMAR_LEXER_ABSOLUTE_PATH)
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/qasl_grammar.txt" QASL_LL_GRAMMAR_ABSOLUTE_PATH)

set(QASL_FILES src/qasl/lang/parse.cpp
                src/util/lexer.cpp
                src/util/llparser.cpp)

# This is a really small library :p
add_library(qasl ${QASL_FILES})
target_compile_options(qasl PRIVATE ${COMPILE_OPTIONS})
target_include_directories(qasl PUBLIC "include")

target_compile_definitions(qasl PUBLIC 
    QASL_LEXER_FILE="${QASL_LEXER_ABSOLUTE_PATH}"
    GRAMMAR_LEXER_FILE="${GRAMMAR_LEXER_ABSOLUTE_PATH}"
    QASL_LL_GRAMMAR_FILE="${QASL_LL_GRAMMAR_ABSOLUTE_PATH}")

add_executable(test_lexer src/lexer.test.cpp)
target_link_libraries(test_lexer PRIVATE qasl)

add_executable(test_llparser src/llparser.test.cpp)
target_link_libraries(test_llparser PRIVATE qasl)
