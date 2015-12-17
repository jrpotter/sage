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

#include <memory>
#include <string>
#include <vector>

namespace sage
{
    class AST
    {
        public:
            AST();
            AST(std::string);
            AST(std::string, std::string);
            AST(std::vector<std::shared_ptr<AST>>);
            ~AST();

        //private:
            std::string type;
            enum { EMPTY, TERMINAL, NONTERMINAL, BRANCHES } tag;
            union {
                std::string token;
                std::vector<std::shared_ptr<AST>> branches;
            };
    };
}

#endif //SAGE_AST_H
