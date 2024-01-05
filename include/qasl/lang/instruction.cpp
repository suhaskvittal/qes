/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include <variant>

namespace qasl {

class Instruction {
public:
    typedef std::variant<int64_t, uint64_t, double, std::string> operand_t;

    Instruction(std::string);

    const std::string name;
private:
};

}   // qasl
