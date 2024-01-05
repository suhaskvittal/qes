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

inline std::vector<rule_t>
LLParser::get_grammar() {
    return grammar;
}

inline bool is_nonterminal(token_type) { return true; }
inline bool is_terminal(token_type) { return true; }

}   // qasl
