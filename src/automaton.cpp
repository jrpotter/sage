/**
 * automaton.cpp
 *
 * Created by jrpotter (11/27/2015).
 */

#include "automaton.h"

using namespace sage;

/**
 * Epsilon Closure.
 * ================================
 *
 * Finds the pointer in question and begins building the closure via
 * a modified form of DFA (in which only epsilon edges are traversed).
 */
weak_set<Automaton::Node> Automaton::Node::epsilonClosure() const
{
    weak_set<Node> closure;
    closure.insert(epsilon.begin(), epsilon.end());
    for(auto node : closure) {
        epsilonClosure(node, closure);
    }
    return closure;
}

void Automaton::Node::epsilonClosure(std::weak_ptr<Node> n, weak_set<Node>& closure) const
{
    if(closure.find(n) == closure.end()) {
        closure.insert(n);
        if(auto current = n.lock()) {
            for(auto next : current->epsilon) {
                epsilonClosure(next, closure);
            }
        }
    }
}


/**
 * Automaton Constructor.
 * ================================
 *
 * All automatons must have a starting state. This is added to the
 * entire state (note only a weak pointer is maintained).
 */
Automaton::Automaton()
{
    start = buildNode();
}


/**
 * Automaton Destructor
 * ================================
 *
 * Provided to force abstraction of the class.
 */
Automaton::~Automaton()
{

}


/**
 * Automaton Node Building
 * ================================
 *
 * Must always maintain a list of all nodes but without allowing
 * cyclic shared references amongst the nodes. A vector of all the
 * nodes and weak pointer references amonst the nodes was the settled
 * compromise.
 *
 * Note also that the epsilon closure of every node includes the element itself.
 * The passed @next reference is a weak pointer to the current node in question.
 */
std::weak_ptr<Automaton::Node> Automaton::buildNode()
{
    std::shared_ptr<Node> next = std::make_shared<Node>();
    std::weak_ptr<Node> w_next = std::weak_ptr<Node>(next);
    next->epsilon.push_back(w_next);
    graph.push_back(next);
    return w_next;
}