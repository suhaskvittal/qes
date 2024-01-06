/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#ifndef QASL_PARSE_IMPL_h
#define QASL_PARSE_IMPL_h

#include "qasl/lang/instruction.h"
#include "qasl/util/parse_network.h"

namespace qasl {

// Implementation of all parse functions and data
// structures.

struct network_data_t {
    Program<>       inst_block;
    Instruction<>   inst;

    std::string             instruction_name;
    std::vector<uint64_t>   instruction_operands;

    uint64_t    repeat_count;
    uint64_t    operand_id;

    std::string modifier_name;

    std::set<std::string>               annotation_set;
    std::map<std::string, property_t>   property_map;

    property_t  anyval;
};

typedef ParseNetwork<network_data_t>    QaslParseNetwork;
typedef parse_node_t<network_data_t>    QaslParseNode;

void    p_IDENTIFIER(sptr<QaslParseNode>);
void    p_I_LITERAL(sptr<QaslParseNode>);
void    p_F_LITERAL(sptr<QaslParseNode>);

void    p_start(sptr<QaslParseNode>);
void    p_line(sptr<QaslParseNode>);
void    p_instruction(sptr<QaslParseNode>);
void    p_modifier(sptr<QaslParseNode>);
void    p_operands(sptr<QaslParseNode>);
void    p_anyval(sptr<QaslParseNode>);

}   // qasl

#include "parse_impl.inl"

#endif  // QASL_PARSE_IMPL_h
