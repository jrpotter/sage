#include <iostream>
#include <fstream>

#include "Parser/Parser.h"

using namespace std;
using namespace sage;

int main(int argc, char** argv)
{
    Parser parser("/home/jrpotter/Documents/sage/grammars/arithmetic.peg");
    std::stringstream ss("195 + (186 * 32) - 14 / 9");
    auto tree = parser.parse(ss);

    return 0;
}