/**
 * choices.h
 *
 * Comes up when parenthesis (outside of quotes) are used, or as the parent level of
 * a definition. In a PEG, each choice is tried in order until success occurs or
 * all options fail.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_CHOICES_H
#define SAGE_CHOICES_H

#include "peg/definition.h"
#include "peg/sequence.h"
#include "string.h"

namespace sage
{
    class Choices : public Definition
    {
        public:
            Choices(std::string);

        protected:
            virtual bool process(Scanner&, std::map<std::string, std::shared_ptr<Definition>>&);

        private:
            std::vector<std::shared_ptr<Definition>> options;
    };
}

#endif //SAGE_CHOICES_H
