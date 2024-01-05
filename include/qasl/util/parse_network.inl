/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include <deque>

inline sptr<parse_node_t>
ParseNetwork::make_node(token_type t) {
    sptr<parse_node_t> x = std::make_shared<parse_node_t>();
    x->symbol = t;
    return x;
}

template <class FUNC> inline void
ParseNetwork::apply_callback_bottom_up(FUNC fn) {
    std::deque<sptr<parse_node_t>> node_list(leaves.begin(), leaves.end());
    while (node_list.size()) {
        sptr<parse_node_t> x = node_list.front();
        node_list.pop_front();
        // Call function:
        fn(x);
        // Append parent to list:
        if (x->parent != nullptr) node_list.push_back(x->parent);
    }
}
