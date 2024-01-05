/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

namespace qasl {

inline bool
rule_t::operator==(const rule_t& r) {
    return lhs == r.lhs && rhs == r.rhs;
}

inline bool
rule_t::operator<(const rule_t& r) {
    return lhs < r.lhs || (lhs == r.lhs && rhs < r.rhs);
}

inline bool
rule_t::is_valid() {
    if (lhs == T_undefined) return false;
    return true;
}

inline std::string
print_rule(rule_t r) {
    std::string s = r.lhs + " -->";
    for (token_type x : r.rhs) s += " " + x;
    return s;
}

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

}   // qasl
