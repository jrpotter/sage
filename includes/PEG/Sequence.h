/**
 * Sequence.h
 *
 * A sequence is regarded as a collection of nonterminals and terminals, processed in order
 * in a first-come-first-serve manner. When processing, we say a Sequence successfully
 * constructs its corresponding AST if all elements of the given sequence process correctly,
 * and in the order in which they are reached.
 *
 * Created by jrpotter (12/16/2015).
 */

#ifndef SAGE_SEQUENCE_H
#define SAGE_SEQUENCE_H

#include "Definition.h"

namespace sage
{
    class Sequence : public Definition
    {
        public:
            Sequence() = default;
            virtual ~Sequence() = default;
            virtual std::shared_ptr<AST> process(Scanner&, const symbol_table&);

            // We allow appending to the sequence during the parsing process
            void append(std::shared_ptr<Definition>);

            // Change the last values repeat operator value
            void setLastOperator(char);

        private:
            std::vector<std::shared_ptr<Definition>> order;
    };
}

#endif //SAGE_SEQUENCE_H
