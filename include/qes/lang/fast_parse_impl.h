/*
 *  author: Suhas Vittal
 *  date:   5 March 2024
 * */

#ifndef QES_FAST_PARSE_IMPL_h
#define QES_FAST_PARSE_IMPL_h

#include "qes/lang/fast_parse.h"

namespace qes {

enum class status_t {
    awaiting_token,
    in_instruction,
    awaiting_modifier,
    in_annotation,
    in_property,
    in_label,
    in_repeat,
    enter_subblock,
    exit_block,
    invalid
};

struct parse_state_t {
    Program<> program;

    std::string inst_name;
    std::vector<any_t> inst_operands;
    std::set<std::string> annotations;
    std::map<std::string, any_t> property_map;

    std::string property_name;

    int64_t repeat_ctr;

    bool in_inst_awaiting_sep = false;
    bool in_property_awaiting_val = false;
    int in_repeat_awaiting_ctr_step = 0;

    void reset() {
        inst_name.clear();
        inst_operands.clear();
        annotations.clear();
        property_map.clear();

        in_inst_awaiting_sep = false;
        in_property_awaiting_val = false;
        in_repeat_awaiting_ctr_step = 0;
    }
};

status_t parse_awaiting_token(std::string, std::string, parse_state_t&);
status_t parse_in_instruction(std::string, std::string, parse_state_t&);
status_t parse_awaiting_modifier(std::string, parse_state_t&);
status_t parse_in_annotation(std::string, std::string, parse_state_t&);
status_t parse_in_property(std::string, std::string, parse_state_t&);
status_t parse_in_label(std::string, std::string, parse_state_t&);
status_t parse_in_repeat(std::string, std::string, parse_state_t&);

any_t get_literal_val(std::string, std::string);

}   // qes

#include "fast_parse_impl.inl"

#endif  // QES_FAST_PARSE_IMPL_h

