/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#include "qasl/util/parse_network.h"

namespace qasl {

ParseNetwork::ParseNetwork()
    :root(make_node(T_empty)),
    leaves()
{}

void
ParseNetwork::recv_rule(rule_t r) {
    // Find first nonterminal == LHS in the leaves.
    sptr<parse_node_t> branch_src = root;

    std::vector<sptr<parse_node_t>> new_leaves;
    for (sptr<parse_node_t> x : leaves) {
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
        sptr<parse_node_t> x = make_node(t);
        x->parent = branch_src;
        branch_src->children.push_back(x);
        new_leaves.push_back(x);
    }
    leaves = std::move(new_leaves);
}

void
ParseNetwork::recv_token(Token tok) {
    // Assign token to the first leaf with the same token_type and
    // has not been assigned a value.
    token_type type = std::get<0>(tok);
    std::string value = std::get<1>(tok);
    for (sptr<parse_node_t> x : leaves) {
        if (x->symbol == type && !x->data_has_been_assigned) {
            x->data.str = value;
            x->data_has_been_assigned = true;
            return;
        }
    }
}

}   // qasl
