/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include "qasl/lang/llparser.h"

#include <fstream>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>

namespace qasl {

LLParser::LLParser(std::string grammar_file) 
    :first(),
    follow(),
    token_stack(),
    grammar()
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
    grammar_lexer.read_tokens_onto_stack(fin);
    // Now, like with Lexer, we can't really parse the tokens because we don't
    // have a Parser yet. Fortunately, the grammar file (should be) parsable by
    // simply reading the tokens in order.
    bool reading_rule = false;
    rule_t current_rule;
    for (Token tok : grammar_lexer.get_token_stack()) {
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
}

void
LLParser::compute_first_set() {
    std::map<std::vector<token_type>, std::set<token_type>> _first;

    bool first_has_changed;
    do {
        for (rule_t& r : grammar) {
            token_type nt = r.lhs;
            std::vector<token_type> rule_string = r.rhs;

            token_type x = rule_string[0];
            if (is_terminal(x)) {
                _first[rule_string] = {x};
            } else if (is_nonterminal(x) && first[x].count(T_empty)) {
                std::set<token_type> s(first[x]);
                s.erase(T_empty);

                std::vector<token_type> sub(rule_string.begin()+1, rule_string.end());
                s.insert(_first[sub].begin(), _first[sub].end());

                _first[rule_string] = s;
            } else if (is_nonterminal(x)) {
                _first[{x}] = first[x];
            } else {
                _first[{x}] = { T_empty };
            }
        }
        // Update the first sets.
        first_has_changed = false;
        for (rule_t& r : grammar) {
            size_t prev_size = first[r.lhs].size();
            first[r.lhs].insert(_first[r.rhs].begin(), _first[r.rhs].end());
            first_has_changed |= prev_size != first[r.lhs].size();
        }
    } while (first_has_changed);
}

}   // qasl
