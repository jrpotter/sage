/**
 * nfa.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_NFA_H
#define SAGE_NFA_H

#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>

#include "memory.h"

namespace sage {

    /**
     * In order to handle cyclic dependencies in the construction of the
     * NFA, the NFA contains a shared pointer to all nodes while nodes
     * contain weak pointer references to one another.
     *
     * When performing an operation that grows the size of the NFA, it
     * will join all nodes into its graph.
     */
    class NFA {

        public:
            NFA();
            NFA(std::string);
            NFA(std::string, std::string);

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

            // Used to represent a range of values (ends inclusive)
            typedef std::pair<std::string, std::string> Range;

            // Represents an element in the NFA
            // It is important to note that, by Thompson's Construction Algorithm,
            // cycles will not occur. Thus it is safe to use shared pointers.
            struct Node {

                // @epsilon refers to neighbor edges that can be reached for "free." That is, there is no
                // requirement to consume a character in order to advance to an NFA in our epsilon vector
                std::vector<std::weak_ptr<Node>> epsilon;

                // @ranges represent edges that allow for multiple characters to traverse said edge. Though
                // initially designed so that each character/string would construct a new edge, this proves
                // much to expensive.
                std::map<NFA::Range, std::weak_ptr<Node>> edges;

            };

            // The starting node of the given NFA
            std::weak_ptr<Node> start;

            // Indicates the finishing nodes of the given NFA
            std::set<std::weak_ptr<Node>, std::owner_less<std::weak_ptr<Node>>> finished;

            // All nodes in the given NFA; used for easier object management
            std::vector<std::shared_ptr<Node>> graph;

            // Builds a new node for use
            std::weak_ptr<Node> buildNode();

    };

}

#endif //SAGE_NFA_H
