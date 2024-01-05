/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#ifndef QASL_LLPARSER_h
#define QASL_LLPARSER_h

#include "qasl/lang/lexer.h"

#include <map>
#include <set>
#include <vector>

namespace qasl {

// Grammar definitions:

struct rule_t {
    token_type              lhs = T_undefined;
    std::vector<token_type> rhs;

    bool operator==(const rule_t&);
    bool operator<(const rule_t&);
    
    bool is_valid(void);
};

std::string print_rule(rule_t);

class LLParser {
public:
    LLParser(std::string grammar_file);

    void    parse(std::vector<Token>);

    void    compute_first_and_follow_sets(void);
    rule_t  get_rule(token_type, token_type);

    bool                    is_nonterminal(token_type);
    std::set<token_type>    get_nonterminals(void);

    std::set<token_type>&   first(token_type);
    std::set<token_type>&   first(std::vector<token_type>);

    std::set<token_type>&   follow(token_type);
    std::set<token_type>&   follow(std::vector<token_type>);

    std::vector<rule_t> get_grammar(void);
private:
    std::vector<rule_t> grammar;
    std::set<token_type> nonterminals;
        
    std::map<std::vector<token_type>, std::set<token_type>> first_map;
    std::map<std::vector<token_type>, std::set<token_type>> follow_map;

    std::map<token_type, std::map<token_type, int>> parsing_table;
};

}   // qasl

#include "llparser.inl"

#endif  // QASL_LLPARSER_h
