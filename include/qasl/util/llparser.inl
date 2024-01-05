/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

namespace qasl {

inline rule_t
LLParser::get_rule(token_type nt, token_type t) {
    const rule_t BAD_RULE = { T_undefined };

    auto& row = parsing_table[nt];
    if (row.count(t)) {
        if (row[t] == -1) {
            return BAD_RULE;
        } else {
            return grammar[row[t]];
        }
    } else {
        // We need to populate the entry.
        for (size_t i = 0; i < grammar.size(); i++) {
            rule_t& r = grammar[i];
            if (r.lhs != nt) continue;
            if (first(r.rhs).count(t) || (first(r.rhs).count(T_empty) && follow(r.lhs).count(t))) {
                row[t] = i;
                return r;
            }
        }
        // Otherwise, the entry could not be populated.
        row[t] = -1;
        return BAD_RULE;
    }
}

inline bool
LLParser::is_nonterminal(token_type t) {
    return nonterminals.count(t);
}

inline std::set<token_type>
LLParser::get_nonterminals() {
    return nonterminals;
}

inline std::set<token_type>&
LLParser::first(token_type t) {
    return first_map[{t}];
}

inline std::set<token_type>&
LLParser::first(std::vector<token_type> t_str) {
    return first_map[t_str];
}

inline std::set<token_type>&
LLParser::follow(token_type t) {
    return follow_map[{t}];
}

inline std::set<token_type>&
LLParser::follow(std::vector<token_type> t_str) {
    return follow_map[t_str];
}

inline std::vector<rule_t>
LLParser::get_grammar() {
    return grammar;
}

// The big parse function:

template <class T> void
LLParser::parse(std::vector<Token> tokens, T& callback_manager) {
    std::vector<token_type> parsing_stack{"$", "start"};

    auto it = tokens.begin();

    while (parsing_stack.size() && it != tokens.end()) {
        Token tok = *it;
        token_type type = std::get<0>(tok);
        std::string value = std::get<1>(tok);

        token_type sym = parsing_stack.back();
        parsing_stack.pop_back();
        if (sym == T_empty) continue; // This is auto matched.
        if (sym == "$") {
            std::cerr << "[ qasl ] parsing error: unexpectedly hit bottom of the stack ($)" << std::endl;
            exit(1);
        } else if (is_nonterminal(sym)) {
            // Push new entries on stack according to rule.
            rule_t r = get_rule(sym, type);
            if (r.lhs == T_undefined) {
                // Try again, with the second argument being T_empty
                r = get_rule(sym, T_empty);
                if (r.lhs == T_undefined) {
                    std::cerr << "[ qasl ] parsing error: failed to get rule for nonterminal "
                        << "\"" << sym << "\" and terminal \"" << type << "\"" << std::endl;
                    exit(1);
                }
            }
            parsing_stack.insert(parsing_stack.end(), r.rhs.rbegin(), r.rhs.rend());
            callback_manager.recv_rule(r);
        } else {
            // Match the token to the stack symbol.
            if (type == sym) {
                callback_manager.recv_token(tok);
                it++;
            } else {
                std::cerr << "[ qasl ] parsing error: mismatched token \"" << print_token(tok)
                    << "\" as terminal of type \"" << sym << "\"" << std::endl;
                exit(1);
            }
        }
    }
}

}   // qasl
