/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 *
 *  Why did I write my own lexer+parser? Bison and Flex drove me insane.
 * */

#ifndef QASL_h
#define QASL_h

#include <string>

namespace qasl {

enum class token_type {
    // Special:
    undefined,
    empty,
    // Keywords:
    KW_repeat,
    KW_annotation,
    KW_property,
    // Terminals:
    T_identifier,
    T_i_literal,
    T_f_literal,
    // Character terminals (fixed):
    T_comma,        // ,
    T_semicolon,    // ;
    T_pike,         // |
    T_quote,        // " or '
    T_brace_open,   // {
    T_brace_close,  // }
    T_modifier,     // @
    T_whitespace,
    T_comment,      // #
    // Non-terminals:
    NT_start,
    NT_line,
    NT_instruction,
    NT_operands,
    NT_modifier
};

}   // qasl

#include "qasl.inl"

#endif  // QASL_h
