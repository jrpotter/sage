/**
 * dfa.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "dfa.h"

using namespace sage;

/**
 * Constructor.
 */
DFA::DFA()
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
