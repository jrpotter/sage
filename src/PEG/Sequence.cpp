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
 * correctly, and in the order in which they are tried.
 */
std::shared_ptr<AST> Sequence::process(Scanner& s, const symbol_table& table)
{
    s.saveCheckpoint();
    std::vector<std::shared_ptr<AST>> nodes;
    for(auto node : order) {
        if(auto result = node->parse(s, table)) {
            nodes.push_back(result);
        } else {
            s.restoreCheckpoint();
            return nullptr;
        }
    }

    return std::make_shared<AST>(nodes);
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
