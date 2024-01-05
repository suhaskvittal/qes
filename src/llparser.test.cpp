/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include <qasl/lang/llparser.h>

using namespace qasl;

int main(int argc, char* argv[]) {
    LLParser parser(QASL_LL_GRAMMAR_FILE);

    for (rule_t r : parser.get_grammar()) {
        std::cout << "rule:\t" << r.lhs << " -->";
        for (token_type x : r.rhs) std::cout << " " << x;
        std::cout << "\n";
    }
}
