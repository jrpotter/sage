/**
 * Choices.h
 *
 * Choices refer to the set of sequences (potentially just one element long) that a certain definition
 * can refer to. These are delimited by the choice operator ('|') and are each attempted in a
 * first-come-first-serve basis. That is, an alternative definition provided after a choice operator
 * is only attempted if the first definition fails, unlike in a CFG.
 *
 * This is also the top-level class for every definition hierarchy, even if there exists only one
 * choice for a given definition (i.e. there does not exist any choice operators).
 *
 * Created by jrpotter (12/16/2015).
 */

#ifndef SAGE_CHOICES_H
#define SAGE_CHOICES_H

#include "Nonterminal.h"
#include "Sequence.h"
#include "Terminal.h"

namespace sage
{
    class Choices : public Definition
    {
        public:
            Choices(Scanner&);
            Choices(Scanner&&);
            virtual ~Choices() = default;
            virtual std::shared_ptr<AST> process(Scanner&, const symbol_table&);

        private:
            std::vector<std::shared_ptr<Sequence>> options;
    };
}

#endif //SAGE_CHOICES_H
