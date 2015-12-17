/**
 * AST.h
 *
 * The following represents a node in an abstract syntax tree, generated
 * by the Parser class. It is a tagged union, and, as such, constructing
 * and destructing must be manually handled.
 *
 * Created by jrpotter (12/16/2015).
 */

#ifndef SAGE_AST_H
#define SAGE_AST_H

#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

namespace sage
{
    class AST
    {
        public:
            AST();
            AST(std::string);
            AST(std::string, std::shared_ptr<AST>);
            AST(std::vector<std::shared_ptr<AST>>);
            ~AST();

            // Useful for quick analyzing of tree
            void format(std::stringstream&, int=0) const;

        private:
            std::string type;
            enum { EMPTY, TERMINAL, NONTERMINAL, BRANCHES } tag;
            union {
                std::string token;
                std::shared_ptr<AST> child;
                std::vector<std::shared_ptr<AST>> branches;
            };
    };
}

#endif //SAGE_AST_H
