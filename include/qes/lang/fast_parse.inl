/*
 *  author: Suhas Vittal
 *  date:   6 March 2024
 * */

namespace qes {

inline std::ostream&
operator<<(std::ostream& out, const debug_state_t& st) {
    out << st.line << ":" << st.col;
    return out;
}

}   // qes
