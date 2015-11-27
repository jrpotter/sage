/**
 * nfa.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "nfa.h"

using namespace sage;

/**
 * Epsilon Closure.
 *
 * Finds the epsilon closure of a given Node via a DFS
 * algorithm. The @closure set will be populated with
 * all values by the end.
 */
void NFA::Node::epsilonClosure(std::set<Node*>& closure)
{
    if(closure.find(this) != closure.end()) {
        closure.insert(this);
        for(auto ptr : epsilon) {
            if(auto node = ptr.lock()) {
                node->epsilonClosure(closure);
            }
        }
    }
}

/**
 * Constructor.
 *
 * Note rather than building edges directly, we instead construct new NFAs
 * and apply the different possible operations on them.
 *
 * A range isn't necessarily between just characters. Otherwise I would feel comfortable
 * simply joining a series of NFAs of just character to character. But since ranges can be
 * arbitrarily large, it is faster and more space efficient to simply test for existence
 * within the range (inclusive).
 */
NFA::NFA()
{
    start = buildNode();
}

NFA::NFA(std::string begin)
    :NFA(begin, begin)
{

}

NFA::NFA(std::string begin, std::string end)
{
    start = buildNode();
    auto range = std::make_pair(begin, end);
    if(auto head = start.lock()) {
        auto next = buildNode();
        head->edges[range] = next;
        finished.insert(next);
    }
}

/**
 * Build Node.
 *
 * Utility method for constructing a new node.
 */
std::weak_ptr<NFA::Node> NFA::buildNode()
{
    graph.emplace_back(std::make_shared<Node>());
    return std::weak_ptr<Node>(graph.back());
}

/**
 * Concatenation.
 *
 * Join two NFAs together. This works by constructing epsilon edges from all
 * final states to the start state of @next.
 */
void NFA::concatenate(std::shared_ptr<NFA> tail)
{
    for(auto node : finished) {
        if(auto n_ptr = node.lock()) {
            n_ptr->epsilon.push_back(tail->start);
        }
    }
    finished = tail->finished;
    graph.insert(graph.end(), tail->graph.begin(), tail->graph.end());
}

/**
 * Union.
 *
 * Makes @tail immediately accessible from the head. This should generally be
 * called by an empty NFA when allowing different options down a regex chain.
 */
void NFA::join(std::shared_ptr<NFA> tail)
{
    auto head = buildNode();
    if(auto h_ptr = head.lock()) {
        h_ptr->epsilon.push_back(tail->start);
        start = head;
    }
}

/**
 * Kleene Star.
 *
 * Here we apply Thompson's construction algorithm to allow repetitions (or
 * complete exclusion). All final states (and the new start state) must in turn
 * point back to the old start state.
 *
 * Note that it is important to have a new state s, instead of creating an ϵ-transition
 * back to s1 since repeated Kleene stars could lead to acceptance problems. For example,
 * the regular language (a∗bc)∗ would not be accepted correctly otherwise (since reading
 * an 'a' would force reading 'bc' but the outer Kleene star would bypass this).
 */
void NFA::kleeneStar()
{
    kleenePlus();
    finished.insert(start);
}

/**
 * Kleene Plus.
 *
 * The Kleene plus operation is a composition of concatenation and kleene star. We implement
 * this using both of these operations.
 */
void NFA::kleenePlus()
{
    auto head = buildNode();
    if(auto h_ptr = head.lock()) {
        h_ptr->epsilon.push_back(start);
        for(auto node : finished) {
            if(auto n_ptr = node.lock()) {
                n_ptr->epsilon.push_back(start);
            }
        }
    }
    start = head;
}

/**
 * Optional Operator.
 *
 * The '?' operator allows one to bypass the current character. This works by adding an
 * epsilon edge to the next NFA (disregarding the current NFA).
 */
void NFA::makeOptional()
{
    auto head = buildNode();
    if(auto h_ptr = head.lock()) {
        h_ptr->epsilon.push_back(start);
        for(auto node : finished) {
            if(auto n_ptr = node.lock()) {
                h_ptr->epsilon.push_back(n_ptr);
            }
        }
    }

    start = head;
}