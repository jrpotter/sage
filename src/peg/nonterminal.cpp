/**
 * nonterminal.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "peg/nonterminal.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
Nonterminal::Nonterminal(std::string reference)
        : reference(reference)
{ }

/**
 * Process
 * ================================
 *
 * Reference corresponding map and try to process instead
 */
bool Nonterminal::process(Scanner& s, std::map<std::string, std::shared_ptr<Definition>>& table)
{
    if (table.find(reference) != table.end()) {
        return table[reference]->process(s, table);
    }
    return false;
}