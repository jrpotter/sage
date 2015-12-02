/**
 * automaton.cpp
 *
 * Created by jrpotter (11/27/2015).
 */

#include "automaton.h"

using namespace sage;

/**
 * Node Constructor
 * ================================
 */
Automaton::Node::Node(bool finish)
    : finish(finish)
{ }

/**
 * Epsilon Closure.
 * ================================
 *
 * Finds the pointer in question and begins building the closure via
 * a modified form of DFA (in which only epsilon edges are traversed).
 */
const weak_set<Automaton::Node> Automaton::Node::epsilonClosure() const
{
    // We are guaranteed the first element refers to the current node
    weak_set<Node> closure;
    epsilonClosure(epsilon[0], closure);
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
    start = buildNode(false);
}

/**
 * Automaton Destructor
 * ================================
 *
 * Provided to force abstraction of the class.
 */
Automaton::~Automaton()
{ }

/**
 * Automaton Copy Constructor
 * ================================
 *
 * The following goes through and creates a new node for every other node,
 * then maps all the edges correctly.
 */
Automaton::Automaton(const Automaton& other)
{
    std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> mapping;

    // First construct the necessary nodes
    graph.reserve(other.graph.size());
    for(auto node : other.graph) {
        graph.emplace_back(std::make_shared<Node>(node->finish));
        mapping.insert(node, graph.back());
    }

    // Next connect the necessary nodes
    for(auto node : other.graph) {
        auto c_node = mapping.find(node)->second;

        // Epsilon Edges
        for(auto e_edge : node->epsilon) {
            if(auto adj = e_edge.lock()) {
                auto target = mapping.find(adj)->second;
                c_node->epsilon.push_back(target);
            }
        }

        // Other Edges
        for(auto itr = node->edges.begin(); itr != node->edges.end(); itr++) {
            if(auto adj = itr->lock()) {
                auto bounds = itr.bounds();
                auto target = mapping.find(adj)->second;
                c_node->edges.insert(bounds.first, bounds.second, target);
            }
        }
    }

    // Connect our starting node
    if(auto s_ptr = other.start.lock()) {
        start = mapping.find(s_ptr)->second;
    }
}

/**
 * Automaton Move Constructor
 * ================================
 */
Automaton::Automaton(Automaton&& other)
    : Automaton()
{
    swap(*this, other);
}

/**
 * Automaton Assignment Operator
 * ================================
 */
Automaton& Automaton::operator= (Automaton other)
{
    std::swap(*this, other);
    return *this;
}

/**
 * Automaton Swap Operator
 * ================================
 */
void Automaton::swap(Automaton& a, Automaton& b)
{
    using std::swap;
    swap(a.graph, b.graph);
    swap(a.start, b.start);
}

/**
 * Automaton Node Building
 * ================================
 *
 * Must always maintain a list of all nodes but without allowing
 * cyclic shared references amongst the nodes. A vector of all the
 * nodes and weak pointer references amongst the nodes was the settled
 * compromise.
 *
 * Note also that the epsilon closure of every node includes the element itself.
 * The passed @next reference is a weak pointer to the current node in question.
 */
std::weak_ptr<Automaton::Node> Automaton::buildNode(bool finish)
{
    std::shared_ptr<Node> next = std::make_shared<Node>(finish);
    std::weak_ptr<Node> w_next = std::weak_ptr<Node>(next);
    next->epsilon.push_back(w_next);
    graph.push_back(next);
    return w_next;
}