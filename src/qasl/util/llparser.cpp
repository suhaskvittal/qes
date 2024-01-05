/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include "qasl/util/lexer.h"
#include "qasl/util/llparser.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>

namespace qasl {

LLParser::LLParser(std::string grammar_file) 
    :grammar(),
    nonterminals(),
    first_map(),
    follow_map()
{
#ifndef GRAMMAR_LEXER_FILE
    std::cerr << "[ qasl ] Macro GRAMMAR_LEXER_FILE is unset." << std::endl;
    exit(1);
#endif
    // Check if LL_GRAMMAR_FILE exists.
    if (faccessat(AT_FDCWD, GRAMMAR_LEXER_FILE, F_OK, 0) != 0) {
        std::cerr << "[ qasl ] GRAMMAR_LEXER_FILE \"" << GRAMMAR_LEXER_FILE
                << "\" does not exist." << std::endl;
        exit(1);
    }
    Lexer grammar_lexer(GRAMMAR_LEXER_FILE);
    // Read grammar file.
    std::ifstream fin(grammar_file);
    grammar_lexer.read_tokens(fin);
    // Now, like with Lexer, we can't really parse the tokens because we don't
    // have a Parser yet. Fortunately, the grammar file (should be) parsable by
    // simply reading the tokens in order.
    bool reading_rule = false;
    rule_t current_rule;
    for (Token tok : grammar_lexer.get_tokens()) {
        token_type type = std::get<0>(tok);
        std::string value = std::get<1>(tok);
        
        if (reading_rule) {
            if (type == "IDENTIFIER") {
                current_rule.rhs.push_back(value);
            } else if (type == "LITERAL") {
                // Remove the surrounding quotes and add the symbol.
                current_rule.rhs.emplace_back(value.begin()+1, value.end()-1);
            } else if (type == ";" || type == "|") {
                // We are done with the current rule.
                if (!current_rule.is_valid()) {
                    std::cerr << "[ qasl ] grammar has invalid rule." << std::endl;
                    exit(1);
                }
                if (current_rule.rhs.empty()) current_rule.rhs.push_back(T_empty);
                grammar.push_back(current_rule);
                if (type == ";") {
                    // Update non-terminal list.
                    nonterminals.insert(current_rule.lhs);
                    // Prepare to read the next nonterminal's rule.
                    current_rule.lhs = T_undefined;
                    reading_rule = false;
                }
                current_rule.rhs.clear();
            } else {
                std::cerr << "[ qasl ] invalid token of type \"" << type 
                        << "\" (value: \"" << value << "\") in rule for non-terminal \"" 
                        << current_rule.lhs << "\"" << std::endl;
                exit(1);
            }
        } else {
            if (type == "IDENTIFIER") {
                current_rule.lhs = value;
            } else if (type == "=") {
                reading_rule = true;
            } else {
                std::cerr << "[ qasl ] invalid token of type \"" << type << "\" (value: \""
                        << value << "\") when reading nonterminal" << std::endl;
                exit(1);
            }
        }
    }
    compute_first_and_follow_sets();
}

void
LLParser::compute_first_and_follow_sets() {
    follow("start") = { "$" };

    bool any_has_changed;
    do {
        any_has_changed = false;
        for (rule_t& r : grammar) {
            for (auto it = r.rhs.begin(); it != r.rhs.end(); it++) {
                std::vector<token_type> first_string(it, r.rhs.end());
                // Get first sets:
                auto& fis = first(first_string);
                std::set<token_type> fis_prev(fis);
                if (is_nonterminal(*it)) {
                    // Get the suffix after *it.
                    std::vector<token_type> suffix(it+1, r.rhs.end());
                    if (suffix.size() == 0) suffix.push_back(T_empty);
                    // Update first sets:
                    if (first(*it).count(T_empty)) {
                        std::set<token_type> s(first(*it));
                        s.erase(T_empty);
                        s.insert(first(suffix).begin(), first(suffix).end());
                        fis = s;
                    } else {
                        fis = first(*it);
                    }
                    // Now, get and update follow sets (only for NTs):
                    auto& fos = follow(*it);
                    std::set<token_type> fos_prev(fos);
                    // Now, update follow based on the suffix.
                    if (suffix.empty() || first(r.lhs).count(T_empty)) {
                        fos.insert(follow(r.lhs).begin(), follow(r.lhs).end());
                    } 
                    for (token_type x : first(suffix)) {
                        if (!is_nonterminal(x) && x != T_empty) {
                            fos.insert(x);
                        }
                    }

                    any_has_changed |= fos != fos_prev;
                } else if (*it != T_empty) {
                    // If *it is a terminal, then it is a simple update to fis.
                    fis = { *it };
                } else if (*it == T_empty && first_string.size() == 1) {
                    fis = { T_empty };
                }
                any_has_changed |= fis != fis_prev;
            }
            size_t prev_fis_size = first(r.lhs).size();
            first(r.lhs).insert(first(r.rhs).begin(), first(r.rhs).end());
            any_has_changed |= first(r.lhs).size() > prev_fis_size;
        }
    } while (any_has_changed);
}

}   // qasl
