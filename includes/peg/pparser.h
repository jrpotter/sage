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

#include "exception/peg_exception.h"
#include "parser/scanner.h"

#define PPARSER_COMMENT '#'
#define PPARSER_START   '\''

namespace sage
{
    class PParser
    {
        // The t_choice typedef represents the series of choices a given
        // nonterminal can equate to. The other vector represents tokens
        // separated by the choice operator (i.e. "'") while each nested
        // vector represents either a string (representing another nonterminal)
        // or a regex (representing some primitive/token).
        struct Definition;
        using t_choice = std::vector<std::vector<Definition>>;

        public:
            PParser(std::string);
            ~PParser();

        private:

            // Source to read from
            std::ifstream stream;

            // Tagged Union
            // The following allows distinguishing between either deferring to another
            // nonterminal or instead simply parsing in a specified token
            struct Definition
            {
                enum { TAG_TERMINAL, TAG_NONTERMINAL } type;
                union {
                    Regex terminal;
                    std::string nonterminal;
                };

                // Constructors
                Definition(Regex);
                Definition(std::string);
                Definition(const Definition&);
                ~Definition();
            };

            // Expression table; start refers to the first nonterminal used
            // in parsing, while the table refers to how to continue parsing
            // the remaining of the stream.
            std::string start;
            std::map<std::string, t_choice> table;

            // Used to actually manipulate and read in the given file
            void parse(Scanner&);
            t_choice parseDefinition(std::string);
    };
}

#endif //SAGE_PPARSER_H
