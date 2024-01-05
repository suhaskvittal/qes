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

// This is a general Lexer function, whose functionality
// can be specified by the file pointed to by lexer_file
// in the constructor. See data/qasl_lexer.txt for examples.
// At a high level:
//      Token types (terminals for the parser) are specified as
//          <TOKEN_TYPE>    <REGEX>
//      If the token is its own regex (i.e. semicolons), they
//      can simply be declared as
//          <TOKEN_TYPE>
//      with no regex. Finally, if the token is a keyword, this
//      can also be declared with
//          *<TOKEN_TYPE>
class Lexer {
public:
    Lexer(std::string lexer_file);

    bool matches(std::string, token_type);

    // read_tokens is the function that perform lexical_analysis.
    // Passing in a std::string analyzes that string, whereas
    // the std::istream& is more general, and can be used with
    // file input streams.
    void read_tokens(std::string);
    void read_tokens(std::istream&);

    std::vector<Token> get_tokens(void);
private:
    std::vector<token_type>             token_order;
    std::map<token_type, std::regex>    regex_map;
    std::set<token_type>                token_ignore_set;

    std::vector<Token> tokens;
};

}   // qasl

#include "lexer.inl"

#endif  // QASL_LEXER_h
