/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include <qasl.h>

using namespace qasl;

int main(int argc, char* argv[]) {
    std::string input_file(argv[1]);

    Program<> prog = from_file(input_file);
    std::cout << prog << std::endl;
    return 0;
}
