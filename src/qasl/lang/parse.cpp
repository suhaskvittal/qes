/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include "qasl/errors.h"
#include "qasl/lang/parse.h"
#include "qasl/lang/parse_impl.h"
#include "qasl/util/lexer.h"
#include "qasl/util/llparser.h"
#include "qasl/util/parse_network.h"

#include <fcntl.h>
#include <unistd.h>

#include <map>

namespace qasl {

#define PUT(sym)    std::make_pair(" #sym ", &p_##sym)

static const std::map<std::string, void(*)(sptr<QaslParseNode>)> 
    PARSE_FUNCTION_TABLE{
        PUT(IDENTIFIER),
        PUT(I_LITERAL),
        PUT(F_LITERAL),
        PUT(start),
        PUT(line),
        PUT(instruction),
        PUT(modifier),
        PUT(operands),
        PUT(anyval)
    };

Program<>
read_program(std::istream& fin) {
#ifndef QASL_LEXER_FILE
    exit_macro_does_not_exist("QASL_LEXER_FILE");
#endif

#ifndef QASL_LL_GRAMMAR_FILE
    exit_macro_does_not_exist("QASL_LL_GRAMMAR_FILE");
#endif
    // Check that both the lexer file and grammar file both exist.
    test_file_exists(QASL_LEXER_FILE, "QASL_LEXER_FILE");
    test_file_exists(QASL_LL_GRAMMAR_FILE, "QASL_LL_GRAMMAR_FILE");

    QaslParseNetwork net;

    Lexer qasl_lexer(QASL_LEXER_FILE);
    qasl_lexer.read_tokens(fin);

    LLParser qasl_parser(QASL_LL_GRAMMAR_FILE);
    qasl_parser.parse(qasl_lexer.get_tokens(), net);
    
    // Now, the parse network should be populated. We simply need to
    // propagate the data.
    net.apply_callback_bottom_up([&] (sptr<QaslParseNode> x)
    {
        if (!PARSE_FUNCTION_TABLE.count(x->symbol)) return;
        PARSE_FUNCTION_TABLE.at(x->symbol)(x);
    });
    return net.root->data.inst_block;
}

}   // qasl
