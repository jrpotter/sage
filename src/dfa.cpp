/**
 * dfa.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "dfa.h"

using namespace sage;

/**
 * Constructor.
 *
 * This requires breaking up the NFA into powersets based on each
 * node's epsilon closure. Each closure represents a node in the DFA,
 * where each edge of each subnode is then an edge in the current
 * DFA.
 */
DFA::DFA(std::shared_ptr<NFA> automaton)
{
    // We break up all nodes into their closure, adding them into a disjoint
    // set for representational purposes.
    // TODO: Construct an iterator for DisjointSet instead of using exceptions
    DisjointSet<NFA::Node*> powersets;
    for(auto node : automaton->graph) {
        try {
            powersets.findSet(node.get());
        } catch(DisjointSet<NFA::Node*>::NoElementException) {
            std::set<NFA::Node*> closure;
            node->epsilonClosure(closure);
            powersets.createSet(node.get());
            for(auto piece : closure) {
                powersets.createSet(piece);
                powersets.join(node.get(), piece);
            }
        }
    }

    // We can then build the corresponding graph matching the DFA,
    // marking any nodes as finishing nodes if necessary
    auto mapping = powersets.getTable();
    graph.reserve(mapping.size());
    for(auto itr = mapping.begin(); itr != mapping.end(); itr++) {
        graph.emplace_back(std::make_shared<DFA::Node>());
        for(auto node : itr->second) {
            if(automaton->finished.find(node) != automaton->finished.end()) {
                graph.back().finish = true;
            }
        }
    }
    for(int i = 0; i < mapping.size(); i++) {

    }

    // Lastly we link all nodes together
    // Using an iterator, could also perform the std::distance operation
    // better (it runs in O(n) time because of the list iterator)
    for(auto itr = mapping.begin(); itr != mapping.end(); itr++) {
        long index = std::distance(mapping.begin(), itr);
        for(auto node : itr->second) {
            for(auto edge : node->edges) {
                if(auto target = edge.second.lock()) {
                    auto parent = powersets.findSet(target.get());
                    long t_index = std::distance(mapping.begin(), mapping.find(parent));
                    graph[index]->edges[edge.first] = std::weak_ptr<DFA::Node>(graph[t_index]);
                }
            }
        }
    }
}

/**
 * Copy Constructor.
 */
DFA::DFA(const DFA& other)
{

}

/**
 * Move Constructor.
 */
DFA::DFA(DFA&& other)
{

}

/**
 * Assignment Operator.
 */
DFA& DFA::operator= (DFA other)
{
    swap(*this, other);
    return *this;
}

/**
 * Swap Operator.
 */
void DFA::swap(DFA& a, DFA& b)
{
    using std::swap;
}
