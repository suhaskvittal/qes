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

# Get grammar specification file for the LL parser.
file(REAL_PATH data/grammar.txt LL_GRAMMAR_ABSOLUTE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# This is a really small library :p
add_library(qasl src/lexer.cpp)
target_compile_options(qasl PRIVATE ${COMPILE_OPTIONS})
target_include_directories(qasl PUBLIC "src")

target_compile_defintions(qasl PUBLIC LL_GRAMMAR_FILE=${LL_GRAMMAR_ABSOLUTE_PATH})

add_executable(test_lexer src/lexer.test.cpp)
target_link_libraries(test_lexer PRIVATE qasl)
