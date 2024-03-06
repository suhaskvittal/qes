/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include "qes/lang/safe_parse.h"
#include "qes/lang/fast_parse.h"

#include <fstream>

namespace qes {

inline Program<>
safe_read_from_file(std::string input_file) {
    std::ifstream fin(input_file);
    return safe_read_program(fin);
}

inline Program<>
fast_read_from_file(std::string input_file) {
    std::ifstream fin(input_file);
    return fast_read_program(fin);
}

inline void
write_to_file(std::string output_file, const Program<>& prog) {
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

}   // qes
