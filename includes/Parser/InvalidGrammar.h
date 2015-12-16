/**
 * invalid_grammar.h
 *
 * The exception raised whenever the construction of a PEG fails.
 *
 * Created by jrpotter (11/26/2015).
 */
#ifndef SAGE_INVALID_GRAMMAR_H
#define SAGE_INVALID_GRAMMAR_H

#include <exception>
#include <sstream>

#include "ScanState.h"

namespace sage
{
    class InvalidGrammar : public std::exception
    {
        public:
            InvalidGrammar(std::string);
            InvalidGrammar(std::string, ScanState);
            virtual const char* what() const noexcept;

        private:
            std::string response;
    };
}

#endif //SAGE_INVALID_GRAMMAR_H
