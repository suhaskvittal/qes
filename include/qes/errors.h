/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#ifndef QASL_ERRORS_h
#define QASL_ERRORS_h

#include <string>

namespace qes {

void    exit_macro_does_not_exist(std::string macro_name);
void    test_file_exists(std::string, std::string macro_name);

}   // qes

#include "errors.inl"

#endif  // QASL_ERRORS_h
