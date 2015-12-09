/**
 * pparser.h
 *
 * The following is a simple parser for the simple PEG grammar
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
#include "regex.h"
#include "scanner.h"

#define PPARSER_COMMENT '#'
#define PPARSER_START   '\''

namespace sage
{
    class PEGException : public std::exception
    {
        public:
            PEGException(std::string);
            virtual const char* what() const noexcept;

        private:
            std::string response;
    };

    class PParser
    {
        // The t_choice typedef represents the series of choices a given
        // nonterminal can equate to. The other vector represents tokens
        // separated by the choice operator (i.e. "'") while each nested
        // vector represents either a string (representing another nonterminal)
        // or a regex (representing some primitive/token).
        struct Definition;
        typedef std::vector<std::vector<Definition>> t_choice;

        public:
            PParser(std::string);
            ~PParser();

        private:

            // Source to read from
            Scanner input;
            std::ifstream stream;

            // Tagged Union
            // The following allows distinguishing between either deferring to another
            // nonterminal or instead simply parsing in a specified token
            struct Definition
            {
                enum { TAG_REGEX, TAG_NONTERMINAL } type = 0;
                union {
                    Regex terminal;
                    std::string nonterminal;
                } value;
            };

            // Expression table; start refers to the first nonterminal used
            // in parsing, while the table refers to how to continue parsing
            // the remaining of the stream.
            std::string start;
            std::map<std::string, t_choice> table;

            // Used to actually manipulate and read in the given file
            void parse();
            t_choice parseDefinition(std::string);
    };
}

#endif //SAGE_PPARSER_H
