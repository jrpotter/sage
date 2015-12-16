/**
 * AST.h
 *
 * The following represents a node in an abstract syntax tree, generated
 * by the Parser class. It is a tagged union, and, as such, constructing
 * and destructing must be manually handled.
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
 *
 * Created by jrpotter (12/16/2015).
 */

#ifndef SAGE_AST_H
#define SAGE_AST_H

#include <memory>
#include <string>
#include <vector>

namespace sage
{
    class AST
    {
        public:

            // Constructors
            AST(std::string);
            AST(std::string, std::string);
            ~AST();

        private:
            std::string type;
            enum { TERMINAL, NONTERMINAL } tag;
            union {
                std::string token;
                std::vector<std::shared_ptr<AST>> branches;
            };
    };
}

#endif //SAGE_AST_H
