/**
 * regex.h
 *
 * The following is the main regular expression interface, used to verify
 * if strings match a given regex. Underneath the class builds an NFA and
 * converts it to a corresponding DFA which it traverses to provide
 * matching functionality.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_REGEX_H
#define SAGE_REGEX_H

#include <list>
#include <string>
#include <sstream>
#include <algorithm>

#include "dfa.h"

namespace sage
{
    class Regex
    {
        public:

            // Constructors
            Regex(std::string);
            ~Regex()=default;
            Regex(const Regex&);
            Regex(Regex&&);
            Regex& operator= (Regex);
            void swap(Regex&, Regex&);

            // Basic operations
            int find(std::string);
            bool matches(std::string, int);

        private:

            // Reference Members
            std::string expr;
            std::unique_ptr<DFA> automaton;

            // Reads in a stream of characters and converts it to a corresponding NFA
            std::shared_ptr<NFA> read(std::stringstream&);

            // Reads in a range of characters (i.e. [-])
            std::shared_ptr<NFA> readRange(std::stringstream&);

            // Utility method to combine NFAs together
            std::shared_ptr<NFA> collapseNFAs(std::list<std::shared_ptr<NFA>>&);
    };

}

#endif //SAGE_REGEX_H
