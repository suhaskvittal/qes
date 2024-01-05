/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include <qasl/lang/lexer.h>

using namespace qasl;

int main(int argc, char* argv[]) {
    std::string text(argv[1]);

    Lexer lexer(QASL_LEXER_FILE);
    lexer.read_tokens(text);
    for (Token t : lexer.get_tokens()) {
        if (std::get<0>(t) == "T_whitespace") continue;
        std::cout << print_token(t) << "\n";
    }

    return 0;
}
