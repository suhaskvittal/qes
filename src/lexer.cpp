/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 * */

#include "lexer.h"

#include <map>
#include <regex>
#include <set>

namespace qasl {

#define PUT_TOKEN(t) std::make_pair(token_type::t, ""#t"")

const std::map<token_type, std::string> TOKEN_TEXT_MAP{
    PUT_TOKEN(KW_repeat),
    PUT_TOKEN(T_identifier),
    PUT_TOKEN(T_i_literal),
    PUT_TOKEN(T_f_literal),
    PUT_TOKEN(T_comma),
    PUT_TOKEN(T_semicolon),
    PUT_TOKEN(T_brace_open),
    PUT_TOKEN(T_brace_close),
    PUT_TOKEN(T_modifier),
    PUT_TOKEN(T_whitespace),
    PUT_TOKEN(T_comment)
};

#define PAIR(x, y) std::make_pair(token_type::x, std::regex(y))

const std::map<token_type, std::regex> REGEX_MAP{
    PAIR(KW_repeat, "repeat"),
    PAIR(T_identifier, "[A-Za-z]+"),
    PAIR(T_i_literal, R"_(\d+)_"),
    PAIR(T_f_literal, R"_(\d*\.\d+|\d+\.\d*)_"),
    PAIR(T_comma, ","),
    PAIR(T_semicolon, ";"),
    PAIR(T_brace_open, "\\("),
    PAIR(T_brace_close, "\\)"),
    PAIR(T_modifier, "@"),
    PAIR(T_whitespace, R"_(\s+)_"),
    PAIR(T_comment, R"_(#.*?\n)_")
};

const std::vector<token_type> TOKEN_ORDER{
    token_type::KW_repeat,
    token_type::T_identifier,
    token_type::T_i_literal,
    token_type::T_f_literal,
    token_type::T_comma,
    token_type::T_semicolon,
    token_type::T_brace_open,
    token_type::T_brace_close,
    token_type::T_modifier,
    token_type::T_whitespace,
    token_type::T_comment
};

Lexer::Lexer()
    :token_stack()
{}

void
Lexer::read_tokens_onto_stack(std::istream& input) {
    std::string prev_token;
    std::string curr_token;
    token_type type = token_type::undefined;

    bool get_char = true;
    char c;
    while (!input.eof()) {
        prev_token = curr_token;
        // Update curr token.
        if (get_char) {
            c = input.get();
            curr_token.push_back(c);
        }
        get_char = false;
        // Recheck token regex.
        if (curr_token.size() > 0) {
            for (token_type t : TOKEN_ORDER) {
                if (matches(curr_token, t)) {
                    type = t;
                    goto match_found;
                }
            }
            // If nothing matches the token, then push back prev_token
            // onto the token_stack.
            if (type == token_type::undefined) {
                // Raise error and exit.
                std::cerr << "Symbol \"" << prev_token << "\" could not be matched to any token." << std::endl;
                exit(1);
            }
            token_stack.push_back(std::make_tuple(type, prev_token));
            // Reset curr_token to just c.
            curr_token.erase(0, prev_token.size());
            type = token_type::undefined;
            continue;
        }
match_found:
        get_char = true;
    }
    // Place the existing token onto the stack.
    if (type != token_type::undefined) {
        token_stack.push_back(std::make_tuple(type, curr_token));
    }
}

}   // qasl

