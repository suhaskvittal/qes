/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 * */

#include <sstream>

namespace qes {

inline bool
Lexer::matches(std::string text, token_type t) {
    return std::regex_match(text, regex_map.at(t));
}

inline void
Lexer::read_tokens(std::string text) {
    std::istringstream iss(text);
    read_tokens(iss);
}

inline std::vector<Token>
Lexer::get_tokens() {
    return tokens;
}

}   // qes
