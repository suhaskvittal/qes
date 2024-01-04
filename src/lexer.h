/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#ifndef QASL_LEXER_h
#define QASL_LEXER_h

#include "qasl.h"

#include <iostream>
#include <regex>
#include <tuple>
#include <vector>

namespace qasl {

extern const std::map<token_type, std::string>  TOKEN_TEXT_MAP;
extern const std::map<token_type, std::regex>   REGEX_MAP;
extern const std::vector<token_type>            TOKEN_ORDER;

// Each token has the following entries:
//  (1) token_type
//  (2) value (std::string)
typedef std::tuple<token_type, std::string> Token;

bool    matches(std::string, token_type);

std::string print_token(token_type);
std::string print_token(Token);

// This is a simple lexer for Qasl. It is flexible,
// in that in lexer.cpp, any of the regex can be
// modified, and QaslLexer will work corresponding,
// provided the regex is valid.
class Lexer {
public:
    Lexer();

    void read_tokens_onto_stack(std::string);
    void read_tokens_onto_stack(std::istream&);

    std::vector<Token> get_token_stack(void);
private:
    std::vector<Token> token_stack;
};

}   // qasl

#include "lexer.inl"

#endif  // QASL_LEXER_h
