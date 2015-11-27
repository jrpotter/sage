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

    class DFA {

        public:
            DFA(std::shared_ptr<NFA>);
            DFA(const DFA&);
            DFA(DFA&&);
            DFA& operator= (DFA);
            void swap(DFA&, DFA&);

        private:

            struct Node {
                bool finish = false;
                std::map<NFA::Range, std::weak_ptr<Node>> edges;
            };

            // Refers to the starting point of our graph

            // Refers to all nodes in a given graph
            std::vector<std::shared_ptr<Node>> graph;

    };

}

#endif //SAGE_DFA_H
