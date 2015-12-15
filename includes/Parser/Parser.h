/**
 * parser.h
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

#ifndef SAGE_PEG_PARSER_H
#define SAGE_PEG_PARSER_H

#include <fstream>

#include "macro.h"

#include "Scanner.h"
#include "PEGException.h"
#include "PEGToken.h"

namespace sage
{
    class Parser
    {
        public:
            Parser(std::string);
            ~Parser();

            void parse(std::istream&);

        private:

            // Source to read from
            std::ifstream init_stream;

            // Expression table; start refers to the first nonterminal used
            // in parsing, while the table refers to how to continue parsing
            // the remaining of the stream.
            std::string start;
            std::map<std::string, std::shared_ptr<PEGToken>> table;

            // Used to actually manipulate and read in the given file
            void initializeTable(Scanner&);
    };
}

#endif //SAGE_PEG_PARSER_H
