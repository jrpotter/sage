#include <iostream>
#include <fstream>

#include "Parser/Parser.h"

using namespace std;
using namespace sage;

int main(int argc, char** argv)
{
    //Parser parser("/home/jrpotter/Documents/sage/grammars/arithmetic.peg");
    //std::stringstream test("195 + (186 * 32) - 14 / 9");

    Parser parser("/home/jrpotter/Documents/sage/grammars/palindrome.peg");
    std::stringstream test("abacab     a");

    auto tree = parser.parse(test);
    if(tree) {
        std::stringstream ss;
        tree->format(ss);
        std::cout << ss.str() << std::endl;
    } else {
        std::cout << "failure" << std::endl;
    }

    return 0;
}