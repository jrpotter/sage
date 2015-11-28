/**
 * dfa.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "dfa.h"

using namespace sage;

/**
 * Constructor
 * ================================
 *
 * This requires breaking up the NFA into powersets based on each
 * node's epsilon closure. Each closure represents a node in the DFA,
 * where each edge of each subnode is then an edge in the current
 * DFA.
 */
DFA::DFA(std::shared_ptr<NFA> automaton)
{
    // The strong components of our graph, where we define a
    // strong component to be the epsilon closure of the graph
    DisjointSet<std::shared_ptr<Node>> s_components;

    // The following is a mapping between the representative of
    // each epsilon closure and the epsilon closure itself
    std::map<std::shared_ptr<Node>, long> indices;
    std::map<std::shared_ptr<Node>, weak_set<Node>> powersets;

    // We group all nodes belonging to the same set together
    // by finding the epsilon closure of the node. The closure
    // belongs to the same disjoint set.
    for(auto node : automaton->graph) {

        // Attempt to create new closure. Only necessary if element
        // not encountered before.
        auto repr = s_components.findSet(node);
        if(repr == s_components.end()) {

            // Group epsilon closure together
            s_components.createSet(node);
            auto e_closure = node->epsilonClosure();
            for(auto component : e_closure) {
                auto c = component.lock();
                if(c && c != node) {
                    s_components.createSet(c);
                    s_components.join(c, node);
                }
            }

            // Lastly pair representatives together
            auto parent = *s_components.findSet(node);
            powersets[parent] = e_closure;
            indices[parent] = indices.size();
        }
    }

    // We now have a vector containing our desired powersets
    // We build corresponding DFA nodes for each
    graph.reserve(powersets.size());
    for(int i = 0; i < powersets.size(); i++) {
        buildNode();
    }

    // Lastly we link all nodes together using the disjoint set information and index information
    for(auto ps : powersets) {

        // This is the node we are currently building edges from. This is found by
        // considering all outer edges in our given closure and finding the representatives
        // of each of the connected nodes. The corresponding DFA node can then be found
        // by the @indices map
        auto current = graph[indices[ps.first]];
        for(auto node : ps.second) {
            if(auto n_lock = node.lock()) {

                // Iterate through the edges of each node in the powerset and
                // find the corresponding nodes we should be connecting to
                for(auto it = n_lock->edges.begin(); it != n_lock->edges.end(); it++) {
                    auto range = it.bounds();
                    auto nfa_node = it->lock();
                    auto t_node = indices[*s_components.findSet(nfa_node)];
                    current->edges.insert(range.first, range.second, std::weak_ptr<Node>(graph[t_node]));
                }

            }
        }
    }
}


/**
 * Copy Constructor
 * ================================
 */
DFA::DFA(const DFA& other)
{

}


/**
 * Move Constructor
 * ================================
 */
DFA::DFA(DFA&& other)
{

}


/**
 * Assignment Operator
 * ================================
 */
DFA& DFA::operator= (DFA other)
{
    swap(*this, other);
    return *this;
}


/**
 * Swap Operator
 * ================================
 */
void DFA::swap(DFA& a, DFA& b)
{
    using std::swap;
}