/**
 * AST.cpp
 *
 * Created by jrpotter (12/16/2015).
 */

#include "Parser/AST.h"

using namespace sage;

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
 * This should simply refer to another AST, but marked with a type.
 */
AST::AST(std::string type, std::string token)
        : type(type)
        , tag(NONTERMINAL)
        , token(token)
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
        case NONTERMINAL:
            token.~string();
            break;
        case BRANCHES:
            branches.~vector<std::shared_ptr<AST>>();
            break;
    }
}