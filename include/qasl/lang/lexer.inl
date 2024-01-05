/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 * */

#include <sstream>

namespace qasl {

inline std::string
print_token(Token token) {
    return std::get<0>(token) + "(" + std::get<1>(token) + ")";
}

inline bool
Lexer::matches(std::string text, token_type t) {
    return std::regex_match(text, regex_map.at(t));
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
