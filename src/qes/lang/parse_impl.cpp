/*
 *  author: Suhas Vittal
 *  date:   29 January 2024
 * */

#include "qes/lang/parse_impl.h"

namespace qes {

static int64_t PC = 0;
static const int ID_REF_OFFSET = 48;

static std::map<std::string, int64_t>   ID_REF_MAP;
static std::map<int64_t, sptr<int64_t>> ID_REF_PC_MAP;

// Need this struct for std::visit.
template <class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

// For tracking the PC, as our parser evaluates the parse tree bottom-up, we
// will track the "negative PC". Hence, an increment actually decrements the PC.
// This is an optimization as when we pass over and replace labels by their PC
// addresses, the address is just
//      id_ref_pc - neg-pc
// i.e.
//      FOO:
//          nop
//          nop
//      BAR:
//          nop
// FOO has a ref pc of -3, BAR has a ref pc of -1; the negative PC is -3. Once
// we go do the replacement as above, their final PCs are 0 and 2 respectively.
// This is exactly what we ant.

void
clear_identifier_refs() {
    ID_REF_MAP.clear();
    ID_REF_PC_MAP.clear();
}

int64_t
get_identifier_ref(std::string id) {
    static int64_t id_ctr = 1;

    if (ID_REF_MAP.count(id)) {
        return ID_REF_MAP.at(id);
    } else {
        int64_t new_id = -(id_ctr << ID_REF_OFFSET);
        ID_REF_MAP[id] = new_id;
        id_ctr++;
        return new_id;
    }
}

void
set_identifier_ref_pc(int64_t ref, sptr<int64_t> pc_ptr) {
    ID_REF_PC_MAP[ref] = pc_ptr;
}

void
replace_id_refs_with_pc(Instruction<>& inst) {
    std::vector<any_t> operands;
    for (any_t op : inst.get_operands()) {
        std::visit(overloads{
                [&] (int64_t x) {
                    if (ID_REF_PC_MAP.count(x)) {
                        operands.emplace_back(PC - *ID_REF_PC_MAP.at(x) - 1);
                    } else {
                        operands.emplace_back(x);
                    }
                },
                [&] (auto x) {
                    operands.emplace_back(x);
                }
            }, op);
    }
    inst.set_operands(operands);
}

void reset_pc() { PC = 0; }
int64_t get_pc() { return PC; }
int64_t increment_pc(int64_t by) { PC += by; return PC; }

}   // qes
