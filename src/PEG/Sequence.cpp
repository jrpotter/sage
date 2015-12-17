/**
 * Sequence.cpp
 *
 * Created by jrpotter (12/16/2015).
 */

#include "PEG/Sequence.h"

using namespace sage;

/**
 * Processing
 * ================================
 *
 * Processing is only successful if every element in the given sequence processes
 * correctly, and in the order in which they are tried. We flatten the tree if
 * possible.
 */
#include <iostream>
std::shared_ptr<AST> Sequence::process(Scanner& s, const symbol_table& table)
{
    auto index = s.saveCheckpoint();
    std::vector<std::shared_ptr<AST>> nodes;

    // Note if there exist no nodes in the order vector, I regard that as an
    // error (someone must've placed a choice operator at the very start of
    // a definition which doesn't make sense). Therefore we return nullptr
    // in this case.
    for(auto node : order) {
        if(auto result = node->parse(s, table)) {
            nodes.push_back(result);
        } else {
            s.restoreCheckpoint(index);
            return nullptr;
        }
    }

    if(nodes.empty()) {
        return nullptr;
    } else if(nodes.size() == 1) {
        return nodes[0];
    } else {
        return std::make_shared<AST>(nodes);
    }
}

/**
 * Appending
 * ================================
 *
 * Utility method to limit access to sequence.
 */
void Sequence::append(std::shared_ptr<Definition> def)
{
    order.push_back(def);
}

/**
 * Last Operator Setting
 * ================================
 *
 * Utility method to set the last operator to the correct repetition count.
 */
void Sequence::setLastOperator(char c)
{
    switch(c) {
        case PPARSER_KLEENE_STAR:
            order.back()->repeat_operator = REPEAT_KLEENE_STAR;
            break;
        case PPARSER_KLEENE_PLUS:
            order.back()->repeat_operator = REPEAT_KLEENE_PLUS;
            break;
        case PPARSER_KLEENE_OPTIONAL:
            order.back()->repeat_operator = REPEAT_OPTIONAL;
            break;
        default:
            order.back()->repeat_operator = REPEAT_NONE;
            break;
    }
}
