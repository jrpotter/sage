/**
 * definition.h
 *
 * The following is an abstract base class used to represent the RHS of a PEG definition;
 * that is, the segment following the arrow operator ('->'). By representing it as such,
 * one can apply backtracking methods when testing whether a certain means of parsing works
 * as expected.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_DEFINITION_H
#define SAGE_DEFINITION_H

#include <string>

#include "parser/scanner.h"
#include "regex/regex.h"

namespace sage
{
    class Definition
    {
        friend class Sequence;

        public:
            Definition();
            virtual ~Definition() = default;

            // The following will run through and attempt to read in from
            // the scanner the corresponding definition if possible. It
            // will return whether or not it was successful.
            virtual bool process(Scanner&, std::map<std::string, std::shared_ptr<Definition>>&) = 0;

        protected:

            // Determines whether or not this element should be
            // repeated, and how much. The following enumerations
            // correspond to those in regular expressions.
            enum {
                REPEAT_KLEENE_STAR,
                REPEAT_KLEENE_PLUS,
                REPEAT_OPTIONAL,
                REPEAT_NONE
            } repeat;

    };
}

#endif //SAGE_DEFINITION_H
