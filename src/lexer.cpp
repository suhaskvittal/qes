/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 * */

#include "lexer.h"

#include <fstream>

#include <fcntl.h>
#include <unistd.h>

namespace qasl {

const token_type T_undefined = "undefined";
const token_type T_empty = "empty";

Lexer::Lexer(std::string lexer_file)
    :token_order(),
    regex_map(),
    token_ignore_set(),
    token_stack()
{
    // Read tokens from token file.
    if (faccessat(AT_FDCWD, lexer_file.c_str(), F_OK, 0) != 0) {
        std::cerr << "[ qasl ] Lexer file \"" << lexer_file << "\" does not exist." << std::endl;
        exit(1);
    }
    // Read the file in.
    std::ifstream fin(lexer_file);
    std::string ln;
    // We need to identify the "tokens" in the line. Unfortunately, we can't exactly bootstrap the lexer
    // with itself (chicken and egg situation).
    std::vector<token_type> keywords;
    std::vector<token_type> not_keywords;
    while (std::getline(fin, ln)) {
        bool ignore_token = false;
        bool is_keyword = false;
        bool started_reading_name = false;
        bool started_reading_regex = false;
        std::string token_name;
        std::string token_regex;
        for (size_t i = 0; i < ln.size(); i++) {
            char c = ln[i];
            if (!started_reading_name && (c == ' ' || c == '\t' || c == '\n')) continue;
            started_reading_name = true;
            if (started_reading_regex) {
                if (c == ' ' || c == '\t' || c == '\n') continue;
                token_regex.push_back(c);
            } else {
                if (c == ' ' || c == '\t' || c == '\n') {
                    started_reading_regex = true;
                } else {
                    if (c == '*') is_keyword = true;
                    else if (c == '^') ignore_token = true;
                    else token_name.push_back(c);
                }
            }
        }
        if (token_name.empty()) continue;
        // Check if the token is a literal.
        if (is_keyword || token_regex.empty()) {
            const std::string special_chars = R"_(*+[](){}.\^)_";
            for (size_t i = 0; i < token_name.size(); i++) {
                char c = token_name[i];
                for (char x : special_chars) {
                    if (c == x) { token_regex.push_back('\\'); break; }
                }
                token_regex.push_back(c);
            }
        }
        // Update token_name with a prefix.
        if (is_keyword) token_name = "KW_" + token_name;
        // Update data structures.
        if (is_keyword) keywords.push_back(token_name);
        else            not_keywords.push_back(token_name);
        regex_map[token_name] = std::regex(token_regex);
        if (ignore_token) token_ignore_set.insert(token_name);
    }
    token_order = std::move(keywords);
    token_order.insert(token_order.end(), not_keywords.begin(), not_keywords.end());
}

void
Lexer::read_tokens_onto_stack(std::istream& input) {
    std::string prev_token;
    std::string curr_token;
    token_type type = T_undefined;

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
            for (token_type t : token_order) {
                if (matches(curr_token, t)) {
                    type = t;
                    goto match_found;
                }
            }
            // If nothing matches the token, then push back prev_token
            // onto the token_stack.
            if (type == T_undefined) {
                // Raise error and exit.
                std::cerr << "Symbol \"" << prev_token << "\" could not be matched to any token." << std::endl;
                exit(1);
            }
            if (!token_ignore_set.count(type)) {
                token_stack.push_back(std::make_tuple(type, prev_token));
            }
            // Reset curr_token to just c.
            curr_token.erase(0, prev_token.size());
            type = T_undefined;
            continue;
        }
match_found:
        get_char = true;
    }
    // Place the existing token onto the stack.
    if (type != T_undefined && !token_ignore_set.count(type)) {
        token_stack.push_back(std::make_tuple(type, curr_token));
    }
}

}   // qasl

