/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 * */

#include <sstream>

namespace qasl {

inline bool
matches(std::string text, token_type t) {
    return std::regex_match(text, REGEX_MAP.at(t));
}

inline std::string
print_token(token_type t) {
    return TOKEN_TEXT_MAP.at(t);
}

inline std::string
print_token(Token token) {
    return TOKEN_TEXT_MAP.at(std::get<0>(token)) + "(" + std::get<1>(token) + ")";
}

inline void
Lexer::read_tokens_onto_stack(std::string text) {
    std::istringstream iss(text);
    read_tokens_onto_stack(iss);
}

inline std::vector<Token>
Lexer::get_token_stack() {
    return token_stack;
}

}   // qasl
