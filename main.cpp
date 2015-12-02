#include <iostream>
#include <fstream>

#include "regex.h"
#include "scanner.h"

using namespace std;
using namespace sage;

int main(int argc, char** argv)
{
    std::ifstream sample("/home/jrpotter/Documents/sage/test.txt", ios::in);

    Scanner s = Scanner(sample);

    std::cout << s.next(Regex::WORD) << std::endl;
    std::cout << s.next(Regex::FLOAT) << std::endl;
    std::cout << s.next(Regex::WORD) << std::endl;

    sample.close();

    return 0;
}