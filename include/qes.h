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

Program<>   from_file(std::string);
void        to_file(std::string, const Program<>&);

std::ostream& operator<<(std::ostream&, const Instruction<>&);
std::ostream& operator<<(std::ostream&, const Program<>&);

}   // qes

#include "qes.inl"

#endif  // QES_h
