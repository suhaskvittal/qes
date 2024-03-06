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

// We provide two functions for reading from files.
//      from_file is very generic and works with the specified grammar. It can be quite slow
//      for dense programs.
//
//      from_simple_file assumes that there are only quantum operations and does not invoke
//      the parser/lexer. Instead, the characters are read one by one and assumed to follow
//      the format of 
//          instruction operand,operand,operand,...;    --> all operands are integers.
//      Annotations and properties are also allowed.
Program<>   from_file(std::string);
Program<>   from_simple_file(std::string);

void        to_file(std::string, const Program<>&);

std::ostream& operator<<(std::ostream&, const Instruction<>&);
std::ostream& operator<<(std::ostream&, const Program<>&);

}   // qes

#include "qes.inl"

#endif  // QES_h
