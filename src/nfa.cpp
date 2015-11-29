/**
 * nfa.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "nfa.h"

using namespace sage;

/**
 * NFA Constructor (Empty)
 * ================================
 *
 * If no character is passed into the NFA, we have have an empty NFA that
 * will match the empty string. This is useful in the case of actual empty
 * strings or as a starting NFA to begin building from.
 */
NFA::NFA()
{
    if(auto s_ptr = start.lock()) {
        s_ptr->finish = true;
        finished.insert(start);
    }
}

/**
 * NFA Constructor (Two Element)
 * ================================
 *
 * The following builds a single starting node that connects to a single
 * ending node marked final. This is how the NFA should be built up; that
 * is, by constructing smaller NFAs and joining/concatenating them together.
 */
NFA::NFA(char c)
    :NFA(c, c)
{ }

NFA::NFA(char begin, char end)
{
    if(auto s_ptr = start.lock()) {
        auto next = buildNode(false);
        if(auto next_ptr = next.lock()) {
            finished.insert(next);
            next_ptr->finish = true;
            s_ptr->edges.insert(begin, end, next);
        }
    }
}

/**
 * Join
 * ================================
 *
 * Makes @tail immediately accessible from the head. This should generally be
 * called by an empty NFA when allowing different options down a regex chain.
 */
void NFA::join(std::shared_ptr<NFA> tail)
{
    auto head = buildNode(false);
    if(auto h_ptr = head.lock()) {
        h_ptr->epsilon.push_back(tail->start);
        h_ptr->epsilon.push_back(start);
        start = h_ptr;
    }
    finished.insert(tail->finished.begin(), tail->finished.end());
    graph.insert(graph.end(), tail->graph.begin(), tail->graph.end());
}

/**
 * Concatenation.
 * ================================
 *
 * Join two NFAs together. This works by constructing epsilon edges from all
 * final states to the start state of @next.
 */
void NFA::concatenate(std::shared_ptr<NFA> tail)
{
    for(auto node : finished) {
        if(auto n_ptr = node.lock()) {
            n_ptr->finish = false;
            n_ptr->epsilon.push_back(tail->start);
        }
    }
    finished = tail->finished;
    graph.insert(graph.end(), tail->graph.begin(), tail->graph.end());
}

/**
 * Kleene Star
 * ================================
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
    // Allow skipping of the current element to the only element
    // while still enabling repetitions as expected
    kleenePlus();
    if(auto s_ptr = start.lock()) {
        for(auto f_node : finished) {
            if(auto f_ptr = f_node.lock()) {
                s_ptr->epsilon.push_back(f_node);
            }
        }
    }
}

/**
 * Kleene Plus
 * ================================
 *
 * The Kleene plus operation is a composition of concatenation and kleene star. We implement
 * this using both of these operations.
 */
void NFA::kleenePlus()
{
    auto head = buildNode(false);
    auto tail = buildNode(true);

    if(auto h_ptr = head.lock()) {
        h_ptr->epsilon.push_back(start);
        for(auto f_node : finished) {
            if(auto f_ptr = f_node.lock()) {
                f_ptr->finish = false;
                f_ptr->epsilon.push_back(start);
                f_ptr->epsilon.push_back(tail);
            }
        }
    }

    start = head;
    finished = weak_set<Node>({ tail });
}

/**
 * Optional Operator
 * ================================
 *
 * The '?' operator allows one to bypass the current character. This works by adding an
 * epsilon edge to the next NFA (disregarding the current NFA).
 */
void NFA::makeOptional()
{
    auto head = buildNode(false);
    auto tail = buildNode(true);

    if(auto h_ptr = head.lock()) {
        h_ptr->epsilon.push_back(start);
        h_ptr->epsilon.push_back(tail);
        for(auto f_node : finished) {
            if(auto f_ptr = f_node.lock()) {
                f_ptr->finish = false;
                f_ptr->epsilon.push_back(tail);
            }
        }
    }

    start = head;
    finished = weak_set<Node>({ tail });
}