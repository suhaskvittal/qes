/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#ifndef QES_INSTRUCTION_h
#define QES_INSTRUCTION_h

#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include <stdint.h>

namespace qes {

typedef std::string annotation_t;
typedef std::variant<int64_t, uint64_t, double, std::string> property_t;
typedef std::variant<int64_t, uint64_t, double, std::string> operand_t;

// Instruction is the basic representation of any command in Qasl.
//
// It is templated (with default arguments) to represent any operand or
// property of the instruction, so it is extensible to custom instructions.
template <class OPERAND=operand_t, class PROPERTY=property_t>
class Instruction {
public:
    Instruction(void);
    Instruction(std::string, std::vector<OPERAND>);
    Instruction(const Instruction&);
    Instruction(Instruction&&);

    template <class T>      Instruction(std::string, std::vector<T>);
    template <class ITER>   Instruction(std::string, ITER begin, ITER end);

    Instruction& operator=(const Instruction&);

    // This function gets an operand, assuming the value is T. If it is not,
    // then std::variant will throw an error.
    OPERAND              get(size_t) const;
    template <class T> T get(size_t) const;

    // put(std::string) adds an annotation, put(std::string, *) adds a property.
    void    put(std::string);
    void    put(std::string, PROPERTY);

    bool    has_annotation(std::string) const;
    bool    has_property(std::string) const;

    PROPERTY             get_property(std::string) const;
    template <class T> T get_property(std::string) const;
    // Merges the operands of two instructions together.
    void        join(const Instruction&);

    std::string             get_name(void) const;
    std::vector<OPERAND>    get_operands(void) const;
    std::set<annotation_t>  get_annotations(void) const;

    std::map<std::string, PROPERTY> get_property_map(void) const;
private:
    std::string             name;
    std::vector<OPERAND>    operands;

    std::set<annotation_t>          annotations;
    std::map<std::string, PROPERTY> property_map;
};

// Prints the instruction as it would appear in Qasl. If print_inline = false,
// then newlines are used for readability.
template <class T, class U>
std::string print_inst(const Instruction<T, U>&, bool print_inline=true);

template <class OPERAND=operand_t, class PROPERTY=property_t>
using Program=std::vector<Instruction<OPERAND, PROPERTY>>;

// This function is never going to print inline. Most useful to dump a Qasl
// program to a file.
template <class T, class U>
std::string print_prog(const Program<T, U>&);

}   // qes

#include "instruction.inl"

#endif  // QES_INSTRUCTION_h
