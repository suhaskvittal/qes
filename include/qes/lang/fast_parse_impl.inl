/*
 *  author: Suhas Vittal
 *  date:   6 March 2024
 * */

namespace qes {

inline any_t
get_literal_val(std::string type, std::string val) {
    if (type == "I_LITERAL") return std::stoll(val);
    else if (type == "F_LITERAL") return std::stod(val);
    else                        return val;
}

}   // qes
