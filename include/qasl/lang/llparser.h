/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include "qasl.h"

#include <map>
#include <set>
#include <vector>

namespace qasl {

// Grammar definitions:

struct rule_t {
    token_type              lhs;
    std::vector<token_type> rhs;

    bool operator==(const rule_t&);
    bool operator<(const rule_t&);
};

bool is_terminal(token_type);
bool is_nonterminal(token_type);

class LLParser {
public:
    LLParser(std::string grammar_file);

    void load_token_stack(std::vector<token_type>);
private:
    void read_grammar(void);
    void compute_first_set(void);
        
    std::map<token_type, std::set<token_type>> first;
    std::map<token_type, std::set<token_type>> follow;

    std::vector<token_type> token_stack;
    std::vector<rule_t> grammar;
};

}   // qasl
