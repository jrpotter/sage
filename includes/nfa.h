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

    class NFA {

        public:
            NFA();
            NFA(std::string);
            NFA(std::string, std::string);
            NFA(const NFA&);
            NFA(NFA&&);
            NFA& operator= (NFA);
            void swap(NFA&, NFA&);

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
                std::vector<std::shared_ptr<Node>> epsilon;

                // @ranges represent edges that allow for multiple characters to traverse said edge. Though
                // initially designed so that each character/string would construct a new edge, this proves
                // much to expensive.
                std::map<NFA::Range, std::shared_ptr<Node>> edges;

            };

            // The starting node of the given NFA
            std::shared_ptr<Node> start;

            // Indicates the finishing nodes of the given NFA
            std::set<std::shared_ptr<Node>> finished;

    };

}

#endif //SAGE_NFA_H
