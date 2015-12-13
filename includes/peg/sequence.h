/**
 * sequence.h
 *
 * Represents a sequence of terminals and nonterminals that need to be evaluated.
 * E.g. '(' Expression ')' represents a sequence consisting of two terminals and
 * one nonterminal.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_SEQUENCE_H
#define SAGE_SEQUENCE_H

#include "choices.h"
#include "nonterminal.h"
#include "terminal.h"

namespace sage
{
    class Sequence : public Definition
    {
        public:
            Sequence(std::string);

        protected:
            virtual bool process(Scanner&);

        private:
            std::vector<std::shared_ptr<Definition>> order;
    };
}

#endif //SAGE_SEQUENCE_H
