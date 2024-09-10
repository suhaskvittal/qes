/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#ifndef QES_SAFE_PARSE_IMPL_h
#define QES_SAFE_PARSE_IMPL_h

#include "qes/lang/instruction.h"
#include "qes/util/parse_network.h"

namespace qes {

// Implementation of all parse functions and data
// structures.

struct network_data_t {
    Program<>       inst_block;
    Instruction<>   inst;

    std::string         instruction_name;
    std::vector<any_t>  instruction_operands;

    int64_t repeat_count;
    int64_t operand_id;

    // A pointer to the potential PC.
    sptr<int64_t> pc_ptr;

    std::string modifier_name;

    std::set<std::string>           annotation_set;
    std::map<std::string, any_t>    property_map;

    any_t   anyval;
};

typedef ParseNetwork<network_data_t>    QesParseNetwork;
typedef parse_node_t<network_data_t>    QesParseNode;

// This function gets an integer that "stands" in for an identifier in an
// anyval. This integer is later replaced with the PC value of the identifier
// for branches.
//
// If the identifier does not have an assigned integer, it is given one.
void    clear_identifier_refs(void);
int64_t get_identifier_ref(std::string);
void    set_identifier_ref_pc(int64_t, sptr<int64_t>);
void    reset_pc(void);
int64_t get_pc(void);
int64_t increment_pc(int64_t by);

void    replace_id_refs_with_pc(Instruction<>&);
void    replace_id_refs_with_pc(Program<>&);

void    p_IDENTIFIER(sptr<QesParseNode>);
void    p_I_LITERAL(sptr<QesParseNode>);
void    p_F_LITERAL(sptr<QesParseNode>);
void    p_S_LITERAL(sptr<QesParseNode>);

void    p_start(sptr<QesParseNode>);
void    p_line(sptr<QesParseNode>);
void    p_instruction(sptr<QesParseNode>);
void    p_modifier(sptr<QesParseNode>);
void    p_operands(sptr<QesParseNode>);
void    p_anyval(sptr<QesParseNode>);

}   // qes

#include "safe_parse_impl.inl"

#endif  // QES_SAFE_PARSE_IMPL_h
