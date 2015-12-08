/**
 * pparser.h
 *
 * The following is a recursive descent parser for the simple PEG grammar
 * as specified in the different examples provided in the /grammars folder.
 * The resulting table is then used when parsing files according
 * to said PEG.
 *
 * Created by jrpotter (12/05/2015).
 */

#ifndef SAGE_PPARSER_H
#define SAGE_PPARSER_H

#include <fstream>

#include "ast.h"
#include "scanner.h"

#define PPARSER_COMMENT '#'
#define PPARSER_START   '\''

namespace sage
{
    class PParser
    {
        public:
            PParser(std::string);
            ~PParser();

        private:
            Scanner input;
            std::ifstream stream;

            // Expression table; start refers to the first nonterminal used
            // in parsing, while the table refers to how to continue parsing
            // the remaining of the stream.
            std::string start;
            std::map<std::string, AST> table;

            // These regexes are used to read in a custom PEG grammer.
            // Refer to /grammars/arithmetic.peg for a more thorough explanation
            // on the grammar. Note all other terms can be manipulated just
            // by reading in the remainder of a line or reading in words.
            Regex arrowOperator;

            // Used to actually manipulate and read in the given file
            void parse();
    };
}

#endif //SAGE_PPARSER_H
