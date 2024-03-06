/*  
 *  author: Suhas Vittal
 *  date:   5 March 2024
 * */

#include "qes/lang/fast_parse_impl.h"

namespace qes {

status_t
parse_awaiting_token(std::string type, std::string val, parse_state_t& st) {
    if (type == "IDENTIFIER") {
        st.inst_name = val;
        return status_t::in_instruction;
    } else if (type == "@") {
        return status_t::awaiting_modifier;
    } else if (type == "(") {
        return status_t::in_label;
    } else if (type == "repeat") {
        if (st.annotations.size() || st.property_map.size()) {
            // This means we were expecting an instruction, but we got a repeat.
            return status_t::invalid;
        }
        return status_t::in_repeat;
    } else if (type == "}") {
        return status_t::exit_block;
    }
    return status_t::invalid;
}

status_t
parse_in_instruction(std::string type, std::string val, parse_state_t& st) {
    if (type == "I_LITERAL" || type == "F_LITERAL" || type == "S_LITERAL") {
        if (st.in_inst_awaiting_sep) {
            return status_t::invalid;
        }
        st.inst_operands.push_back(get_literal_val(type, val));
        st.in_inst_awaiting_sep = true;
        return status_t::in_instruction;
    } else if (type == "," && st.in_inst_awaiting_sep) {
        st.in_inst_awaiting_sep = false;
        return status_t::in_instruction;
    } else if (type == ";") {
        // Push the instruction onto the program.
        Instruction<> inst(st.inst_name, st.inst_operands);
        for (std::string a : st.annotations) {
            inst.put(a);
        }
        for (auto& [ k, v ] : st.property_map) {
            inst.put(k, v);
        }
        st.program.push_back(inst);

        st.reset();
        return status_t::awaiting_token;
    }
    return status_t::invalid;
}

status_t
parse_awaiting_modifier(std::string type, parse_state_t& st) {
    if (type == "annotation") {
        return status_t::in_annotation;
    } else if (type == "property") {
        return status_t::in_property;
    }
    return status_t::invalid;
}

status_t
parse_in_annotation(std::string type, std::string val, parse_state_t& st) {
    if (type != "IDENTIFIER") return status_t::invalid;
    st.annotations.insert(val);
    return status_t::awaiting_token;
}

status_t
parse_in_property(std::string type, std::string val, parse_state_t& st) {
    if (st.in_property_awaiting_val) {
        if (type == "I_LITERAL" || type == "F_LITERAL" || type == "S_LITERAL") {
            st.property_map[st.property_name] = get_literal_val(type, val);
            return status_t::awaiting_token;
        } else {
            return status_t::invalid;
        }
    } else {
        if (type != "IDENTIFIER") {
            return status_t::invalid;
        } else {
            st.property_name = type;
            st.in_property_awaiting_val = true;
            return status_t::in_property;
        }
    }
    return status_t::invalid;
}

status_t
parse_in_label(std::string type, std::string val, parse_state_t& st) {
    // TODO: implement labels.
    return status_t::invalid;
}

status_t
parse_in_repeat(std::string type, std::string val, parse_state_t& st) {
    if (type == "(" && st.in_repeat_awaiting_ctr_step == 0) {
        st.in_repeat_awaiting_ctr_step++;
        return status_t::in_repeat;
    } else if (type == "I_LITERAL" && st.in_repeat_awaiting_ctr_step == 1) {
        st.repeat_ctr = std::stoll(val);
        st.in_repeat_awaiting_ctr_step++;
        return status_t::in_repeat;
    } else if (type == ")" && st.in_repeat_awaiting_ctr_step == 2) {
        st.in_repeat_awaiting_ctr_step++;
        return status_t::in_repeat;
    } else if (type == "{" && st.in_repeat_awaiting_ctr_step == 3) {
        return status_t::enter_subblock;
    }
    return status_t::invalid;
}

}   // qes
