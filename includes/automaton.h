/**
 * automaton.h
 *
 * The following is the base class for both the NFA (nondeterministic) and DFA (deterministic).
 * Both of these are used by the @regex.h class to convert a regular expression into a state
 * machine to be traversed.
 *
 * Created by jrpotter (11/27/2015).
 */

#ifndef SAGE_AUTOMATON_H
#define SAGE_AUTOMATON_H

#include <set>
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "memory.h"
#include "interval.h"

namespace sage
{
    class Automaton
    {
        public:
            Automaton();
            virtual ~Automaton()=0;

        protected:

            // Represents an element in the FA
            // Note we use weak pointers internally since cycles can crop up
            // when using Thompson's Construction Algorithm (e.g. Kleene Star)
            struct Node {

                public:

                    // An indicator that this node is a finishing node
                    bool finish = false;

                    // @epsilon refers to neighbor edges that can be reached for "free." That is, there is no
                    // requirement to consume a character in order to advance to an NFA in our epsilon vector
                    std::vector<std::weak_ptr<Node>> epsilon;

                    // @ranges represent edges that allow for multiple characters to traverse said edge. Though
                    // initially designed so that each character/string would construct a new edge, this proves
                    // much to expensive.
                    IntervalTree<char, std::weak_ptr<Node>> edges;

                    // Returns a collection of nodes corresponding to the epsilon closure
                    weak_set<Node> epsilonClosure();

                private:

                    // Useful wrapper to actually populate the given set correctly
                    void epsilonClosure(std::weak_ptr<Node>, weak_set<Node>&);

            };

            // In order to handle cyclic dependencies in the construction of the
            // FA, the FA contains a shared pointer to all nodes while nodes
            // contain weak pointer references to one another.
            //
            // When performing an operation that grows the size of the FA, it
            // will join all nodes into its graph.
            std::vector<std::shared_ptr<Node>> graph;

            // The starting node of the given FA
            std::weak_ptr<Node> start;

            // Utility method to construct node
            std::weak_ptr<Node> buildNode();

    };

}

#endif //SAGE_AUTOMATON_H
