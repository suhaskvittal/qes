/*
 *  author: Suhas Vittal
 *  date:   4 January 2024
 * */

#ifndef QES_PARSE_NETWORK_h
#define QES_PARSE_NETWORK_h

#include "qes/util/token.h"

#include <memory>
#include <vector>

#include <stdint.h>

namespace qes {

template <class T> using sptr=std::shared_ptr<T>;

// parse_node_t has been templated with a template T
// that refers to some data (i.e. a struct).
// This is so that the user can control what data exists 
// in each node.
template <class T>
struct parse_node_t {
    token_type  symbol;
    T           data;

    std::string tmp_data; // Raw data from recv_token
    bool data_has_been_assigned = false;
    // Pointers to parent and children:
    sptr<parse_node_t>              parent = nullptr;
    std::vector<sptr<parse_node_t>> children;
};

// A ParseNetwork is the simplest example
// of a callback_manager, where 
//  (1) recv_rule adds a node to the graph (one per symbol
//      in the production rule) and necessary edges.
//  (2) recv_token attaches a value onto a leaf on the graph.
// After the ParseNetwork is construct upon completing
// parse, a user can simply analyze the ParseNetwork to
// extract data.
template <class T>
class ParseNetwork {
public:
    ParseNetwork();
    
    void    recv_rule(rule_t);
    void    recv_token(Token);

    // Calls FUNC on every node, from the leaves up to the root by
    // traversing using the parent pointers.
    template <class FUNC> void apply_callback_bottom_up(FUNC);

    sptr<parse_node_t<T>>              root;
    std::vector<sptr<parse_node_t<T>>> leaves;
private:
    sptr<parse_node_t<T>>  make_node(token_type);
};

}   // qes

#include "parse_network.inl"

#endif  // QES_PARSE_NETWORK_h
