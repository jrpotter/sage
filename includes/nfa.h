/**
 * nfa.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_NFA_H
#define SAGE_NFA_H

#include "automaton.h"
#include "memory.h"

namespace sage {

    class NFA : public Automaton {

        friend class DFA;

        public:
            NFA()=default;
            NFA(char);
            NFA(char, char);

            // Concatenation
            // Join two NFAs together.
            void concatenate(std::shared_ptr<NFA>);

            // Union.
            // Sets up target NFA as immediately accessible.
            void join(std::shared_ptr<NFA>);

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
