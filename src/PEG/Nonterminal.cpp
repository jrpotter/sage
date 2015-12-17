/**
 * Nonterminal.cpp
 *
 * Created by jrpotter (12/16/2015).
 */

#include "PEG/Nonterminal.h"

using namespace sage;

/**
 * Default Constructor
 * ================================
 */
Nonterminal::Nonterminal(std::string reference)
    : reference(reference)
{ }

/**
 * Processing
 * ================================
 *
 * Processing a nonterminal merely refers to processing the definition it references.
 */
std::shared_ptr<AST> Nonterminal::process(Scanner& s, const symbol_table& table)
{
    auto itr = table.find(reference);
    if (itr != table.end()) {
        if(auto result = itr->second->parse(s, table)) {
            return std::make_shared<AST>(reference, result);
        }
    }

    return nullptr;
}