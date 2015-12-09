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
        public:
            PParser(std::string);
            ~PParser();

        private:

            // Source to read from
            Scanner input;
            std::ifstream stream;

            // Expression table; start refers to the first nonterminal used
            // in parsing, while the table refers to how to continue parsing
            // the remaining of the stream.
            std::string start;
            std::map<std::string, std::vector<std::string>> table;

            // Used to actually manipulate and read in the given file
            void parse();
    };
}

#endif //SAGE_PPARSER_H
