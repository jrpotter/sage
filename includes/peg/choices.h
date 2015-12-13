/**
 * choices.h
 *
 * Comes up when parenthesis (outside of quotes) are used, or as the parent level of
 * a definition. In a PEG, each choice is tried in order until success occurs or
 * all options fail.
 *
 * Created by jrpotter (12/13/2015).
 */

#include "definition.h"

#ifndef SAGE_CHOICES_H
#define SAGE_CHOICES_H

namespace sage
{
    class Choices : public Definition
    {
        private:
            std::vector<Definition> options;
    };
}

#endif //SAGE_CHOICES_H
