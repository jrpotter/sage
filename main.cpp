#include <iostream>
#include <fstream>

#include "Parser/Parser.h"

using namespace std;
using namespace sage;

int main(int argc, char** argv)
{
    //Parser parser("/home/jrpotter/Documents/sage/grammars/arithmetic.peg");
    Regex test("\\babc");
    Regex test2("\\bbc");
    std::stringstream ss("ef    abc");
    Scanner tmp(ss);
    std::cout << test.matches("abc") << std::endl;
    std::cout << test.matches(" abc", 1) << std::endl;
    std::cout << test.matches("abc") << std::endl;
    std::cout << test2.matches("abc", 1) << std::endl;
    std::cout << test2.matches("a bc", 2) << std::endl;
    std::string blah;
    getline(ss, blah);
    std::cout << blah << std::endl;
    return 0;
}