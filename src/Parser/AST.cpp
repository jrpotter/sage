/**
 * AST.cpp
 *
 * Created by jrpotter (12/16/2015).
 */

#include "Parser/AST.h"

using namespace sage;

/**
 * Constructor
 * ================================
 *
 * Here I make a distinction between results returned when parsing results. In particular,
 * a nullptr indicates a failure in parsing while an empty parse tree (i.e. with an empty tag)
 * indicates that parsing was successful but required no nodes (perhaps all elements were optional).
 */
AST::AST()
    : type("")
    , tag(EMPTY)
{ }

/**
 * Constructor (Terminal)
 * ================================
 *
 * This should have a single token corresponding to the matched regex. Note
 * that there does not exist any clear idea of what type an AST is at this
 * level. When conducting contextual analysis, make sure to mark each needed
 * type parameter with a nonterminal.
 */
AST::AST(std::string token)
        : type("")
        , tag(TERMINAL)
        , token(token)
{ }

/**
 * Constructor (Nonterminal)
 * ================================
 *
 * This should simply refer to another AST, but marked with a type (i.e. the name
 * of the nonterminal).
 */
AST::AST(std::string type, std::shared_ptr<AST> child)
        : type(type)
        , tag(NONTERMINAL)
        , child(child)
{ }

/**
 * Constructor (Branches)
 * ================================
 *
 * This should have a series of branches referring to other ASTs.
 */
AST::AST(std::vector<std::shared_ptr<AST>> branches)
        : type("")
        , tag(BRANCHES)
        , branches(branches)
{ }

/**
 * Destructor
 * ================================
 *
 * For my own sake, and a reminder on how unrestricted unions work, I'll write out my
 * understanding on why I need to use the placement new and manually destruct.
 *
 * In essence, placement new is taking in the passed address and setting
 * that as the "this" of the given object. I need to do this because, unless
 * I initialize the necessary variant from the beginning, all elements of
 * the given union will be undefined. As a result, trying to assign the
 * member of the element will not make sense, since the operator = is actually
 * a member of a given object.
 */
AST::~AST()
{
    using namespace std;
    switch(tag) {
        case TERMINAL:
            token.~string();
            break;
        case NONTERMINAL:
            child.~shared_ptr<AST>();
            break;
        case BRANCHES:
            branches.~vector<std::shared_ptr<AST>>();
            break;
        case EMPTY:
            break;
    }
}

/**
 * Display
 * ================================
*/
void AST::format(std::stringstream& output, int level) const
{
    // No output in this case
    if(tag == EMPTY) {
        return;
    } else if(tag != BRANCHES) {
        output << "|-" << std::setfill('-') << std::setw(level * 5) << ' ';
    }

    switch(tag) {
        case TERMINAL:
            output << token << std::endl;
            break;
        case NONTERMINAL:
            output << type << std::endl;
            child->format(output, level + 1);
            break;
        case BRANCHES:
            for(auto branch : branches) {
                branch->format(output, level + 1);
            }
            break;
        default:
            break;
    }
}