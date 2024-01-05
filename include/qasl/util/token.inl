/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

namespace qasl {

inline std::string
print_token(Token token) {
    return std::get<0>(token) + "(" + std::get<1>(token) + ")";
}

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

}   // qasl
