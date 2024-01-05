/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#ifndef QASL_TOKEN_h
#define QASL_TOKEN_h

#include <string>
#include <tuple>

namespace qasl {

typedef std::string token_type;
// Each token has the following entries:
//  (1) token_type
//  (2) value (std::string)
typedef std::tuple<token_type, std::string> Token;

const token_type T_undefined = "undefined";
const token_type T_empty = "empty";

struct rule_t {
    token_type              lhs = T_undefined;
    std::vector<token_type> rhs;

    bool operator==(const rule_t&);
    bool operator<(const rule_t&);
    
    bool is_valid(void);
};

std::string print_token(Token token);
std::string print_rule(rule_t);

}   // qasl

#include "token.inl"

#endif  // QASL_TOKEN_h
