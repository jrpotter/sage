/**
 * Nonterminal.h
 *
 * A nonterminal simply references another definition, provided in the symbol table.
 *
 * Created by jrpotter (12/16/2015).
 */

#ifndef SAGE_NONTERMINAL_H
#define SAGE_NONTERMINAL_H

#include "Definition.h"

namespace sage
{
    class Nonterminal : public Definition
    {
        public:
            Nonterminal(std::string);
            virtual ~Nonterminal() = default;
            virtual std::shared_ptr<AST> process(Scanner&, const symbol_table&);

        private:
            std::string reference;
    };
}

#endif //SAGE_NONTERMINAL_H
