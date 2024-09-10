/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#ifndef QES_LLPARSER_h
#define QES_LLPARSER_h

#include "qes/util/token.h"

#include <map>
#include <set>
#include <vector>

namespace qes {

// Grammar definitions:

// The LLParser implements an LLParser according
// to the passed in grammar file. See data/qes_grammar.txt
// for examples. At a high level:
//      A production rule can be declared as follows:
//          <nonterminal> = <rule1> | <rule2> | ... | <rulek>;
//      One of these nonterminals must be called "start".
//
//      Terminals used in the productions must match those
//      used by the Lexer (if one is used). If a keyword
//      terminal is used, make sure to prefix the keyword
//      with KW_<keyword>.
class LLParser {
public:
    LLParser(std::string grammar_file);

    // parse is the main parsing function that parses a 
    // sequence of Tokens (which can be retrieved from a 
    // Lexer). The function is generic, and a callback_manager
    // can be used to interact with the parser.
    // The callback_manager should implement two functions:
    //      (1) recv_token(Token). Upon matching a token to
    //          a symbol on the parsing stack, this will be called.
    //      (2) recv_rule(rule_t). Upon using a rule, this
    //          will be called.
    template <class T>
    void parse(std::vector<Token>, T& callback_manager);

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

}   // qes

#include "llparser.inl"

#endif  // QES_LLPARSER_h
