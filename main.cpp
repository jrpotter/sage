#include <iostream>

#include "regex.h"

using namespace std;
using namespace sage;

int main() {

    try {
        Regex r("a[]");
        cout << r.matches("a", 0) << endl;
    } catch(InvalidRegularExpression e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}