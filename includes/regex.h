/**
 * regex.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_REGEX_H
#define SAGE_REGEX_H

#include <list>
#include <sstream>
#include <string>
#include <memory>

#include "nfa.h"
#include "dfa.h"

namespace sage {

    class Regex {

        public:
            Regex(std::string);
            ~Regex()=default;
            Regex(const Regex&);
            Regex(Regex&&);
            Regex& operator= (Regex);
            void swap(Regex&, Regex&);

        private:
            std::string expr;
            std::unique_ptr<DFA> automaton;

            // Reads in a stream of characters and converts it to a corresponding NFA
            std::shared_ptr<NFA> read(std::stringstream&);

            // Reads in a range of characters (i.e. [-])
            std::shared_ptr<NFA> readRange(std::stringstream&);
    };

}

#endif //SAGE_REGEX_H
