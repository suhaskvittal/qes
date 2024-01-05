/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include <qasl/lang/llparser.h>

#include <fstream>
#include <iostream>

using namespace qasl;

int main(int argc, char* argv[]) {
    std::string input_file(argv[1]);

    std::ifstream fin(input_file);

    Lexer lexer(QASL_LEXER_FILE);
    lexer.read_tokens(fin);

    LLParser parser(QASL_LL_GRAMMAR_FILE);

    for (rule_t r : parser.get_grammar()) {
        std::cout << "rule:\t" << print_rule(r) << "\n";
    }
    for (token_type nt : parser.get_nonterminals()) {
        std::cout << "first(" << nt << ") = {";
        for (token_type x : parser.first(nt)) {
            std::cout << " " << x;
        }
        std::cout << " }\n";
        std::cout << "follow(" << nt << ") = {";
        for (token_type x : parser.follow(nt)) {
            std::cout << " " << x;
        }
        std::cout << " }\n";
    }
    parser.parse(lexer.get_tokens());
}
