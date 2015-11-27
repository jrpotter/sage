/**
 * nfa.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include <cassert>
#include <memory>
#include "nfa.h"

using namespace sage;

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
    :start(std::make_shared<Node>())
{

}

NFA::NFA(std::string begin)
    :NFA(begin, begin)
{

}

NFA::NFA(std::string begin, std::string end)
    :start(std::make_shared<Node>())
{
    auto range = std::make_pair(begin, end);
    start->edges[range] = std::make_shared<Node>();
    finished.insert(start->edges[range]);
}

/**
 * Copy Constructor.
 */
NFA::NFA(const NFA& other)
{
    assert(false);
}

/**
 * Move Constructor.
 */
NFA::NFA(NFA&& other)
{
    assert(false);
}

/**
 * Assignment Operator.
 */
NFA& NFA::operator= (NFA other)
{
    assert(false);
}

/**
 * Swap Operator.
 */
void NFA::swap(NFA& a, NFA& b)
{

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
        node->epsilon.push_back(tail->start);
    }
    finished = tail->finished;
}

/**
 * Union.
 *
 * Makes @tail immediately accessible from the head. This should generally be
 * called by an empty NFA when allowing different options down a regex chain.
 */
void NFA::join(std::shared_ptr<NFA> tail)
{
    auto head = std::make_shared<Node>();
    head->epsilon.push_back(tail->start);
    start = head;
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
    auto head = std::make_shared<Node>();
    head->epsilon.push_back(start);
    for(auto node : finished) {
        node->epsilon.push_back(start);
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
    auto head = std::make_shared<Node>();
    head->epsilon.push_back(start);
    for(auto node : finished) {
        head->epsilon.push_back(node);
    }
    start = head;
}