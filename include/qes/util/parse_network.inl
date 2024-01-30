/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include <deque>
#include <iostream>

namespace qes {

template <class T>
ParseNetwork<T>::ParseNetwork()
    :root(nullptr),
    leaves()
{}

template <class T> void
ParseNetwork<T>::recv_rule(rule_t r) {
    // Find first nonterminal == LHS in the leaves.
    sptr<parse_node_t<T>> branch_src = nullptr;

    std::vector<sptr<parse_node_t<T>>> new_leaves,
                                        leaves_after_branch;
    for (sptr<parse_node_t<T>> x : leaves) {
        if (x->symbol == r.lhs && branch_src == nullptr) {
            // Create a node for each symbol in the RHS.
            branch_src = x;
        } else if (branch_src == nullptr) {
            new_leaves.push_back(x);
        } else {
            leaves_after_branch.push_back(x);
        }
    }
    // branch_src = nullptr or one of the leaves. If branch_src
    // is null, then that means the tree is empty. Make LHS the
    // root of the tree.
    if (branch_src == nullptr) {
        root = make_node(r.lhs);
        root->parent = nullptr;
        branch_src = root;
    }
    // Expand the tree by branching from branch_src.
    for (token_type t : r.rhs) {
        sptr<parse_node_t<T>> x = make_node(t);
        x->parent = branch_src;
        branch_src->children.push_back(x);
        new_leaves.push_back(x);
    }
    new_leaves.insert(new_leaves.end(), leaves_after_branch.begin(), leaves_after_branch.end());
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
            x->tmp_data = value;
            x->data_has_been_assigned = true;
            return;
        }
    }
    std::cerr << "could not find leaf for token " << print_token(tok) << "\n";
}

template <class T>
template <class FUNC> inline void
ParseNetwork<T>::apply_callback_bottom_up(FUNC fn) {
    std::deque<sptr<parse_node_t<T>>> node_list(leaves.begin(), leaves.end());
    std::set<sptr<parse_node_t<T>>> ready;
    while (node_list.size()) {
        sptr<parse_node_t<T>> x = node_list.front();
        node_list.pop_front();
        if (ready.count(x)) continue;
        // Check if all children are ready.
        bool is_ready = true;
        for (auto c : x->children) {
            if (!ready.count(c)) {
                is_ready = false;
                break;
            }
        }
        if (!is_ready) continue;
        // Call function:
        fn(x);
        ready.insert(x);
        // Append parent to list:
        if (x->parent != nullptr) node_list.push_back(x->parent);
    }
}

template <class T> inline sptr<parse_node_t<T>>
ParseNetwork<T>::make_node(token_type t) {
    sptr<parse_node_t<T>> x = std::make_shared<parse_node_t<T>>();
    x->symbol = t;
    return x;
}

}   // qes
