/**
 * nfa.h
 *
 * Nondeterministic finite automata constructed via Thompson's algorithm.
 * Larger NFAs should be composed by smaller ones via concatenations or
 * joining. By constructing epsilon edges between nodes, one can eventually
 * create a supernode of all nodes reachable by a single node (i.e. the
 * epsilon closure).
 *
 * This class should not be manipulated directly; the @Regex class composes
 * one manually (via the aforementioned compositions) and passes it to a
 * DFA for construction.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_NFA_H
#define SAGE_NFA_H

#include "Automaton.h"
#include "memory.h"

namespace sage
{
    class NFA : public Automaton
    {
        friend class DFA;

        public:

            // Constructors
            NFA();
            NFA(char);
            NFA(char, char);
            virtual ~NFA() = default;
            NFA(const NFA&);
            NFA(NFA&&);
            NFA& operator= (NFA);
            void swap(NFA&, NFA&);

            // Union.
            // Sets up target NFA as immediately accessible.
            void join(std::shared_ptr<NFA>);

            // Concatenation
            // Join two NFAs together.
            void concatenate(std::shared_ptr<NFA>);

            // Operator '*'
            // The Kleene star operation means the previous character needs to be repeated 0 or more times.
            void kleeneStar();

            // Operator '+'
            // The Kleene plus operation means the previous character needs to be repeated 1 or more times.
            void kleenePlus();

            // Operator '?'
            // The optional operator means the previous character can be included or excluded.
            void makeOptional();

        private:

            // Indicates the finishing nodes of the given NFA
            // Note it is important that we do not simply label nodes as finished but
            // instead add it to our given set. This allows for quick construction from
            // multiple NFAs.
            weak_set<Node> finished;
    };
}

#endif //SAGE_NFA_H
