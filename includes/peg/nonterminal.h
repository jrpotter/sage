/**
 * nonterminal.h
 *
 * Nonterminals are represented by alphanumeric words without quotes ("") surrounding
 * them. They merely refer to another definition (or the same one recursively) and allow
 * jumping to the referenced definition.
 *
 * Created by jrpotter (12/13/2015).
 */

#include "definition.h"

#ifndef SAGE_NONTERMINAL_H
#define SAGE_NONTERMINAL_H

namespace sage
{
    class Nonterminal : public Definition
    {
        public:
            Nonterminal(std::string);

        protected:
            bool process(Scanner&, std::map<std::string, std::shared_ptr<Definition>>&);

        private:
            std::string reference;
    };
}

#endif //SAGE_NONTERMINAL_H
