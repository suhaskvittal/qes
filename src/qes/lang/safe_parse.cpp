/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include "qes/errors.h"
#include "qes/lang/safe_parse.h"
#include "qes/lang/safe_parse_impl.h"
#include "qes/util/lexer.h"
#include "qes/util/llparser.h"
#include "qes/util/parse_network.h"

#include <fcntl.h>
#include <unistd.h>

#include <map>

namespace qes {

#define PUT(sym)    std::make_pair( #sym , &p_##sym)

static const std::map<std::string, void(*)(sptr<QesParseNode>)> 
    PARSE_FUNCTION_TABLE{
        PUT(IDENTIFIER),
        PUT(I_LITERAL),
        PUT(F_LITERAL),
        PUT(S_LITERAL),
        PUT(start),
        PUT(line),
        PUT(instruction),
        PUT(modifier),
        PUT(operands),
        PUT(anyval)
    };

Program<>
safe_read_program(std::istream& fin) {
    clear_identifier_refs();
    reset_pc();
#ifndef QES_LEXER_FILE
    exit_macro_does_not_exist("QES_LEXER_FILE");
#endif

#ifndef QES_LL_GRAMMAR_FILE
    exit_macro_does_not_exist("QES_LL_GRAMMAR_FILE");
#endif
    // Check that both the lexer file and grammar file both exist.
    test_file_exists(QES_LEXER_FILE, "QES_LEXER_FILE");
    test_file_exists(QES_LL_GRAMMAR_FILE, "QES_LL_GRAMMAR_FILE");

    QesParseNetwork net;

    Lexer qes_lexer(QES_LEXER_FILE);
    qes_lexer.read_tokens(fin);

    LLParser qes_parser(QES_LL_GRAMMAR_FILE);
    qes_parser.parse(qes_lexer.get_tokens(), net);
    
    // Now, the parse network should be populated. We simply need to
    // propagate the data.
    net.apply_callback_bottom_up([&] (sptr<QesParseNode> x)
    {
        if (!PARSE_FUNCTION_TABLE.count(x->symbol)) return;
        PARSE_FUNCTION_TABLE.at(x->symbol)(x);
    });
    Program<> program = std::move(net.root->data.inst_block);
    replace_id_refs_with_pc(program);
    return program;
}

}   // qes
