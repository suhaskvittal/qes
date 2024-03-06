/*
 *  author: Suhas Vittal
 *  date:   5 March 2024
 * */

#include "qes.h"

#include <fstream>

namespace qes {

Program<>
from_simple_file(std::string file) {
    std::ifstream fin(file);
}

}   // qes
