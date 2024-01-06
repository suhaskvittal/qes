/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

namespace qasl {

inline void
p_IDENTIFIER(sptr<QaslParseNode> x) {
    x->data.instruction_name = x->tmp_data;
    x->data.modifier_name = x->tmp_data;
}

inline void
p_I_LITERAL(sptr<QaslParseNode> x) {
    const uint64_t ui = std::stoull(x->tmp_data);
    x->data.operand_id = ui;
    x->data.repeat_count = ui;

    x->data.anyval = static_cast<int64_t>(ui);
}

inline void
p_F_LITERAL(sptr<QaslParseNode> x) {
    const double fp = std::stod(x->tmp_data);
    x->data.anyval = fp;
}

inline void
p_start(sptr<QaslParseNode> x) {
    if (x->children.empty() || x->children[0]->symbol == T_empty) return;
    Program<> prog;

    Program<> tail = std::move(x->children.back()->data.inst_block);
    // Check if the children correspond to a repeat block.
    bool is_repeat_block = (x->children[0]->symbol == "KW_repeat");
    if (is_repeat_block) {
        uint64_t n_repeats = x->children[2]->data.repeat_count;
        Program<> blk = std::move(x->children[5]->data.inst_block);

        Program<> prog;
        while (n_repeats--) {
            prog.insert(prog.end(), blk.cbegin(), blk.cend());
        }
    } else {
        // This is just an instruction
        prog.push_back(std::move(x->children[0]->data.inst));
    }
    prog.insert(prog.end(), tail.cbegin(), tail.cend());
    x->data.inst_block = std::move(prog);
}

inline void
p_line(sptr<QaslParseNode> x) {
    Instruction<> inst;
    // Check if line is a modifier or an instruction.
    //
    // Note that as this is bottom-up propagation, the first modifier
    // is the last visited node.
    if (x->children[0]->symbol == "@") {
        // This is a modifier.
        inst = std::move(x->children[2]->data.inst);
        // Update annotations and properties.
        auto mc = x->children[1];
        for (annotation_t a : mc->data.annotation_set)  inst.put(a);
        for (auto p : mc->data.property_map)            inst.put(p.first, p.second);
    } else {
        // This is a simple instruction.
        inst = std::move(x->children[0]->data.inst);
    }
    x->data.inst = std::move(inst);
}

inline void
p_instruction(sptr<QaslParseNode> x) {
    auto c1 = x->children[0],
         c2 = x->children[1];
    x->data.inst = Instruction<>(c1->data.instruction_name, c2->data.instruction_operands);
}

inline void
p_modifier(sptr<QaslParseNode> x) {
    // Check if this is an annotation or property.
    std::string modifier_name = x->children[1]->data.modifier_name;
    if (x->children[0]->symbol == "KW_annotation") {
        x->data.annotation_set.insert(modifier_name);
    } else {
        x->data.property_map[modifier_name] = x->children[2]->data.anyval;
    }
}

inline void
p_operands(sptr<QaslParseNode> x) {
    // Check if this is the first set of operands (no comma) or this is a later set. Also
    // make sure the first children is not empty (in which case we do nothing).
    if (x->children[0]->symbol == T_empty) return;

    std::vector<uint64_t> operands;

    size_t off = (x->children[0]->symbol == ",") ? 1 : 0;
    std::vector<uint64_t> tail = std::move(x->children[1+off]->data.instruction_operands);

    operands.push_back(x->children[off]->data.operand_id);
    operands.insert(operands.end(), tail.begin(), tail.end());
    x->data.instruction_operands = std::move(operands);
}

inline void
p_anyval(sptr<QaslParseNode> x) {
    // Child is either I_LITERAL or F_LITERAL. Does
    // not matter which.
    x->data.anyval = x->children[0]->data.anyval;
}

}   // qasl
