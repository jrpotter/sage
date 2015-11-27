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
