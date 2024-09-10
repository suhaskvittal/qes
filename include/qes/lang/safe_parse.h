/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#ifndef QES_SAFE_PARSE_h
#define QES_SAFE_PARSE_h

#include "qes/lang/instruction.h"

#include <iostream>

namespace qes {

// This file is dedicated to parsing the baseline Qes language.
//
// Extensions to the language will need their own parsing code, but
// can interface with Instruction and ParseNetwork using the templates.

Program<> safe_read_program(std::istream&);

}   // qes

#endif  // QES_SAFE_PARSE_h
