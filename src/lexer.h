/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#ifndef QASL_LEXER_h
#define QASL_LEXER_h

#include <iostream>
#include <regex>
#include <set>
#include <tuple>
#include <vector>

namespace qasl {

typedef std::string token_type;
// Each token has the following entries:
//  (1) token_type
//  (2) value (std::string)
typedef std::tuple<token_type, std::string> Token;

extern const token_type T_undefined;
extern const token_type T_empty;

std::string print_token(Token);

// This is a simple lexer for Qasl. It is flexible,
// in that in lexer.cpp, any of the regex can be
// modified, and QaslLexer will work corresponding,
// provided the regex is valid.
class Lexer {
public:
    Lexer(std::string lexer_file);

    bool matches(std::string, token_type);

    void read_tokens_onto_stack(std::string);
    void read_tokens_onto_stack(std::istream&);

    std::vector<Token> get_token_stack(void);
private:
    std::vector<token_type>             token_order;
    std::map<token_type, std::regex>    regex_map;
    std::set<token_type>                token_ignore_set;

    std::vector<Token> token_stack;
};

}   // qasl

#include "lexer.inl"

#endif  // QASL_LEXER_h
