/**
 * Terminal.h
 *
 * A terminal represents a regular expression. It is the lowest-level parsing unit
 * in a PEG grammar (in essence, it could be regarded as a token, but this would
 * be a slight simplification as it could technically match delimiters as well).
 *
 * Created by jrpotter (12/16/2015).
 */

#ifndef SAGE_TERMINAL_H
#define SAGE_TERMINAL_H

#include "Definition.h"

namespace sage
{
    class Terminal : public Definition
    {
        public:
            Terminal(std::string);
            virtual ~Terminal() = default;

            // Note the terminal does not need to use a symbol table, but we
            // provide it anyways to force the abstraction process.
            virtual std::shared_ptr<AST> process(Scanner&, const symbol_table&);

        private:
            Regex expr;
    };
}

#endif //SAGE_TERMINAL_H
