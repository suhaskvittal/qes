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

bool is_terminal(token_type);
bool is_nonterminal(token_type);

class LLParser {
public:
    LLParser(std::string grammar_file);

    void load_token_stack(std::vector<token_type>);

    std::vector<rule_t> get_grammar(void);
private:
    void compute_first_set(void);
        
    std::map<token_type, std::set<token_type>> first;
    std::map<token_type, std::set<token_type>> follow;

    std::vector<token_type> token_stack;
    std::vector<rule_t> grammar;
};

}   // qasl

#include "llparser.inl"

#endif  // QASL_LLPARSER_h
