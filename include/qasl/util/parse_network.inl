/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include <deque>

template <class T>
ParseNetwork<T>::ParseNetwork()
    :root(make_node(T_empty)),
    leaves()
{}

template <class T> void
ParseNetwork<T>::recv_rule(rule_t r) {
    // Find first nonterminal == LHS in the leaves.
    sptr<parse_node_t<T>> branch_src = root;

    std::vector<sptr<parse_node_t>> new_leaves;
    for (sptr<parse_node_t<T>> x : leaves) {
        if (x->symbol == r.lhs && branch_src == root) {
            // Create a node for each symbol in the RHS.
            branch_src = x;
        } else {
            new_leaves.push_back(x);
        }
    }
    // branch_src = root or one of the leaves. Now,
    // we need to make a new node for the LHS (nonterminal).
    if (branch_src == root) {
        branch_src = make_node(r.lhs);
        branch_src->parent = root;
        root->children.push_back(branch_src);
    }
    // Expand the tree by branching from branch_src.
    for (token_type t : r.rhs) {
        sptr<parse_node_t<T>> x = make_node(t);
        x->parent = branch_src;
        branch_src->children.push_back(x);
        new_leaves.push_back(x);
    }
    leaves = std::move(new_leaves);
}

template <class T> inline void
ParseNetwork<T>::recv_token(Token tok) {
    // Assign token to the first leaf with the same token_type and
    // has not been assigned a value.
    token_type type = std::get<0>(tok);
    std::string value = std::get<1>(tok);
    for (sptr<parse_node_t<T>> x : leaves) {
        if (x->symbol == type && !x->data_has_been_assigned) {
            x->data.str = value;
            x->data_has_been_assigned = true;
            return;
        }
    }
}

template <class T, class FUNC> inline void
ParseNetwork<T>::apply_callback_bottom_up(FUNC fn) {
    std::deque<sptr<parse_node_t<T>>> node_list(leaves.begin(), leaves.end());
    while (node_list.size()) {
        sptr<parse_node_t<T>> x = node_list.front();
        node_list.pop_front();
        // Call function:
        fn(x);
        // Append parent to list:
        if (x->parent != nullptr) node_list.push_back(x->parent);
    }
}

template <class T> inline sptr<parse_node_t>
ParseNetwork<T>::make_node(token_type t) {
    sptr<parse_node_t<T>> x = std::make_shared<parse_node_t<T>>();
    x->symbol = t;
    return x;
}
