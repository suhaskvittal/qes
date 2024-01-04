/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include "llparser.h"
#include "lexer.h"

#include <iostream>

#include <unistd.h>

namespace qasl {

void
LLParser::read_grammar() {
#ifndef LL_GRAMMAR_FILE
    std::cerr << "[ qasl ] Macro LL_GRAMMAR_FILE is unset." << std::endl;
    exit(1);
#endif
    // Check if LL_GRAMMAR_FILE exists.
    if (faccessat(AT_FDCWD, LL_GRAMMAR_FILE, F_OK, 0) != 0) {
        std::cerr << "[ qasl ] LL_GRAMMAR_FILE \"" << LL_GRAMMAR_FILE
                << "\" does not exist." << std::endl;
        exit(1);
    }
    
}

void
LLParser::compute_first_set() {
    std::map<std::vector<token_type>, std::set<token_type>> _first;

    bool first_has_changed;
    do {
        for (rule_t& r : grammar) {
            token_type nt = r.lhs;
            std::vector<token_type> rule_string = r.lhs;

            token_type x = rule_string[0];
            if (is_terminal(x)) {
                _first[rule_string] = {x};
            } else if (is_nonterminal(x) && first[x].count(token_type::empty)) {
                std::set<token_type> s(first[x]);
                s.erase(token_type::empty);

                std::vector<token_type> sub(rule_string.begin()+1, rule_string.end());
                s.insert(_first[sub].begin(), _first[sub].end());

                _first[rule_string] = s;
            } else if (is_nonterminal(x)) {
                _first[x] = first[x];
            } else {
                _first[x] = { token_type::empty };
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
