/*
 *  author: Suhas Vittal
 *  date:   5 March 2024
 * */

#ifndef QES_FAST_PARSE_h
#define QES_FAST_PARSE_h

#include "qes/lang/instruction.h"
#include "qes/util/token.h"

#include <iostream>

namespace qes {

struct debug_state_t {
    size_t line;
    size_t col;
};

std::ostream& operator<<(std::ostream&, const debug_state_t&);

Program<> fast_read_program(std::istream&);

Program<> read_block(std::istream&, debug_state_t&);
Token read_next_token(std::istream&, debug_state_t&);

}   // qes

#include "fast_parse.inl"

#endif  // QES_FAST_PARSE_h
