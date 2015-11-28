/**
 * dfa.h
 *
 * Deterministic finite automaton. This should only ever be constructed
 * by a corresponding NFA by finding epsilon closures and mapping these
 * nodes as new nodes in the new FA.
 *
 * This is also the underlying engine of the regex, allowing for quick
 * matching of a string, especially via the use of the interval map.
 * Given a string of length b, we expect an indicator specifying a
 * match or not to take, in the worst case, blog(n), where n is the
 * average number of edges out of each node of the DFA.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_DFA_H
#define SAGE_DFA_H

#include "disjoint.h"
#include "nfa.h"

namespace sage
{
    class DFA : public Automaton
    {
        public:
        
            // Constructors
            DFA(std::shared_ptr<NFA>);

            // Operations to move through the DFA
            void reset();
            bool final() const;
            bool traverse(char);

        private:
        
            // The marker specifying the state the DFA is currently on.
            // This should be @reset internally during each call required
            // to traverse the machine.
            std::weak_ptr<Node> cursor;
    };
}

#endif //SAGE_DFA_H
