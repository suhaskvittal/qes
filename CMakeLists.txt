# author:   Suhas Vittal
# date:     4 January 2024

# QES: Quantum Experiment Specification language

cmake_minimum_required(VERSION 3.20.2)
project(Qes VERSION 0.1)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(COMPILE_OPTIONS -Ofast -fno-strict-aliasing)
else()
    set(COMPILE_OPTIONS -ggdb3 -Wall)
endif()

set(CMAKE_CXX_STANDARD 20)

# Get data files for the grammar and lexer.
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/qes_lexer.txt" QES_LEXER_ABSOLUTE_PATH)
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/grammar_lexer.txt" GRAMMAR_LEXER_ABSOLUTE_PATH)
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/qes_grammar.txt" QES_LL_GRAMMAR_ABSOLUTE_PATH)

set(QES_FILES src/qes/lang/parse.cpp
                src/qes/lang/parse_impl.cpp
                src/qes/util/lexer.cpp
                src/qes/util/llparser.cpp)

# This is a really small library :p
add_library(qes ${QES_FILES})
target_compile_options(qes PRIVATE ${COMPILE_OPTIONS})
target_include_directories(qes PUBLIC "include")

target_compile_definitions(qes PUBLIC 
    QES_LEXER_FILE="${QES_LEXER_ABSOLUTE_PATH}"
    GRAMMAR_LEXER_FILE="${GRAMMAR_LEXER_ABSOLUTE_PATH}"
    QES_LL_GRAMMAR_FILE="${QES_LL_GRAMMAR_ABSOLUTE_PATH}")

if (COMPILE_TESTS)
    add_executable(test_qes src/qes.test.cpp)
    target_link_libraries(test_qes PRIVATE qes)
endif()
