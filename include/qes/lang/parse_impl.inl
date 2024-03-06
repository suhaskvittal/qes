/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

namespace qes {

inline void
replace_id_refs_with_pc(Program<>& program) {
    for (auto& inst : program) replace_id_refs_with_pc(inst);
}

}   // qes
