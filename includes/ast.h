/**
 * ast.cpp
 *
 * Created by jrpotter (12/05/2015).
 */

#ifndef SAGE_AST_H
#define SAGE_AST_H

#include <string>
#include <vector>

namespace sage
{
    class AST
    {
        public:
            AST(std::string);

        private:
            std::string symbol;
            std::vector<AST> branches;
    };
}

#endif //SAGE_AST_H
