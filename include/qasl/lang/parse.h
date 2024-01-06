/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#ifndef QASL_PARSE_h
#define QASL_PARSE_h

#include "qasl/lang/instruction.h"

#include <iostream>

namespace qasl {

// This file is dedicated to parsing the baseline Qasl language.
//
// Extensions to the language will need their own parsing code, but
// can interface with Instruction and ParseNetwork using the templates.

Program read_program(std::istream&);

}   // qasl

#endif  // QASL_PARSE_h
