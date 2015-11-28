/**
 * dfa.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_DFA_H
#define SAGE_DFA_H

#include <memory>
#include <string>

#include "disjoint.h"
#include "nfa.h"

namespace sage {

    class DFA : public Automaton {

        public:
            DFA(std::shared_ptr<NFA>);
            DFA(const DFA&);
            DFA(DFA&&);
            DFA& operator= (DFA);
            void swap(DFA&, DFA&);

    };

}

#endif //SAGE_DFA_H
