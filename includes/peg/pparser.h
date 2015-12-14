/**
 * pparser.h
 *
 * The following is a simple parser for the simple PEG grammar as specified in the different
 * examples provided in the /grammars folder. The resulting table is then used when parsing files
 * according to said PEG.
 *
 * Though it would be nice to be able to construct another DFA-like automaton to traverse when
 * beginning to parse files, I'm not entirely sure how this would work in regards to recursive
 * definitions. Since backtracking needs to be employed anyways, I simply encapsulate each
 * definition and apply each one.
 *
 * Created by jrpotter (12/05/2015).
 */

#ifndef SAGE_PPARSER_H
#define SAGE_PPARSER_H

#include <fstream>

#include "macro.h"
#include "parser/scanner.h"
#include "peg_exception.h"
#include "ptoken.h"

namespace sage
{
    class PParser
    {
        public:
            PParser(std::string);
            ~PParser();

        private:

            // Source to read from
            std::ifstream stream;

            // Expression table; start refers to the first nonterminal used
            // in parsing, while the table refers to how to continue parsing
            // the remaining of the stream.
            std::string start;
            std::map<std::string, std::shared_ptr<PToken>> table;

            // Used to actually manipulate and read in the given file
            void parse(Scanner&);
    };
}

#endif //SAGE_PPARSER_H
