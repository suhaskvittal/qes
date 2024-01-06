/*
 *  author: Suhas Vittal
 *  date:   3 January 2024
 *
 *  Why did I write my own lexer+parser? Bison and Flex drove me insane.
 * */

#ifndef QASL_h
#define QASL_h

#include "qasl/lang/instruction.h"

#include <iostream>

namespace qasl {

Program<>   from_file(std::string);
void        to_file(std::string, const Program<>&);

std::ostream& operator<<(std::ostream&, const Instruction<>&);
std::ostream& operator<<(std::ostream&, const Program<>&);

}   // qasl

#include "qasl.inl"

#endif  // QASL_h
