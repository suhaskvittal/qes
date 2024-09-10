/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include <qes.h>

using namespace qes;

int main(int argc, char* argv[]) {
    std::string input_file(argv[1]);

    Program<> prog = fast_read_from_file(input_file);
    std::cout << prog << std::endl;
    return 0;
}
