/**
 * terminal.h
 *
 * Represents the regex literal within a given definition. In a PEG grammar,
 * these are always denoted around quotes (""), and all elements are matched
 * as such.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_TERMINAL_H
#define SAGE_TERMINAL_H

#include "definition.h"

namespace sage
{
    class Terminal : public Definition
    {
        private:
            Regex terminal;
    };
}

#endif //SAGE_TERMINAL_H
