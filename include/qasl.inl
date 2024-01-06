/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include "qasl/lang/parse.h"

#include <fstream>

namespace qasl {

inline Program<>
from_file(std::string input_file) {
    std::ifstream fin(input_file);
    return read_program(fin);
}

inline void
to_file(std::string output_file, const Program<>& prog) {
    std::ofstream fout(output_file);
    fout << prog << std::endl;
}

inline std::ostream&
operator<<(std::ostream& out, const Instruction<>& inst) {
    out << print_inst(inst);
    return out;
}

inline std::ostream&
operator<<(std::ostream& out, const Program<>& prog) {
    out << print_prog(prog);
    return out;
}

}   // qasl
