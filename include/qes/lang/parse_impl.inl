/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

namespace qes {

inline void
replace_id_refs_with_pc(Program<>& program) {
    for (auto& inst : program) replace_id_refs_with_pc(inst);
}

inline void
p_IDENTIFIER(sptr<QesParseNode> x) {
    x->data.instruction_name = x->tmp_data;
    x->data.modifier_name = x->tmp_data;
    x->data.anyval = get_identifier_ref(x->tmp_data);
}

inline void
p_I_LITERAL(sptr<QesParseNode> x) {
    const int64_t z = std::stoll(x->tmp_data);
    x->data.repeat_count = z;
    x->data.anyval = z;
}

inline void
p_F_LITERAL(sptr<QesParseNode> x) {
    const double fp = std::stod(x->tmp_data);
    x->data.anyval = fp;
}

inline void
p_S_LITERAL(sptr<QesParseNode> x) {
    x->data.anyval = x->tmp_data;
}

inline void
p_start(sptr<QesParseNode> x) {
    if (x->children.empty() || x->children[0]->symbol == T_empty) return;
    Program<> prog;

    Program<> tail = std::move(x->children.back()->data.inst_block);
    // Check if the children correspond to a repeat block.
    bool is_repeat_block = (x->children[0]->symbol == "KW_repeat");
    if (is_repeat_block) {
        uint64_t n_repeats = x->children[2]->data.repeat_count;
        Program<> blk = std::move(x->children[5]->data.inst_block);

        while (n_repeats--) {
            prog.insert(prog.end(), blk.cbegin(), blk.cend());
        }
    } else {
        // This is just an instruction
        prog.push_back(std::move(x->children[0]->data.inst));
        if (*x->children[0]->data.pc_ptr < 0) {
            *x->children[0]->data.pc_ptr = get_pc();
        }
        increment_pc(1);
    }
    prog.insert(prog.end(), tail.cbegin(), tail.cend());
    x->data.inst_block = std::move(prog);
}

inline void
p_line(sptr<QesParseNode> x) {
    Instruction<> inst;
    sptr<int64_t> pc_ptr = nullptr;
    // Check if line is a modifier or an instruction.
    //
    // Note that as this is bottom-up propagation, the first modifier
    // is the last visited node.
    if (x->children[0]->symbol == "@") {
        // This is a modifier.
        inst = std::move(x->children[2]->data.inst);
        pc_ptr = x->children[2]->data.pc_ptr;
        // Update annotations and properties.
        auto mc = x->children[1];
        for (annotation_t a : mc->data.annotation_set)  inst.put(a);
        for (auto p : mc->data.property_map)            inst.put(p.first, p.second);
    } else if (x->children[1]->symbol == "IDENTIFIER") {
        // This is a label and an instruction.
        inst = std::move(x->children[3]->data.inst);
        pc_ptr = x->children[3]->data.pc_ptr;
        // Set the label's PC.
        int64_t id_ref = std::get<int64_t>(x->children[1]->data.anyval);
        set_identifier_ref_pc(id_ref, pc_ptr);
    } else {
        // This is a simple instruction.
        inst = std::move(x->children[0]->data.inst);
        // As this is the end of an instruction, give this a unique PC.
        pc_ptr = std::make_shared<int64_t>(-1);
    }
    x->data.inst = std::move(inst);
    x->data.pc_ptr = pc_ptr;
}

inline void
p_instruction(sptr<QesParseNode> x) {
    auto c1 = x->children[0],
         c2 = x->children[1];
    x->data.inst = Instruction<>(c1->data.instruction_name, c2->data.instruction_operands);
}

inline void
p_modifier(sptr<QesParseNode> x) {
    // Check if this is an annotation or property.
    std::string modifier_name = x->children[1]->data.modifier_name;
    if (x->children[0]->symbol == "KW_annotation") {
        x->data.annotation_set.insert(modifier_name);
    } else {
        x->data.property_map[modifier_name] = x->children[2]->data.anyval;
    }
}

inline void
p_operands(sptr<QesParseNode> x) {
    // Check if this is the first set of operands (no comma) or this is a later set. Also
    // make sure the first children is not empty (in which case we do nothing).
    if (x->children[0]->symbol == T_empty) return;

    std::vector<any_t> operands;

    size_t off = (x->children[0]->symbol == ",") ? 1 : 0;
    std::vector<any_t> tail = std::move(x->children[1+off]->data.instruction_operands);

    operands.push_back(x->children[off]->data.anyval);
    operands.insert(operands.end(), tail.begin(), tail.end());
    x->data.instruction_operands = std::move(operands);
}

inline void
p_anyval(sptr<QesParseNode> x) {
    // Just pass the child into anyval.
    x->data.anyval = x->children[0]->data.anyval;
}

}   // qes
