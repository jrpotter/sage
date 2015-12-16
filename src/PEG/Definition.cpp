/**
 * definition.cpp
 *
 * A definition serves as the abstract base class of the different components of a definition
 * in a PEG (parsing expression grammar), used in the symbols table when the parser parses the
 * .peg file passed to it.
 *
 * Created by jrpotter (12/16/2015).
 */

#include "PEG/Definition.h"

using namespace sage;

/**
 * Default Constructor
 * ================================
 */
Definition::Definition()
    : repeat_operator(REPEAT_NONE)
{ }