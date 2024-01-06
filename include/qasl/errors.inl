/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include <iostream>

#include <fcntl.h>
#include <unistd.h>

namespace qasl {

inline void
exit_macro_does_not_exist(std::string macro_name) {
    std::cerr << "[ qasl ] Macro " << macro_name << " is unset." << std::endl;
    exit(1);
}

inline void
test_file_exists(std::string file_name, std::string macro_name) {
    // Check if LL_GRAMMAR_FILE exists.
    if (faccessat(AT_FDCWD, file_name.c_str(), F_OK, 0) != 0) {
        std::cerr << "[ qasl ] " << macro_name << " \""
                << file_name << "\" does not exist." << std::endl;
        exit(1);
    }
}

}   // qasl
