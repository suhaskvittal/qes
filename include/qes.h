/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 *
 *  Why did I write my own lexer+parser? Bison and Flex drove me insane.
 * */

#ifndef QES_h
#define QES_h

#include "qes/lang/instruction.h"

#include <iostream>

namespace qes {

// We provide two functions for IO. 
//  safe_read_from_file uses the lexer and grammar to parse the input file. This approach
//  is not hardcoded, and can be controlled by updating the lexer/grammar and safe_parse
//  and safe_parse_impl.
//
//  fast_read_from_file is hard-coded and does not use the provided lexer or grammar files.
//  This function should be used for a stable version of qes. Consequently, new capabilities
//  should be first tested with safe_read_from_file and then implemented in fast_read_from_file.
Program<>   safe_read_from_file(std::string);
Program<>   fast_read_from_file(std::string);

Program<>   from_file(std::string); // alias for fast_read_from_file.

void        to_file(std::string, const Program<>&);

std::ostream& operator<<(std::ostream&, const Instruction<>&);
std::ostream& operator<<(std::ostream&, const Program<>&);

}   // qes

#include "qes.inl"

#endif  // QES_h
